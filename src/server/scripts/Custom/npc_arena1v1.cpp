/*
 *
 * Copyright (C) 2013 Emu-Devstore <http://emu-devstore.com/>
 * Written by Teiby <http://www.teiby.de/>
 *
 */

#include "ScriptMgr.h"
#include "ArenaTeamMgr.h"
#include "Common.h"
#include "DisableMgr.h"
#include "BattlegroundMgr.h"
#include "Battleground.h"
#include "ArenaTeam.h"
#include "Language.h"

// TalentTab.dbc -> TalentTabID
const uint32 FORBIDDEN_TALENTS_IN_1V1_ARENA[] = 
{
    // Healer
    201, // Priest Discipline
    202, // Priest Holy
    382, // Paladin Holy
    262, // Shaman Restoration
    282, // Druid Restoration

    // Tanks
    //383, // Paladin Protection
    //163, // Warrior Protection

    0 // End
};

bool Arena1v1CheckTalents(Player* player)
{
    if (!player)
        return false;

    if (!sWorld->getBoolConfig(CONFIG_ARENA_1V1_BLOCK_FORBIDDEN_TALENTS))
        return true;

    uint32 count = 0;
    for (uint32 talentId = 0; talentId < sTalentStore.GetNumRows(); ++talentId)
    {
        TalentEntry const* talentInfo = sTalentStore.LookupEntry(talentId);

        if (!talentInfo)
            continue;

        for (int8 rank = 0; rank < MAX_TALENT_RANK; ++rank)
        {
            if (talentInfo->RankID[rank] == 0)
                continue;

            if (player->HasTalent(talentInfo->RankID[rank], player->GetActiveSpec()))
            {
                for (int8 i = 0; FORBIDDEN_TALENTS_IN_1V1_ARENA[i] != 0; ++i)
                    if (FORBIDDEN_TALENTS_IN_1V1_ARENA[i] == talentInfo->TalentTab)
                        count += rank + 1;
            }
        }
    }

    if (count > 35)
    {
        ChatHandler(player->GetSession()).SendSysMessage("You can't join 1v1 Arena because you have put too many points in a forbidden talent trees. Please change your talents.");
        return false;
    }
    else
        return true;
}

class npc_1v1arena : public CreatureScript
{
public:
    npc_1v1arena() : CreatureScript("npc_1v1arena") 
    {
    }

