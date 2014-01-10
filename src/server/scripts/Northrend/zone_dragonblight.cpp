/*
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
SDName: Dragonblight
SD%Complete: 100
SDComment:
SDCategory: Dragonblight
EndScriptData */

/* ContentData
npc_alexstrasza_wr_gate
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "ScriptedEscortAI.h"
#include "Vehicle.h"
#include "CombatAI.h"
#include "Player.h"

/*#####
# npc_commander_eligor_dawnbringer
#####*/

enum CommanderEligorDawnbringer
{
    MODEL_IMAGE_OF_KELTHUZAD           = 24787, // Image of Kel'Thuzad
    MODEL_IMAGE_OF_SAPPHIRON           = 24788, // Image of Sapphiron
    MODEL_IMAGE_OF_RAZUVIOUS           = 24799, // Image of Razuvious
    MODEL_IMAGE_OF_GOTHIK              = 24804, // Image of Gothik
    MODEL_IMAGE_OF_THANE               = 24802, // Image of Thane Korth'azz
    MODEL_IMAGE_OF_BLAUMEUX            = 24794, // Image of Lady Blaumeux
    MODEL_IMAGE_OF_ZELIEK              = 24800, // Image of Sir Zeliek
    MODEL_IMAGE_OF_PATCHWERK           = 24798, // Image of Patchwerk
    MODEL_IMAGE_OF_GROBBULUS           = 24792, // Image of Grobbulus
    MODEL_IMAGE_OF_THADDIUS            = 24801, // Image of Thaddius
    MODEL_IMAGE_OF_GLUTH               = 24803, // Image of Gluth
    MODEL_IMAGE_OF_ANUBREKHAN          = 24789, // Image of Anub'rekhan
    MODEL_IMAGE_OF_FAERLINA            = 24790, // Image of Faerlina
    MODEL_IMAGE_OF_MAEXXNA             = 24796, // Image of Maexxna
    MODEL_IMAGE_OF_NOTH                = 24797, // Image of Noth
    MODEL_IMAGE_OF_HEIGAN              = 24793, // Image of Heigan
    MODEL_IMAGE_OF_LOATHEB             = 24795, // Image of Loatheb

    NPC_IMAGE_OF_KELTHUZAD             = 27766, // Image of Kel'Thuzad
    NPC_IMAGE_OF_SAPPHIRON             = 27767, // Image of Sapphiron
    NPC_IMAGE_OF_RAZUVIOUS             = 27768, // Image of Razuvious
    NPC_IMAGE_OF_GOTHIK                = 27769, // Image of Gothik
    NPC_IMAGE_OF_THANE                 = 27770, // Image of Thane Korth'azz
    NPC_IMAGE_OF_BLAUMEUX              = 27771, // Image of Lady Blaumeux
    NPC_IMAGE_OF_ZELIEK                = 27772, // Image of Sir Zeliek
    NPC_IMAGE_OF_PATCHWERK             = 27773, // Image of Patchwerk
    NPC_IMAGE_OF_GROBBULUS             = 27774, // Image of Grobbulus
    NPC_IMAGE_OF_THADDIUS              = 27775, // Image of Thaddius
    NPC_IMAGE_OF_GLUTH                 = 27782, // Image of Gluth
    NPC_IMAGE_OF_ANUBREKHAN            = 27776, // Image of Anub'rekhan
    NPC_IMAGE_OF_FAERLINA              = 27777, // Image of Faerlina
    NPC_IMAGE_OF_MAEXXNA               = 27778, // Image of Maexxna
    NPC_IMAGE_OF_NOTH                  = 27779, // Image of Noth
    NPC_IMAGE_OF_HEIGAN                = 27780, // Image of Heigan
    NPC_IMAGE_OF_LOATHEB               = 27781, // Image of Loatheb

    NPC_INFANTRYMAN                    = 27160, // Add in case I randomize the spawning
    NPC_SENTINAL                       = 27162,
    NPC_BATTLE_MAGE                    = 27164,

    // Five platforms to choose from
    SAY_PINNACLE                       = 0,
    SAY_DEATH_KNIGHT_WING              = 1,
    SAY_ABOMINATION_WING               = 2,
    SAY_SPIDER_WING                    = 3,
    SAY_PLAGUE_WING                    = 4,
    // Used in all talks
    SAY_TALK_COMPLETE                  = 5,
    // Pinnacle of Naxxramas
    SAY_SAPPHIRON                      = 6,
    SAY_KELTHUZAD_1                    = 7,
    SAY_KELTHUZAD_2                    = 8,
    SAY_KELTHUZAD_3                    = 9,
    // Death knight wing of Naxxramas
    SAY_RAZUVIOUS                      = 10,
    SAY_GOTHIK                         = 11,
    SAY_DEATH_KNIGHTS_1                = 12,
    SAY_DEATH_KNIGHTS_2                = 13,
    SAY_DEATH_KNIGHTS_3                = 14,
    SAY_DEATH_KNIGHTS_4                = 15,
    // Blighted abomination wing of Naxxramas
    SAY_PATCHWERK                      = 16,
    SAY_GROBBULUS                      = 17,
    SAY_GLUTH                          = 18,
    SAY_THADDIUS                       = 19,
    // Accursed spider wing of Naxxramas
    SAY_ANUBREKHAN                     = 20,
    SAY_FAERLINA                       = 21,
    SAY_MAEXXNA                        = 22,
    // Dread plague wing of Naxxramas
    SAY_NOTH                           = 23,
    SAY_HEIGAN_1                       = 24,
    SAY_HEIGAN_2                       = 25,
    SAY_LOATHEB                        = 26,

    SPELL_HEROIC_IMAGE_CHANNEL         = 49519,

    EVENT_START_RANDOM                 = 1,
    EVENT_MOVE_TO_POINT                = 2,
    EVENT_TALK_COMPLETE                = 3,
    EVENT_GET_TARGETS                  = 4,
    EVENT_KELTHUZAD_2                  = 5,
    EVENT_KELTHUZAD_3                  = 6,
    EVENT_DEATH_KNIGHTS_2              = 7,
    EVENT_DEATH_KNIGHTS_3              = 8,
    EVENT_DEATH_KNIGHTS_4              = 9,
    EVENT_HEIGAN_2                     = 10
};

uint32 const AudienceMobs[3] = { NPC_INFANTRYMAN, NPC_SENTINAL, NPC_BATTLE_MAGE };

Position const PosTalkLocations[6] =
{
    { 3805.453f, -682.9075f, 222.2917f, 2.793398f }, // Pinnacle of Naxxramas
    { 3807.508f, -691.0882f, 221.9688f, 2.094395f }, // Death knight wing of Naxxramas
    { 3797.228f, -690.3555f, 222.5019f, 1.134464f }, // Blighted abomination wing of Naxxramas
    { 3804.038f, -672.3098f, 222.5019f, 4.578917f }, // Accursed spider wing of Naxxramas
    { 3815.097f, -680.2596f, 221.9777f, 2.86234f  }, // Dread plague wing of Naxxramas
    { 3798.05f,  -680.611f,  222.9825f, 6.038839f }, // Home
};

