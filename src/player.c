/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file player.c
 *     Player state and information handling.
 * @par Purpose:
 *     Implement functions for getting and updating player state.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 Aug 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "player.h"

#include "game.h"
#include "people.h"
#include "thing.h"
#include "weapon.h"
#include "swlog.h"
/******************************************************************************/

void player_update_from_cryo_agent(ushort cryo_no, PlayerInfo *p_player, ushort plagent)
{
    ushort wepfp;

    p_player->Weapons[plagent] = cryo_agents.Weapons[cryo_no];
    p_player->Mods[cryo_no].Mods = cryo_agents.Mods[cryo_no].Mods;

    for (wepfp = 0; wepfp < WFRPK_COUNT; wepfp++) {
        p_player->FourPacks[wepfp][plagent] = \
          cryo_agents.FourPacks[cryo_no].Amount[wepfp];
    }
}

void cryo_update_from_player_agent(ushort cryo_no, PlayerInfo *p_player, ushort plagent)
{
    struct Thing *p_agent;
    ushort wepfp;

    p_agent = p_player->MyAgent[plagent];

    cryo_agents.Weapons[cryo_no] = p_agent->U.UPerson.WeaponsCarried & ~(1 << (WEP_ENERGYSHLD-1));
    cryo_agents.Mods[cryo_no].Mods = p_agent->U.UPerson.UMod.Mods;

    for (wepfp = 0; wepfp < WFRPK_COUNT; wepfp++) {
        cryo_agents.FourPacks[cryo_no].Amount[wepfp] = \
          p_player->FourPacks[wepfp][plagent];
    }
}

void player_update_agents_from_cryo(PlayerInfo *p_player)
{
    ushort plagent;

    for (plagent = 0; plagent < 4; plagent++) {
        ushort cryo_no = plagent;
        player_update_from_cryo_agent(cryo_no, p_player, plagent);
    }
}

void cryo_update_agents_from_player(PlayerInfo *p_player)
{
    ushort plagent, nremoved;

    nremoved = 0;
    for (plagent = 0; plagent < playable_agents; plagent++)
    {
        struct Thing *p_agent;

        p_agent = p_player->MyAgent[plagent];
        if ((p_agent->Flag & TngF_Unkn0002) != 0) {
            remove_agent(plagent);
            ++nremoved;
            continue;
        }
        if ((p_agent->SubType == SubTT_PERS_AGENT) || (p_agent->SubType == SubTT_PERS_ZEALOT))
        {
            ushort cryo_no;
            // Removing an agent from cryo shifted all further agents down; account for that
            cryo_no = plagent - nremoved;
            cryo_update_from_player_agent(cryo_no, p_player, plagent);
        }
    }
}

void players_sync_from_cryo(void)
{
    PlayerInfo *p_locplayer;

    p_locplayer = &players[local_player_no];
    player_update_agents_from_cryo(p_locplayer);
}

void remove_agent(u8 cryo_no)
{
    asm volatile ("call ASM_remove_agent\n"
        : : "a" (cryo_no));
}

void add_agent(u32 weapons, ushort mods)
{
    asm volatile ("call ASM_add_agent\n"
        : : "a" (weapons), "d" (mods));
}

/******************************************************************************/