    bool JoinQueueArena(Player* player, Creature* me, bool isRated)
    {
        if (!player || !me)
            return false;

        if (sWorld->getIntConfig(CONFIG_ARENA_1V1_MIN_LEVEL) > player->getLevel())
            return false;

        uint64 guid = player->GetGUID();
        uint8 arenaslot = ArenaTeam::GetSlotByType(ARENA_TEAM_1v1);
        uint32 arenaRating = 0;
        uint32 matchmakerRating = 0;

        // ignore if we already in BG or BG queue
        if (player->InBattleground())
            return false;

        //check existance
        Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(BATTLEGROUND_AA);
        if (!bg)
        {
            TC_LOG_ERROR("network", "Battleground: template bg (all arenas) not found");
            return false;
        }

        if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, BATTLEGROUND_AA, NULL))
        {
            ChatHandler(player->GetSession()).PSendSysMessage(LANG_ARENA_DISABLED);
            return false;
        }

        BattlegroundTypeId bgTypeId = bg->GetTypeID();
        BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, ARENA_TYPE_1v1);
        PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), player->getLevel());
        if (!bracketEntry)
            return false;

        GroupJoinBattlegroundResult err = ERR_GROUP_JOIN_BATTLEGROUND_FAIL;

        // check if already in queue
        if (player->GetBattlegroundQueueIndex(bgQueueTypeId) < PLAYER_MAX_BATTLEGROUND_QUEUES)
            return false;

        // check if has free queue slots
        if (!player->HasFreeBattlegroundQueueId())
            return false;

        uint32 ateamId = 0;

        if (isRated)
        {
            ateamId = player->GetArenaTeamId(arenaslot);
            ArenaTeam* at = sArenaTeamMgr->GetArenaTeamById(ateamId);
            if (!at)
            {
                player->GetSession()->SendNotInArenaTeamPacket(ARENA_TYPE_1v1);
                return false;
            }

            // get the team rating for queueing
            arenaRating = at->GetRating();
            matchmakerRating = arenaRating;
            // the arena team id must match for everyone in the group

            if (arenaRating <= 0)
                arenaRating = 1;
        }

        BattlegroundQueue &bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);

        if (bgQueue.IPExistsInQueue(player, NULL, bracketEntry, isRated, false))
        {
            ChatHandler(player->GetSession()).SendSysMessage("You cannot join 1v1 Arena now because there is already someone queued with the same IP address.");
            return false;
        }

        bg->SetRated(isRated);

        GroupQueueInfo* ginfo = bgQueue.AddGroup(player, NULL, bgTypeId, bracketEntry, ARENA_TYPE_1v1, isRated, false, arenaRating, matchmakerRating, ateamId);
        uint32 avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
        uint32 queueSlot = player->AddBattlegroundQueueId(bgQueueTypeId);

        WorldPacket data;
        // send status packet (in queue)
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, queueSlot, STATUS_WAIT_QUEUE, avgTime, 0, ARENA_TYPE_1v1, 0);
        player->GetSession()->SendPacket(&data);

        sBattlegroundMgr->ScheduleQueueUpdate(matchmakerRating, ARENA_TYPE_1v1, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());

        return true;
    }


    bool CreateArenaTeam(Player* player, Creature* me)
    {
        if (!player || !me)
            return false;

        uint8 slot = ArenaTeam::GetSlotByType(ARENA_TEAM_1v1);
        if (slot >= MAX_ARENA_SLOT)
            return false;

        // Check if player is already in an arena team
        if (player->GetArenaTeamId(slot))
        {
            player->GetSession()->SendArenaTeamCommandResult(ERR_ARENA_TEAM_CREATE_S, player->GetName(), "", ERR_ALREADY_IN_ARENA_TEAM);
            return false;
        }


        // Teamname = playername
        // if teamname exist, we have to choose another name (playername + number)
        int i = 1;
        std::stringstream teamName;
        teamName << player->GetName();
        do
        {
            if (sArenaTeamMgr->GetArenaTeamByName(teamName.str()) != NULL) // teamname exist, so choose another name
            {
                teamName.str(std::string());
                teamName << player->GetName() << i++;
            }
            else
                break;
        } while (i < 100); // should never happen

        // Create arena team
        ArenaTeam* arenaTeam = new ArenaTeam();

        if (!arenaTeam->Create(player->GetGUID(), ARENA_TEAM_1v1, teamName.str(), 4283124816, 45, 4294242303, 5, 4294705149))
        {
            delete arenaTeam;
            return false;
        }

        // Register arena team
        sArenaTeamMgr->AddArenaTeam(arenaTeam);
        arenaTeam->AddMember(player->GetGUID());

        ChatHandler(player->GetSession()).SendSysMessage("1v1 Arena Team successful created!");

        return true;
    }

    bool OnGossipHello(Player* player, Creature* me)
    {
        if (!player || !me)
            return true;

        if (sWorld->getBoolConfig(CONFIG_ARENA_1V1_ENABLE) == false)
        {
            ChatHandler(player->GetSession()).SendSysMessage("1v1 Arena is currently disabled.");
            return true;
        }

        if (player->InBattlegroundQueueForBattlegroundQueueType(BATTLEGROUND_QUEUE_1v1))
                player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, "Leave 1v1 Arena Queue", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4, "Are you sure?", 0, false);
        else
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Sign up for 1v1 Arena (Unrated)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);

        if (player->GetArenaTeamId(ArenaTeam::GetSlotByType(ARENA_TEAM_1v1)) == NULL)
            player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, "Create new 1v1 Arena Team", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1, "Create 1v1 Arena Team?", sWorld->getIntConfig(CONFIG_ARENA_1V1_COSTS), false);
        else if (!player->InBattlegroundQueueForBattlegroundQueueType(BATTLEGROUND_QUEUE_1v1))
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Sign up for 1v1 Arena (Rated)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, "Disband 1v1 Arena Team", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5, "Are you sure?", 0, false);
        }

        player->SEND_GOSSIP_MENU(68, me->GetGUID());
        return true;
    }



    bool OnGossipSelect(Player* player, Creature* me, uint32 /*uiSender*/, uint32 uiAction)
    {
        if (!player || !me)
            return true;

        player->PlayerTalkClass->ClearMenus();

        switch (uiAction)
        {
        case GOSSIP_ACTION_INFO_DEF+1: // Create new 1v1 Arena Team
            {
                if (sWorld->getIntConfig(CONFIG_ARENA_1V1_MIN_LEVEL) <= player->getLevel())
                {
                    if (player->GetMoney() >= sWorld->getIntConfig(CONFIG_ARENA_1V1_COSTS) && CreateArenaTeam(player, me))
                        player->ModifyMoney(sWorld->getIntConfig(CONFIG_ARENA_1V1_COSTS) * -1);
                }
                else
                {
                    ChatHandler(player->GetSession()).PSendSysMessage("You need level %u+ to create an 1v1 Arena Team.", sWorld->getIntConfig(CONFIG_ARENA_1V1_MIN_LEVEL));
                    player->CLOSE_GOSSIP_MENU();
                    return true;
                }
            }
            break;

        case GOSSIP_ACTION_INFO_DEF+2: // Join 1v1Arena Queue (Rated)
            {
                if (Arena1v1CheckTalents(player) && !JoinQueueArena(player, me, true))
                    ChatHandler(player->GetSession()).SendSysMessage("Something went wrong when joining the queue.");
                
                player->CLOSE_GOSSIP_MENU();
                return true;
            }
            break;

        case GOSSIP_ACTION_INFO_DEF+3: // Join 1v1 Arena Queue (Unrated)
            {
                if (Arena1v1CheckTalents(player) && !JoinQueueArena(player, me, false))
                    ChatHandler(player->GetSession()).SendSysMessage("Something went wrong when joining the queue.");
                
                player->CLOSE_GOSSIP_MENU();
                return true;
            }
            break;

        case GOSSIP_ACTION_INFO_DEF+4: // Leave Queue
            {
                WorldPacket Data;
                Data << (uint8)0x1 << (uint8)0x0 << (uint32)BATTLEGROUND_AA << (uint16)0x0 << (uint8)0x0;
                player->GetSession()->HandleBattleFieldPortOpcode(Data);
                player->CLOSE_GOSSIP_MENU();
                return true;
            }
            break;
        case GOSSIP_ACTION_INFO_DEF+5: // Disband arena team
            {
                WorldPacket Data;
                Data << (uint32)player->GetArenaTeamId(ArenaTeam::GetSlotByType(ARENA_TEAM_1v1));
                player->GetSession()->HandleArenaTeamLeaveOpcode(Data);
                ChatHandler(player->GetSession()).SendSysMessage("Arena Team deleted!");
                player->CLOSE_GOSSIP_MENU();
                return true;
            }
            break;
        }

        OnGossipHello(player, me);
        return true;
    }
};


void AddSC_npc_1v1arena()
{
    new npc_1v1arena();
}