class npc_commander_eligor_dawnbringer : public CreatureScript
{
    public: npc_commander_eligor_dawnbringer() : CreatureScript("npc_commander_eligor_dawnbringer") {}

        struct npc_commander_eligor_dawnbringerAI : public ScriptedAI
        {
            npc_commander_eligor_dawnbringerAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                talkWing = 0;
                memset(audienceList, 0, sizeof(audienceList));
                memset(imageList, 0, sizeof(imageList));
                _events.ScheduleEvent(EVENT_GET_TARGETS, 5000);
                _events.ScheduleEvent(EVENT_START_RANDOM, 20000);
            }

            void MovementInform(uint32 type, uint32 id) OVERRIDE
            {
                if (type == POINT_MOTION_TYPE)
                {
                    if (id == 1)
                    {
                        me->SetFacingTo(PosTalkLocations[talkWing].m_orientation);
                        TurnAudience();

                        switch (talkWing)
                        {
                        case 0: // Pinnacle of Naxxramas
                            {
                                switch (urand (0, 1))
                                {
                                    case 0: ChangeImage(NPC_IMAGE_OF_KELTHUZAD, MODEL_IMAGE_OF_KELTHUZAD, SAY_KELTHUZAD_1);
                                            _events.ScheduleEvent(EVENT_KELTHUZAD_2, 8000); break;
                                    case 1: ChangeImage(NPC_IMAGE_OF_SAPPHIRON, MODEL_IMAGE_OF_SAPPHIRON, SAY_SAPPHIRON); break;
                                }
                            }
                            break;
                        case 1: // Death knight wing of Naxxramas
                            {
                                switch (urand (0, 2))
                                {
                                    case 0: ChangeImage(NPC_IMAGE_OF_RAZUVIOUS, MODEL_IMAGE_OF_RAZUVIOUS, SAY_RAZUVIOUS); break;
                                    case 1: ChangeImage(NPC_IMAGE_OF_GOTHIK, MODEL_IMAGE_OF_GOTHIK, SAY_GOTHIK); break;
                                    case 2: ChangeImage(NPC_IMAGE_OF_THANE, MODEL_IMAGE_OF_THANE, SAY_DEATH_KNIGHTS_1);
                                            _events.ScheduleEvent(EVENT_DEATH_KNIGHTS_2, 10000); break;
                                }
                            }
                            break;
                        case 2: // Blighted abomination wing of Naxxramas
                            {
                                switch (urand (0, 3))
                                {
                                    case 0: ChangeImage(NPC_IMAGE_OF_PATCHWERK, MODEL_IMAGE_OF_PATCHWERK, SAY_PATCHWERK); break;
                                    case 1: ChangeImage(NPC_IMAGE_OF_GROBBULUS, MODEL_IMAGE_OF_GROBBULUS, SAY_GROBBULUS); break;
                                    case 2: ChangeImage(NPC_IMAGE_OF_THADDIUS, MODEL_IMAGE_OF_THADDIUS, SAY_THADDIUS); break;
                                    case 3: ChangeImage(NPC_IMAGE_OF_GLUTH, MODEL_IMAGE_OF_GLUTH, SAY_GLUTH); break;
                                }
                            }
                            break;
                        case 3: // Accursed spider wing of Naxxramas
                            {
                                switch (urand (0, 2))
                                {
                                    case 0: ChangeImage(NPC_IMAGE_OF_ANUBREKHAN, MODEL_IMAGE_OF_ANUBREKHAN, SAY_ANUBREKHAN); break;
                                    case 1: ChangeImage(NPC_IMAGE_OF_FAERLINA, MODEL_IMAGE_OF_FAERLINA, SAY_FAERLINA); break;
                                    case 2: ChangeImage(NPC_IMAGE_OF_MAEXXNA, MODEL_IMAGE_OF_MAEXXNA, SAY_MAEXXNA); break;
                                }
                            }
                            break;
                        case 4: // Dread plague wing of Naxxramas
                            {
                                switch (urand (0, 2))
                                {
                                    case 0: ChangeImage(NPC_IMAGE_OF_NOTH, MODEL_IMAGE_OF_NOTH, SAY_NOTH); break;
                                    case 1: ChangeImage(NPC_IMAGE_OF_HEIGAN, MODEL_IMAGE_OF_HEIGAN, SAY_HEIGAN_1);
                                            _events.ScheduleEvent(EVENT_HEIGAN_2, 8000); break;
                                    case 2: ChangeImage(NPC_IMAGE_OF_LOATHEB, MODEL_IMAGE_OF_LOATHEB, SAY_LOATHEB); break;
                                }
                            }
                            break;
                        case 5: // Home
                            _events.ScheduleEvent(EVENT_START_RANDOM, 30000);
                            break;
                        }
                    }
                }
            }

            void StoreTargets()
            {
                uint8 creaturecount;

                creaturecount = 0;

                for (uint8 ii = 0; ii < 3; ++ii)
                {
                    std::list<Creature*> creatureList;
                    GetCreatureListWithEntryInGrid(creatureList, me, AudienceMobs[ii], 15.0f);
                    for (std::list<Creature*>::iterator itr = creatureList.begin(); itr != creatureList.end(); ++itr)
                    {
                        if (Creature* creatureList = *itr)
                        {
                            audienceList[creaturecount] = creatureList->GetGUID();
                            ++creaturecount;
                        }
                    }
                }

                if (Creature* creature = me->FindNearestCreature(NPC_IMAGE_OF_KELTHUZAD, 20.0f, true))
                    imageList[0] = creature->GetGUID();
                if (Creature* creature = me->FindNearestCreature(NPC_IMAGE_OF_RAZUVIOUS, 20.0f, true))
                    imageList[1] = creature->GetGUID();
                if (Creature* creature = me->FindNearestCreature(NPC_IMAGE_OF_PATCHWERK, 20.0f, true))
                    imageList[2] = creature->GetGUID();
                if (Creature* creature = me->FindNearestCreature(NPC_IMAGE_OF_ANUBREKHAN, 20.0f, true))
                    imageList[3] = creature->GetGUID();
                if (Creature* creature = me->FindNearestCreature(NPC_IMAGE_OF_NOTH, 20.0f, true))
                    imageList[4] = creature->GetGUID();
            }

