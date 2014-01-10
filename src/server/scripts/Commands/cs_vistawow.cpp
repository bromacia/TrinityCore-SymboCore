/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
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

#include "Chat.h"
#include "Language.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "Group.h"
#include "ScriptMgr.h"

class vistawow_commandscript : public CommandScript
{
public:
    vistawow_commandscript() : CommandScript("vistawow_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand vistaCommandTable[] =
        {
            { "checkboost",     SEC_PLAYER,         false, &HandleCheckBoostCommand,           "", NULL },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };

        static ChatCommand commandTable[] =
        {
            { "vistawow",       SEC_PLAYER,         false, NULL,                  "", vistaCommandTable },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        return commandTable;
    }

    static bool HandleCheckBoostCommand(ChatHandler* handler, char const* /*args*/)
    {
        if (Player* player = handler->GetSession()->GetPlayer())
        {
            if (Group* group = player->GetGroup())
            {
                handler->SendSysMessage("Os jogadores de seu grupo estão haptos a receber bônus nas seguintes modalidades:");

                for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
                    if (Player* member = itr->GetSource())
                    {
                        member->UpdateDropBoostRating();
                        uint32 rating = member->GetDropBoostRating();

                        std::ostringstream ss;
                        ss << handler->playerLink(member->GetName());

                        if (rating <= 3000)
                            ss << " |cff00ff005N|r";
                        else
                            ss << " |cffff00005N|r";

                        if (rating <= 3500)
                            ss << " |cff00ff005H|r";
                        else
                            ss << " |cffff00005H|r";

                        if (rating <= 4000)
                            ss << " |cff00ff0010N|r";
                        else
                            ss << " |cffff000010N|r";

                        if (rating <= 4500)
                            ss << " |cff00ff0025N|r";
                        else
                            ss << " |cffff000025N|r";

                        if (rating <= 5000)
                            ss << " |cff00ff0010H|r";
                        else
                            ss << " |cffff000010H|r";

                        if (rating <= 5000)
                            ss << " |cff00ff0025H|r";
                        else
                            ss << " |cffff000025H|r";

                        if (!player->IsAtGroupRewardDistance(member))
                            ss << " -- |cffff0000player ausente|r";
                        handler->SendSysMessage(ss.str().c_str());
                    }
            }
            else
                handler->SendSysMessage("Você não está em um grupo.");
        }
        return true;
    }
};

void AddSC_vistawow_commandscript()
{
    new vistawow_commandscript();
}