            void ChangeImage(uint32 entry, uint32 model, uint8 text)
            {
                if (Creature* creature = ObjectAccessor::GetCreature(*me, imageList[talkWing]))
                {
                    Talk(text);
                    creature->SetEntry(entry);
                    creature->SetDisplayId(model);
                    creature->CastSpell(creature, SPELL_HEROIC_IMAGE_CHANNEL);
                    _events.ScheduleEvent(EVENT_TALK_COMPLETE, 40000);
                }
            }

            void TurnAudience()
            {
                for (uint8 i = 0; i < 10; ++i)
                {
                    if (Creature* creature = ObjectAccessor::GetCreature(*me, audienceList[i]))
                        creature->SetFacingToObject(me);
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
               _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_START_RANDOM:
                            talkWing = urand (0, 4);
                            Talk(talkWing);
                            _events.ScheduleEvent(EVENT_MOVE_TO_POINT, 8000);
                            break;
                        case EVENT_MOVE_TO_POINT:
                            me->SetWalk(true);
                            me->GetMotionMaster()->Clear();
                            me->GetMotionMaster()->MovePoint(1, PosTalkLocations[talkWing].m_positionX, PosTalkLocations[talkWing].m_positionY, PosTalkLocations[talkWing].m_positionZ);
                            break;
                        case EVENT_TALK_COMPLETE:
                            talkWing = 5;
                            Talk(talkWing);
                            _events.ScheduleEvent(EVENT_MOVE_TO_POINT, 5000);
                            break;
                        case EVENT_GET_TARGETS:
                            StoreTargets();
                            break;
                        case EVENT_KELTHUZAD_2:
                            Talk(SAY_KELTHUZAD_2);
                            _events.ScheduleEvent(EVENT_KELTHUZAD_3, 8000);
                            break;
                        case EVENT_KELTHUZAD_3:
                            Talk(SAY_KELTHUZAD_3);
                            break;
                        case EVENT_DEATH_KNIGHTS_2:
                            Talk(SAY_DEATH_KNIGHTS_2);
                            if (Creature* creature = ObjectAccessor::GetCreature(*me, imageList[talkWing]))
                            {
                                creature->SetEntry(NPC_IMAGE_OF_BLAUMEUX);
                                creature->SetDisplayId(MODEL_IMAGE_OF_BLAUMEUX);
                            }
                            _events.ScheduleEvent(EVENT_DEATH_KNIGHTS_3, 10000);
                            break;
                        case EVENT_DEATH_KNIGHTS_3:
                            Talk(SAY_DEATH_KNIGHTS_3);
                            if (Creature* creature = ObjectAccessor::GetCreature(*me, imageList[talkWing]))
                            {
                                creature->SetEntry(NPC_IMAGE_OF_ZELIEK);
                                creature->SetDisplayId(MODEL_IMAGE_OF_ZELIEK);
                            }
                            _events.ScheduleEvent(EVENT_DEATH_KNIGHTS_4, 10000);
                            break;
                        case EVENT_DEATH_KNIGHTS_4:
                            Talk(SAY_DEATH_KNIGHTS_4);
                            break;
                        case EVENT_HEIGAN_2:
                            Talk(SAY_HEIGAN_2);
                            break;
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
            private:
                EventMap _events;
                uint64   audienceList[10];
                uint64   imageList[5];
                uint8    talkWing;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_commander_eligor_dawnbringerAI(creature);
        }
};

enum AlexstraszaWrGate
{
    // Quest
    QUEST_RETURN_TO_AG_A    = 12499,
    QUEST_RETURN_TO_AG_H    = 12500,

    // Movie
    MOVIE_ID_GATES          = 14
};

#define GOSSIP_ITEM_WHAT_HAPPENED   "Alexstrasza, can you show me what happened here?"

class npc_alexstrasza_wr_gate : public CreatureScript
{
public:
    npc_alexstrasza_wr_gate() : CreatureScript("npc_alexstrasza_wr_gate") { }

    bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (player->GetQuestRewardStatus(QUEST_RETURN_TO_AG_A) || player->GetQuestRewardStatus(QUEST_RETURN_TO_AG_H))
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_WHAT_HAPPENED, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*sender*/, uint32 action) OVERRIDE
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == GOSSIP_ACTION_INFO_DEF+1)
        {
            player->CLOSE_GOSSIP_MENU();
            player->SendMovieStart(MOVIE_ID_GATES);
        }

        return true;
    }
};

/*######
## vehicle_forsaken_blight_spreader
######*/

#define AREA_VENOMSPITE         4186
#define AREA_VENEGANCE_PASS     4232
#define AREA_CARRION_FIELDS     4188
#define AREA_DRAGONBLIGHT       65

class vehicle_forsaken_blight_spreader : public CreatureScript
{
public:
    vehicle_forsaken_blight_spreader() : CreatureScript("vehicle_forsaken_blight_spreader") { }

    CreatureAI* GetAI(Creature *_Creature) const
    {
        return new  vehicle_forsaken_blight_spreaderAI(_Creature);
    }

    struct vehicle_forsaken_blight_spreaderAI : public VehicleAI
    {
        vehicle_forsaken_blight_spreaderAI(Creature *c) : VehicleAI(c) { }

        uint32 check_Timer;
        bool isInUse;

        void Reset()
        {
            check_Timer = 5000;
        }

        void OnCharmed(bool apply)
        {
            isInUse = apply;

            if(!apply)
                check_Timer = 30000;
        }

        void UpdateAI(const uint32 diff)
        {
            if(!me->IsVehicle())
                return;

            if(isInUse)
            {
                if(check_Timer < diff)
                {
                    uint32 area = me->GetAreaId();
                    switch(area)
                    {
                    case AREA_VENOMSPITE:
                    case AREA_VENEGANCE_PASS:
                    case AREA_CARRION_FIELDS:
                    case AREA_DRAGONBLIGHT:
                        break;
                    default: 
                        me->DealDamage(me,me->GetHealth());
                        break;
                    }

                    check_Timer = 5000;
                }else check_Timer -= diff;
           }else
            {
                if(check_Timer < diff)
                {
                    uint32 area = me->GetAreaId();
                    if(area != AREA_VENOMSPITE)
                    {
                        me->DealDamage(me,me->GetHealth());
                    }
                    check_Timer = 5000;
                }else check_Timer -= diff;
            }
        }

    };

};

/*######
## npc_warsong_battle_standard
######*/

#define SPELL_SUMMON_ANUBAR_INVADER         47303
#define ENTRY_ANUBAR_INVADER                26676
#define ENTRY_QUEST_MIGHT_OF_HORDE          12053

class npc_warsong_battle_standard : public CreatureScript
{
public:
    npc_warsong_battle_standard() : CreatureScript("npc_warsong_battle_standard") { }

    CreatureAI* GetAI(Creature *_Creature) const
    {
        return new  npc_warsong_battle_standardAI(_Creature);
    }

    struct npc_warsong_battle_standardAI : public ScriptedAI
    {
        npc_warsong_battle_standardAI(Creature *c) : ScriptedAI(c) 
        {
            part = 1;
            part_Timer = 5000;
        }

        uint32 part_Timer;
        uint32 part;

        void Reset()
        {
            me->SetReactState(REACT_PASSIVE);
        }

        void UpdateAI(const uint32 diff)
        {
            if(part > 4 || me->isDead())
                return;

            if(part_Timer <= diff)
            {
                switch(part)
                {
                case 1:
                    DoCast(me,SPELL_SUMMON_ANUBAR_INVADER,true);
                    part++;
                    break;
                case 2:
                    DoCast(me,SPELL_SUMMON_ANUBAR_INVADER,true);
                    DoCast(me,SPELL_SUMMON_ANUBAR_INVADER,true);
                    part++;
                    break;
                case 3:
                    DoCast(me,SPELL_SUMMON_ANUBAR_INVADER,true);
                    DoCast(me,SPELL_SUMMON_ANUBAR_INVADER,true);
                    DoCast(me,SPELL_SUMMON_ANUBAR_INVADER,true);
                    part++;
                    break;
                case 4:
                    Unit* owner = me->GetCharmerOrOwnerOrSelf();
                    if(owner && owner->GetTypeId() == TYPEID_PLAYER)
                        owner->ToPlayer()->CompleteQuest(ENTRY_QUEST_MIGHT_OF_HORDE);
                    part++;
                    break;
                }
                part_Timer = 58000;
            }else part_Timer -= diff;
        }
    };

};

/*######
## npc_emissary_brighthoof
######*/

#define QUEST_BLOOD_OATH_HORDE                  11983

class npc_emissary_brighthoof : public CreatureScript
{
public:
    npc_emissary_brighthoof() : CreatureScript("npc_emissary_brighthoof") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pCreature->IsQuestGiver())
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());

        if (pPlayer->GetQuestRewardStatus(QUEST_BLOOD_OATH_HORDE) && pPlayer->getQuestStatusMap()[QUEST_BLOOD_OATH_HORDE].CreatureOrGOCount[0] >= 5)
            pPlayer->CompleteQuest(QUEST_BLOOD_OATH_HORDE);

        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
        return true;
    }

};

/*######
## npc_wintergarde_mine_bomb
######*/

#define SPELL_WMB_EXPLOSION                 48742

class npc_wintergarde_mine_bomb : public CreatureScript
{
public:
    npc_wintergarde_mine_bomb() : CreatureScript("npc_wintergarde_mine_bomb") { }

    CreatureAI* GetAI(Creature *_Creature) const
    {
        return new  npc_wintergarde_mine_bombAI(_Creature);
    }


    struct npc_wintergarde_mine_bombAI : public ScriptedAI
    {
        npc_wintergarde_mine_bombAI(Creature *c) : ScriptedAI(c) 
        {
            Explote_Timer = 13000;
        }

        uint32 Explote_Timer;

        void Reset()
        {
            me->SetReactState(REACT_PASSIVE);
        }

        void UpdateAI(const uint32 diff)
        {
            if(Explote_Timer <= diff)
            {
                Unit* owner = me->GetCharmerOrOwnerOrSelf();

                Creature* target = me->FindNearestCreature(27436,10,true);
                if(target && owner->ToPlayer())
                    owner->ToPlayer()->KilledMonsterCredit(27436,target->GetGUID());

                target = me->FindNearestCreature(27437,10,true);
                if(target && owner->ToPlayer())
                    owner->ToPlayer()->KilledMonsterCredit(27437,target->GetGUID());

                DoCast(SPELL_WMB_EXPLOSION);
                Explote_Timer = 9999999;
            }else Explote_Timer -= diff;
        }

    };

};

/*######
## npc_devout_bodyguard
######*/

float DevoutBodyguardWay[1][21][3] =
{
    {
        {2790.324f, -490.081f, 119.616f},
        {2792.576f, -483.441f, 119.616f},
        {2801.183f, -478.036f, 119.616f},
        {2814.852f, -465.822f, 119.613f},
        {2818.68f, -469.324f, 119.614f},
        {2821.951f, -471.869f, 123.61f},
        {2828.52f, -472.507f, 129.532f},
        {2834.976f, -469.976f, 135.162f},
        {2840.456f, -460.922f, 135.362f},
        {2838.088f, -453.457f, 135.362f},
        {2831.782f, -448.045f, 135.362f},
        {2823.282f, -447.724f, 135.362f},
        {2816.99f, -452.967f, 135.362f},
        {2814.536f, -460.418f, 135.362f},
        {2816.803f, -468.241f, 135.361f},
        {2818.536f, -469.547f, 135.355f},
        {2823.803f, -472.822f, 141.208f},
        {2831.668f, -471.995f, 148.345f},
        {2834.233f, -470.502f, 150.673f},
        {2837.822f, -467.322f, 150.836f},
        {2832.710f, -462.756f, 150.835f}
    }
};

class npc_devout_bodyguard : public CreatureScript
{
public:
    npc_devout_bodyguard() : CreatureScript("npc_devout_bodyguard") { }

    CreatureAI* GetAI(Creature *_Creature) const
    {
        return new npc_devout_bodyguardAI(_Creature);
    }

    struct npc_devout_bodyguardAI: public ScriptedAI
    {
            npc_devout_bodyguardAI(Creature *c) : ScriptedAI(c) {}

            uint32 waypoint;
            uint32 WaitTimer;
            bool isMoving;

            void Reset()
            {
                me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
                isMoving = false;
                waypoint = 0;
                WaitTimer = 1;
            }

            void StartMove()
            {
                me->AddUnitMovementFlag(MOVEMENTFLAG_WALKING);
                WaitTimer = 1;
                isMoving = true;
            }

            void MovementInform(uint32, uint32)
            {
                if (waypoint == 20)
                {
                    isMoving = false;
                    me->DisappearAndDie();
                }
                WaitTimer = 1;
            }

            void UpdateAI(const uint32 diff)
            {

                if (WaitTimer == 1)
                {
                    me->GetMotionMaster()->Clear();
                    if (isMoving)
                        me->GetMotionMaster()->MovePoint(0, DevoutBodyguardWay[0][waypoint][0], DevoutBodyguardWay[0][waypoint][1], DevoutBodyguardWay[0][waypoint][2]);
                    ++waypoint;
                    WaitTimer = 0;
                }

                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }

            void EnterCombat(Unit* /*who*/) {}
    };

};

/*######
## npc_high_abbot_landgren
######*/


enum eHighAbbotLandgren
{
    QUEST_A_FALL_FROM_GRACE                     = 12274,

    SPELL_SCARLET_RAVEN_PRIEST_IMAGE_FEMALE     = 48761,
    SPELL_SCARLET_RAVEN_PRIEST_IMAGE_MALE       = 48763,

    TEXT_1                                      = 12848,
    TEXT_2                                      = 12849,

    NPC_HIGH_ABBOT_LANDGREN_ENTRY               = 27245,
    NPC_HIGH_ABBOT_LANDGREN_ESCORTEE_ENTRY      = 27439,
    NPC_DEVOUT_BODYGUARD                        = 27247
};

float HighAbbotLandgrenSpawnPos[1][1][4] = {{ 2828.981934f, -439.187012f, 119.624001f, 1.647600f }};
float HighAbbotLandgrenJumpPos[1][1][3] = {{ 2719.657f, -554.755f, 23.988f }};

static const char* HighAbbotText[] =
{
        "I am ready, your grace. <kiss the ring>",
        "I know a place nearby where we could speak in private, my child. Follow me.",
        "Did you think that i could not see through your flimsy disguise, $N.",
        "There is much that you do not understand, $R. The Master sees all.",
        "He told me that you would come for me. I won't die by your hand, through. I have seen what you have done to my compatriots.",
        "No i will leave this world in a manner of my own choosing. And i will return, the grand admiral's permitting.",
        "AAAEEEEIIIiiiiiiiiiiiiiiiiiiiiiiiiiiii........................................"
};

class npc_high_abbot_landgren : public CreatureScript
{
public:
    npc_high_abbot_landgren() : CreatureScript("npc_high_abbot_landgren") { }

    struct npc_high_abbot_landgrenAI : public npc_escortAI
    {
        npc_high_abbot_landgrenAI(Creature* pCreature) : npc_escortAI(pCreature) { }

        bool BodyGuardStart;
        bool CheckPlayerDist;
        bool EventStarted;
        bool EndSequence;
        uint8 GuardCount;
        uint8 EndSayCount;
        uint32 AuraCheckTimer;
        uint32 BodyGuardMoveTimer;
        uint32 EndSequenceTimer;
        float Range;
        uint64 PlayerGUID;

        void WaypointReached(uint32 i)
        {
            if (me->GetEntry() != NPC_HIGH_ABBOT_LANDGREN_ESCORTEE_ENTRY)
                return;

            if(PlayerGUID)
                if(Player* pPlayer = Unit::GetPlayer(*me, PlayerGUID))
                {
                    switch (i)
                    {
                        case 0:
                            me->MonsterSay(HighAbbotText[1], LANG_UNIVERSAL, NULL);
                            break;
                        case 12:
                            EndSequence = true;
                            break;
                    }
                }
        }

        void Reset()
        {
            BodyGuardStart = true;
            CheckPlayerDist = false;
            EventStarted = false;
            EndSequence = false;
            GuardCount = 0;
            EndSayCount = 2;
            AuraCheckTimer = 300;
            BodyGuardMoveTimer = 2000;
            EndSequenceTimer = 500;
            Range = 3.0f;
            PlayerGUID = 0;
        }

        void StartEvent()
        {
            CheckPlayerDist = true;
        }

        void StartMove()
        {
            if ( Creature* tmp = me->SummonCreature(NPC_HIGH_ABBOT_LANDGREN_ESCORTEE_ENTRY, HighAbbotLandgrenSpawnPos[0][0][0], HighAbbotLandgrenSpawnPos[0][0][1], HighAbbotLandgrenSpawnPos[0][0][2], HighAbbotLandgrenSpawnPos[0][0][3]))
            {
                CAST_AI(npc_high_abbot_landgren::npc_high_abbot_landgrenAI, tmp->AI())->PlayerGUID = PlayerGUID;
                me->DespawnOrUnsummon();
            }
        }

        void UpdateAI(const uint32 diff)
        {
            npc_escortAI::UpdateAI(diff);

            if (PlayerGUID)
            {
                Player* AuraPlayer = Unit::GetPlayer(*me, PlayerGUID);

                if (!AuraPlayer)
                {
                    me->DisappearAndDie();
                    return;
                }
                else
                {
                    if (AuraPlayer->isDead())
                    {
                        AuraPlayer->FailQuest(QUEST_A_FALL_FROM_GRACE);
                        me->DisappearAndDie();
                        return;
                    }

                    if (EndSequence)
                    {
                        if (EndSequenceTimer <= diff)
                        {
                            EndSequenceTimer = 12000;
                            if (EndSayCount == 6)
                            {
                                me->GetMotionMaster()->MoveJump(HighAbbotLandgrenJumpPos[0][0][0], HighAbbotLandgrenJumpPos[0][0][1], HighAbbotLandgrenJumpPos[0][0][2], 0.5f, 8.0f);
                                EndSequenceTimer = 2000;
                            }
                            if (EndSayCount == 7)
                            {
                                me->MonsterSay(HighAbbotText[6], LANG_UNIVERSAL, NULL);
                                EndSequenceTimer = 2000;
                            }
                            if (EndSayCount == 8)
                            {
                                AuraPlayer->KilledMonsterCredit(27444, me->GetGUID());
                                if (AuraPlayer->HasAura(SPELL_SCARLET_RAVEN_PRIEST_IMAGE_FEMALE))
                                    AuraPlayer->RemoveAura(SPELL_SCARLET_RAVEN_PRIEST_IMAGE_FEMALE);
                                if (AuraPlayer->HasAura(SPELL_SCARLET_RAVEN_PRIEST_IMAGE_MALE))
                                    AuraPlayer->RemoveAura(SPELL_SCARLET_RAVEN_PRIEST_IMAGE_MALE);
                                EndSequence = false;
                            }

                            if (EndSayCount < 6)
                                me->MonsterSay(HighAbbotText[EndSayCount], LANG_UNIVERSAL, AuraPlayer);

                            EndSayCount++;
                        } else
                            EndSequenceTimer -= diff;
                    }

                    if (!EventStarted && me->GetEntry() == NPC_HIGH_ABBOT_LANDGREN_ESCORTEE_ENTRY)
                    {
                        Start(false, false, PlayerGUID, 0, false);
                        EventStarted = true;
                    }

                    if (CheckPlayerDist)
                        if (AuraPlayer->GetDistance(2827.796f, -420.191f, 118.196f) < 4)
                            StartMove();

                    if (AuraCheckTimer <= diff) {
                        if (AuraPlayer && AuraPlayer->GetQuestStatus(QUEST_A_FALL_FROM_GRACE) == QUEST_STATUS_INCOMPLETE && AuraPlayer->getQuestStatusMap()[QUEST_A_FALL_FROM_GRACE].CreatureOrGOCount[0] == 1 && !AuraPlayer->HasAura(SPELL_SCARLET_RAVEN_PRIEST_IMAGE_MALE) && !AuraPlayer->HasAura(SPELL_SCARLET_RAVEN_PRIEST_IMAGE_FEMALE)) {
                            switch(AuraPlayer->getGender())
                            {
                            case GENDER_FEMALE:
                                AuraPlayer->CastSpell(AuraPlayer, SPELL_SCARLET_RAVEN_PRIEST_IMAGE_FEMALE, false);
                                break;
                            case GENDER_MALE:
                                AuraPlayer->CastSpell(AuraPlayer, SPELL_SCARLET_RAVEN_PRIEST_IMAGE_MALE, false);
                                break;
                            }
                        }
                        AuraCheckTimer = 300;
                    } else
                        AuraCheckTimer -= diff;

                    if (me->GetEntry() != NPC_HIGH_ABBOT_LANDGREN_ESCORTEE_ENTRY)
                    {
                        if (BodyGuardStart && AuraPlayer->GetQuestStatus(QUEST_A_FALL_FROM_GRACE) == QUEST_STATUS_INCOMPLETE && AuraPlayer->getQuestStatusMap()[QUEST_A_FALL_FROM_GRACE].CreatureOrGOCount[0] == 1) {
                            if (Creature* tmp = me->FindNearestCreature(NPC_DEVOUT_BODYGUARD, Range, true))
                            {
                                if (BodyGuardMoveTimer <= diff)
                                {
                                    CAST_AI(npc_devout_bodyguard::npc_devout_bodyguardAI, tmp->AI())->StartMove();
                                    BodyGuardMoveTimer = 6000;
                                    Range = 4.0f;
                                    if (GuardCount == 1)
                                        BodyGuardStart = false;
                                    GuardCount++;
                                } else
                                    BodyGuardMoveTimer -= diff;
                            }
                        }
                    }
                }
            }
        }
    };

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        switch (uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF + 1:
                pPlayer->SEND_GOSSIP_MENU(TEXT_2, pCreature->GetGUID());
                CAST_AI(npc_high_abbot_landgren::npc_high_abbot_landgrenAI, pCreature->AI())->StartEvent();
                break;
        }
        return true;
    }

    bool OnGossipHello(Player *player, Creature *pCreature)
    {
        if (pCreature->IsQuestGiver())
            player->PrepareQuestMenu(pCreature->GetGUID());

        if (pCreature->GetEntry() == NPC_HIGH_ABBOT_LANDGREN_ENTRY && player->GetQuestStatus(QUEST_A_FALL_FROM_GRACE) == QUEST_STATUS_INCOMPLETE && player->getQuestStatusMap()[QUEST_A_FALL_FROM_GRACE].CreatureOrGOCount[0] == 1 && (player->HasAura(SPELL_SCARLET_RAVEN_PRIEST_IMAGE_MALE) || player->HasAura(SPELL_SCARLET_RAVEN_PRIEST_IMAGE_FEMALE)))
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, HighAbbotText[0], GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            player->SEND_GOSSIP_MENU(TEXT_1, pCreature->GetGUID());
            return true;
        }

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(pCreature), pCreature->GetGUID());
        return true;
    }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_high_abbot_landgrenAI(pCreature);
    }

};

/*######
## npc_7th_legion_siege_engineer
######*/

#define ENTRY_ENGENEER                                  27163
#define ENTRY_SCOURGE_PLAGE_CATAPULT                    27607
#define SPELL_PLACE_SCOURGE_DISCOMBOBULATER             49114 // Summons Object for 15 secounds
#define SPELL_DESTURCTION                               49215
#define SPELL_DESTURCTION_TRIGGER                       49218
#define ENTRY_PLAGUE_PULT_CREDIT                        27625

class npc_7th_legion_siege_engineer : public CreatureScript
{
public:
    npc_7th_legion_siege_engineer() : CreatureScript("npc_7th_legion_siege_engineer") { }

    struct npc_7th_legion_siege_engineerAI : public ScriptedAI
    {
        npc_7th_legion_siege_engineerAI(Creature* pCreature) : ScriptedAI (pCreature) { }

        uint64 guid_owner;
        uint64 guid_pult;

        uint32 check_Timer;
        uint32 phase;

        void Reset()
        {
            guid_owner = 0;
            guid_pult = 0;
            phase = 0; //0 - Check for PlageCatapult, 1 - MoveToPult, 2 - Summon Object, 3 - Give Credit
            check_Timer = 2000;
        }

        void SpellHitTarget(Unit *target, const SpellInfo *spell)
        {
            if(spell->Id == SPELL_DESTURCTION_TRIGGER)
            {
                if(Player *owner = Unit::GetPlayer(*me,guid_owner))
                    owner->KilledMonsterCredit(ENTRY_PLAGUE_PULT_CREDIT,target->GetGUID());
                if(target->ToCreature())
                {
                    target->DealDamage(target,target->GetHealth());
                    //target->ToCreature()->RemoveCorpse();
                }
            }
        }

        void MoveInLineOfSight(Unit *who)
        {
            if(guid_owner > 0)
                return;

            if(me->GetDistance2d(who) > 20)
                return;

            if(who->GetTypeId() == TYPEID_UNIT)
            {
                if(who->IsVehicle() && who->GetCharmer() && who->GetCharmer()->ToPlayer())
                    guid_owner = who->GetCharmer()->ToPlayer()->GetGUID();
            }

            if(who->GetTypeId() == TYPEID_PLAYER)
            {
                guid_owner = who->ToPlayer()->GetGUID();
            }
        }

        void AttackStart(Unit *attacker){}

        void MovementInform(uint32 type, uint32 id)
        {
            if(type != POINT_MOTION_TYPE)
                return;

            if(id != 1)
                return;

            phase = 2;
            check_Timer = 2000;
        }

        void UpdateAI(const uint32 diff)
        {
            if(check_Timer <= diff)
            {
                switch(phase)
                {
                case 0:
                    if(Unit::GetPlayer(*me,guid_owner))
                    {
                        Creature* pult = me->FindNearestCreature(ENTRY_SCOURGE_PLAGE_CATAPULT,50);
                        if(pult)
                        {
                            guid_pult = pult->GetGUID();
                            me->GetMotionMaster()->MovePoint(1,pult->GetPositionX(),pult->GetPositionY(),pult->GetPositionZ());
                            phase = 1;
                        }
                    }
                    else me->DealDamage(me,me->GetHealth());
                    break;
                case 1:
                    return;
                case 2:
                    if(Creature* pult = Creature::GetCreature(*me,guid_pult))
                    {
                        me->CastSpell(pult,SPELL_DESTURCTION,true);
                        me->CastSpell(me,SPELL_PLACE_SCOURGE_DISCOMBOBULATER,false);
                        phase++;
                    }
                    break;
                }

            }else check_Timer -= diff;

        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_7th_legion_siege_engineerAI(pCreature);
    }

};

/*######
## vehicle_alliance_steamtank
######*/

#define AREA_CARRION_FIELDS         4188
#define AREA_WINTERGARD_MAUSOLEUM   4246
#define AREA_THORSONS_POINT         4190

class vehicle_alliance_steamtank : public CreatureScript
{
public:
    vehicle_alliance_steamtank() : CreatureScript("vehicle_alliance_steamtank") { }

    struct vehicle_alliance_steamtankAI : public VehicleAI
    {
        vehicle_alliance_steamtankAI(Creature *c) : VehicleAI(c) { }

        uint32 check_Timer;
        bool isInUse;

        void Reset()
        {
            check_Timer = 5000;
        }

        void OnCharmed(bool apply)
        {
            isInUse = apply;

            if(!apply)
                check_Timer = 30000;
        }

        void UpdateAI(const uint32 diff)
        {
            if(!me->IsVehicle())
                return;

            if(isInUse)
            {
                if(check_Timer < diff)
                {
                    uint32 area = me->GetAreaId();
                    switch(area)
                    {
                    case AREA_CARRION_FIELDS:
                    case AREA_WINTERGARD_MAUSOLEUM:
                    case AREA_THORSONS_POINT:
                        break;
                    default: 
                        me->DealDamage(me,me->GetHealth());
                        break;
                    }

                    check_Timer = 5000;
                }else check_Timer -= diff;
            }else
            {
                if(check_Timer < diff)
                {
                    uint32 area = me->GetAreaId();
                    if(area != AREA_THORSONS_POINT)
                    {
                        me->DealDamage(me,me->GetHealth());
                    }
                    check_Timer = 5000;
                }else check_Timer -= diff;
            }
        }

    };

    CreatureAI* GetAI(Creature *_Creature) const
    {
        return new  vehicle_alliance_steamtankAI(_Creature);
    }

};

/*######
## npc_agent_skully
######*/

const char* AgentSkullySay[] =
{
        "Please wait $N, someone else is doing the Task! Come back later and try again!"
};

class npc_agent_skully : public CreatureScript
{
public:
    npc_agent_skully() : CreatureScript("npc_agent_skully") { }

    bool OnQuestAccept(Player* pPlayer, Creature* pCreature, Quest const* quest)
    {
        if (quest->GetQuestId() == QUEST_A_FALL_FROM_GRACE)
        {
            if (pPlayer->getGender() == GENDER_FEMALE)
                pCreature->CastSpell(pPlayer, SPELL_SCARLET_RAVEN_PRIEST_IMAGE_FEMALE, false);
            if (pPlayer->getGender() == GENDER_MALE)
                pCreature->CastSpell(pPlayer, SPELL_SCARLET_RAVEN_PRIEST_IMAGE_MALE, false);

            if (Creature *cC = pCreature->FindNearestCreature(NPC_HIGH_ABBOT_LANDGREN_ENTRY, 30))
            {
                if (!CAST_AI(npc_high_abbot_landgren::npc_high_abbot_landgrenAI, cC->AI())->PlayerGUID)
                    CAST_AI(npc_high_abbot_landgren::npc_high_abbot_landgrenAI, cC->AI())->PlayerGUID = pPlayer->GetGUID();
                else
                {
                    if (Player* uPlayer = Unit::GetPlayer(*pCreature, CAST_AI(npc_high_abbot_landgren::npc_high_abbot_landgrenAI, cC->AI())->PlayerGUID))
                    {
                        if (!uPlayer->IsInWorld() || uPlayer->GetAreaId() != 4180 || uPlayer->GetQuestStatus(QUEST_A_FALL_FROM_GRACE) != QUEST_STATUS_INCOMPLETE)
                            CAST_AI(npc_high_abbot_landgren::npc_high_abbot_landgrenAI, cC->AI())->PlayerGUID = pPlayer->GetGUID();
                        else
                        {
                            pCreature->MonsterSay(AgentSkullySay[0], LANG_UNIVERSAL, pPlayer);
                            pPlayer->FailQuest(QUEST_A_FALL_FROM_GRACE);
                        }
                    }
                }
            }
            else
            {
                pCreature->MonsterSay(AgentSkullySay[0], LANG_UNIVERSAL, pPlayer);
                pPlayer->FailQuest(QUEST_A_FALL_FROM_GRACE);
            }
        }
        return true;
    }

};

/*######
## Quest Strengthen the Ancients (12096|12092)
######*/

enum StrengthenAncientsMisc
{
    SAY_WALKER_FRIENDLY         = 0,
    SAY_WALKER_ENEMY            = 1,
    SAY_LOTHALOR                = 0,

    SPELL_CREATE_ITEM_BARK      = 47550,
    SPELL_CONFUSED              = 47044,

    NPC_LOTHALOR                = 26321,

    FACTION_WALKER_ENEMY        = 14,
};

class spell_q12096_q12092_dummy : public SpellScriptLoader // Strengthen the Ancients: On Interact Dummy to Woodlands Walker
{
public:
    spell_q12096_q12092_dummy() : SpellScriptLoader("spell_q12096_q12092_dummy") { }

    class spell_q12096_q12092_dummy_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_q12096_q12092_dummy_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            uint32 roll = rand() % 2;

            Creature* tree = GetHitCreature();
            Player* player = GetCaster()->ToPlayer();

            if (!tree || !player)
                return;

            tree->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);

            if (roll == 1) // friendly version
            {
                tree->CastSpell(player, SPELL_CREATE_ITEM_BARK);
                tree->AI()->Talk(SAY_WALKER_FRIENDLY, player);
                tree->DespawnOrUnsummon(1000);
            }
            else if (roll == 0) // enemy version
            {
                tree->AI()->Talk(SAY_WALKER_ENEMY, player);
                tree->setFaction(FACTION_WALKER_ENEMY);
                tree->Attack(player, true);
            }
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_q12096_q12092_dummy_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_q12096_q12092_dummy_SpellScript();
    }
};

class spell_q12096_q12092_bark : public SpellScriptLoader // Bark of the Walkers
{
public:
    spell_q12096_q12092_bark() : SpellScriptLoader("spell_q12096_q12092_bark") { }

    class spell_q12096_q12092_bark_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_q12096_q12092_bark_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Creature* lothalor = GetHitCreature();
            if (!lothalor || lothalor->GetEntry() != NPC_LOTHALOR)
                return;

            lothalor->AI()->Talk(SAY_LOTHALOR);
            lothalor->RemoveAura(SPELL_CONFUSED);
            lothalor->DespawnOrUnsummon(4000);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_q12096_q12092_bark_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_q12096_q12092_bark_SpellScript();
    }
};

/*######
## Quest: Defending Wyrmrest Temple ID: 12372
######*/

enum WyrmDefenderEnum
{
    // Quest data
    QUEST_DEFENDING_WYRMREST_TEMPLE          = 12372,
    GOSSIP_TEXTID_DEF1                       = 12899,

    // Gossip data
    GOSSIP_TEXTID_DEF2                       = 12900,

    // Spells data
    SPELL_CHARACTER_SCRIPT                   = 49213,
    SPELL_DEFENDER_ON_LOW_HEALTH_EMOTE       = 52421, // ID - 52421 Wyrmrest Defender: On Low Health Boss Emote to Controller - Random /self/
    SPELL_RENEW                              = 49263, // cast to heal drakes
    SPELL_WYRMREST_DEFENDER_MOUNT            = 49256,

    // Texts data
    WHISPER_MOUNTED                        = 0,
    BOSS_EMOTE_ON_LOW_HEALTH               = 2
};

#define GOSSIP_ITEM_1      "We need to get into the fight. Are you ready?"

class npc_wyrmrest_defender : public CreatureScript
{
    public:
        npc_wyrmrest_defender() : CreatureScript("npc_wyrmrest_defender") { }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            if (player->GetQuestStatus(QUEST_DEFENDING_WYRMREST_TEMPLE) == QUEST_STATUS_INCOMPLETE)
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_DEF1, creature->GetGUID());
            }
            else
                player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();
            if (action == GOSSIP_ACTION_INFO_DEF+1)
            {
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_DEF2, creature->GetGUID());
                // Makes player cast trigger spell for 49207 on self
                player->CastSpell(player, SPELL_CHARACTER_SCRIPT, true);
                // The gossip should not auto close
            }

            return true;
        }

        struct npc_wyrmrest_defenderAI : public VehicleAI
        {
            npc_wyrmrest_defenderAI(Creature* creature) : VehicleAI(creature) { }

            bool hpWarningReady;
            bool renewRecoveryCanCheck;

            uint32 RenewRecoveryChecker;

            void Reset() OVERRIDE
            {
                hpWarningReady = true;
                renewRecoveryCanCheck = false;

                RenewRecoveryChecker = 0;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                // Check system for Health Warning should happen first time whenever get under 30%,
                // after it should be able to happen only after recovery of last renew is fully done (20 sec),
                // next one used won't interfere
                if (hpWarningReady && me->GetHealthPct() <= 30.0f)
                {
                    me->CastSpell(me, SPELL_DEFENDER_ON_LOW_HEALTH_EMOTE);
                    hpWarningReady = false;
                }

                if (renewRecoveryCanCheck)
                {
                    if (RenewRecoveryChecker <= diff)
                    {
                        renewRecoveryCanCheck = false;
                        hpWarningReady = true;
                    }
                    else RenewRecoveryChecker -= diff;
                }
            }

            void SpellHit(Unit* /*caster*/, SpellInfo const* spell) OVERRIDE
            {
                switch (spell->Id)
                {
                    case SPELL_WYRMREST_DEFENDER_MOUNT:
                        Talk(WHISPER_MOUNTED, me->GetCharmerOrOwner());
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);
                        break;
                    // Both below are for checking low hp warning
                    case SPELL_DEFENDER_ON_LOW_HEALTH_EMOTE:
                        Talk(BOSS_EMOTE_ON_LOW_HEALTH, me->GetCharmerOrOwner());
                        break;
                    case SPELL_RENEW:
                        if (!hpWarningReady && RenewRecoveryChecker <= 100)
                        {
                            RenewRecoveryChecker = 20000;
                        }
                        renewRecoveryCanCheck = true;
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_wyrmrest_defenderAI(creature);
        }
};

/*#####
# npc_torturer_lecraft
#####*/

enum TorturerLeCraft
{
    SPELL_HEMORRHAGE                   = 30478,
    SPELL_KIDNEY_SHOT                  = 30621,
    SPELL_HIGH_EXECUTORS_BRANDING_IRON = 48603,
    NPC_TORTURER_LECRAFT               = 27394,
    EVENT_HEMORRHAGE                   = 1,
    EVENT_KIDNEY_SHOT                  = 2,
    SAY_AGGRO                          = 0
};

class npc_torturer_lecraft : public CreatureScript
{
    public: npc_torturer_lecraft() : CreatureScript("npc_torturer_lecraft") {}

        struct npc_torturer_lecraftAI : public ScriptedAI
        {
            npc_torturer_lecraftAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                _textCounter = 1;
                _playerGUID  = 0;
            }

            void EnterCombat(Unit* who) OVERRIDE
            {
                _events.ScheduleEvent(EVENT_HEMORRHAGE, urand(5000, 8000));
                _events.ScheduleEvent(EVENT_KIDNEY_SHOT, urand(12000, 15000));

                if (Player* player = who->ToPlayer())
                    Talk (SAY_AGGRO, player);
            }

            void SpellHit(Unit* caster, const SpellInfo* spell) OVERRIDE
            {
                if (spell->Id != SPELL_HIGH_EXECUTORS_BRANDING_IRON)
                    return;

                if (Player* player = caster->ToPlayer())
                {
                    if (_textCounter == 1)
                        _playerGUID = player->GetGUID();

                    if (_playerGUID != player->GetGUID())
                        return;

                    Talk(_textCounter, player);

                    if (_textCounter == 5)
                        player->KilledMonsterCredit(NPC_TORTURER_LECRAFT, 0);

                    ++_textCounter;

                    if (_textCounter == 13)
                        _textCounter = 6;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
               if (!UpdateVictim())
                   return;

               _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_HEMORRHAGE:
                            DoCastVictim(SPELL_HEMORRHAGE);
                            _events.ScheduleEvent(EVENT_HEMORRHAGE, urand(12000, 168000));
                            break;
                        case EVENT_KIDNEY_SHOT:
                            DoCastVictim(SPELL_KIDNEY_SHOT);
                            _events.ScheduleEvent(EVENT_KIDNEY_SHOT, urand(20000, 26000));
                            break;
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
            private:
                EventMap _events;
                uint8    _textCounter;
                uint64   _playerGUID;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_torturer_lecraftAI(creature);
        }
};

void AddSC_dragonblight()
{
    new vehicle_forsaken_blight_spreader;
    new npc_warsong_battle_standard;
    new npc_emissary_brighthoof;
    new npc_wintergarde_mine_bomb;
    new npc_devout_bodyguard;
    new npc_high_abbot_landgren;
    new npc_agent_skully;
    new npc_7th_legion_siege_engineer;
    new vehicle_alliance_steamtank;
    new npc_commander_eligor_dawnbringer();
    new npc_alexstrasza_wr_gate();
    new spell_q12096_q12092_dummy();
    new spell_q12096_q12092_bark();
    new npc_wyrmrest_defender();
    new npc_torturer_lecraft();
}
