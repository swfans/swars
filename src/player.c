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

#include <assert.h>
#include "bfutility.h"

#include "game.h"
#include "guitext.h"
#include "hud_panel.h"
#include "people.h"
#include "thing.h"
#include "weapon.h"
#include "swlog.h"
/******************************************************************************/

void player_mission_agents_reset(PlayerIdx plyr)
{
    PlayerInfo *p_plyr;

    p_plyr = &players[plyr];
    p_plyr->MissionAgents = 0x0F;
}

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
    if (p_agent->Type != TT_PERSON)
        return;

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
        if (p_agent->Type != TT_PERSON)
            continue;
        if ((p_agent->Flag & TngF_Destroyed) != 0) {
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

void player_agents_add_random_epidermises(PlayerInfo *p_player)
{
    ushort plagent;
    struct Thing *p_person;

    for (plagent = 0; plagent < playable_agents; plagent++)
    {
        p_person = p_player->MyAgent[plagent];
        if (p_person == NULL)
            continue;
        if (cybmod_skin_level(&p_person->U.UPerson.UMod) == 0)
        {
            ushort rnd;
            rnd = LbRandomAnyShort();
            set_cybmod_skin_level(&p_person->U.UPerson.UMod, 1 + (rnd & 3));
        }
    }
}

void players_sync_from_cryo(void)
{
    PlayerInfo *p_locplayer;

    p_locplayer = &players[local_player_no];
    player_update_agents_from_cryo(p_locplayer);
}

void player_agents_init_prev_weapon(PlayerIdx plyr)
{
    PlayerInfo *p_locplayer;
    ushort plagent;

    p_locplayer = &players[local_player_no];
    for (plagent = 0; plagent < playable_agents; plagent++)
    {
        struct Thing *p_agent;
        ushort weptype;

        p_agent = p_locplayer->MyAgent[plagent];
        if (p_agent->Type == TT_PERSON)
            weptype = find_nth_weapon_held(p_agent->ThingOffset, 1);
        else
            weptype = WEP_NULL;
        p_locplayer->PrevWeapon[plagent] = weptype;
    }
    for (; plagent < AGENTS_SQUAD_MAX_COUNT; plagent++)
    {
        p_locplayer->PrevWeapon[plagent] = WEP_NULL;
    }
}

void player_agent_update_prev_weapon(struct Thing *p_agent)
{
    PlayerInfo *p_player;
    PlayerIdx plyr;
    ushort plagent;

    plyr = p_agent->U.UPerson.ComCur >> 2;
    plagent = p_agent->U.UPerson.ComCur & 3;
    p_player = &players[plyr];

    if (p_agent->U.UPerson.CurrentWeapon != 0)
        p_player->PrevWeapon[plagent] = p_agent->U.UPerson.CurrentWeapon;
    else
        p_player->PrevWeapon[plagent] = find_nth_weapon_held(p_agent->ThingOffset, 1);
}

short player_agent_current_or_prev_weapon(PlayerIdx plyr, ushort plagent)
{
    PlayerInfo *p_player;
    struct Thing *p_agent;
    short curwep;

    p_player = &players[plyr];
    p_agent = p_player->MyAgent[plagent];
    if (p_agent->Type != TT_PERSON)
        return WEP_NULL;
    if (plagent != (p_agent->U.UPerson.ComCur & 3)) {
        LOGERR("Player %d agent (thing %d) claims it has slot %d while in fact it fills %d",
          (int)plyr, (int)p_agent->ThingOffset, (int)(p_agent->U.UPerson.ComCur & 3), (int)plagent);
        return WEP_NULL;
    }

    curwep = p_agent->U.UPerson.CurrentWeapon;
    if (curwep == WEP_NULL) {
        curwep = p_player->PrevWeapon[plagent];
    }
    return curwep;
}

short player_agent_weapon_delay(PlayerIdx plyr, ushort plagent, ubyte weapon)
{
    PlayerInfo *p_player;

    p_player = &players[plyr];
    return p_player->WepDelays[plagent][weapon];
}


TbBool player_agent_has_weapon(PlayerIdx plyr, ushort plagent, ubyte weapon)
{
    PlayerInfo *p_player;

    p_player = &players[plyr];
    return weapons_has_weapon(p_player->Weapons[plagent], weapon);
}

TbBool player_agent_is_alive(PlayerIdx plyr, ushort plagent)
{
    PlayerInfo *p_player;
    struct Thing *p_agent;

    p_player = &players[plyr];
    p_agent = p_player->MyAgent[plagent];
    if (p_agent->Type != TT_PERSON)
        return false;

    return ((p_agent->Flag & TngF_Destroyed) == 0);
}

TbBool player_agent_is_executing_commands(PlayerIdx plyr, ushort plagent)
{
    PlayerInfo *p_player;
    struct Thing *p_agent;

    p_player = &players[plyr];
    p_agent = p_player->MyAgent[plagent];
    if (p_agent->Type != TT_PERSON)
        return false;

    return person_is_executing_commands(p_agent->ThingOffset);
}

TbBool free_slot(ushort plagent, ubyte weapon)
{
    TbBool ret;
    asm volatile ("call ASM_free_slot\n"
        : "=r" (ret) : "a" (plagent & 0xff), "d" (weapon));
    return ret;
}

TbBool player_cryo_add_weapon_one(ushort cryo_no, ubyte weapon)
{
    TbBool added;

    added = weapons_add_one(&cryo_agents.Weapons[cryo_no], &cryo_agents.FourPacks[cryo_no], weapon);
    if (!added)
        return false;

    if (cryo_no < 4) {
        PlayerInfo *p_locplayer;
        p_locplayer = &players[local_player_no];
#if 0
        // TODO re-enable when player->FourPacks is unified and in the same format as cryo_agents.FourPacks
        weapons_add_one(&p_locplayer->Weapons[cryo_no], &p_player->FourPacks[cryo_no], weapon);
#else
        // Copying all weapons will work as well
        player_update_from_cryo_agent(cryo_no, p_locplayer, cryo_no);
#endif
    }
    return added;
}

TbBool player_cryo_add_cybmod(ushort cryo_no, ubyte cybmod)
{
    if (!check_mod_allowed_to_flags(&cryo_agents.Mods[cryo_no], cybmod))
        return false;

    add_mod_to_flags(&cryo_agents.Mods[cryo_no], cybmod);

    if (cryo_no < 4) {
        PlayerInfo *p_locplayer;
        p_locplayer = &players[local_player_no];

        add_mod_to_flags(&p_locplayer->Mods[cryo_no], cybmod);
    }

    return true;
}

TbBool player_cryo_remove_weapon_one(ushort cryo_no, ubyte weapon)
{
    TbBool rmved;

    rmved = weapons_remove_one(&cryo_agents.Weapons[cryo_no], &cryo_agents.FourPacks[cryo_no], weapon);
    if (!rmved)
        return false;

    if (cryo_no < 4) {
        PlayerInfo *p_locplayer;
        p_locplayer = &players[local_player_no];
#if 0
        // TODO re-enable when player->FourPacks is unified and in the same format as cryo_agents.FourPacks
        weapons_remove_one(&p_locplayer->Weapons[cryo_no], &p_player->FourPacks[cryo_no], weapon);
#else
        // Copying all weapons will work as well
        player_update_from_cryo_agent(cryo_no, p_locplayer, cryo_no);
#endif
    }
    return rmved;
}

TbBool player_cryo_transfer_weapon_between_agents(ushort from_cryo_no,
  ushort to_cryo_no, ubyte weapon)
{
    TbBool added;

    added = player_cryo_add_weapon_one(to_cryo_no, weapon);
    if (!added)
        return false;
    player_cryo_remove_weapon_one(from_cryo_no, weapon);
    return added;
}

const char *get_cryo_agent_name(ushort cryo_no)
{
    ushort rndname;

    if (selected_agent < 0)
        return gui_strings[536];

    rndname = cryo_agents.RandomName[cryo_no];
    if (background_type == 1)
    {
        if (cryo_agents.Sex & (1 << selected_agent))
            return gui_strings[227 + rndname];
        else
            return gui_strings[177 + rndname];
    }
    else
    {
        return gui_strings[77 + rndname];
    }

}

void remove_agent(ubyte cryo_no)
{
    asm volatile ("call ASM_remove_agent\n"
        : : "a" (cryo_no));
}

void add_agent(ulong weapons, ushort mods)
{
    asm volatile ("call ASM_add_agent\n"
        : : "a" (weapons), "d" (mods));
}

ThingIdx direct_control_thing_for_player(PlayerIdx plyr)
{
    PlayerInfo *p_player;
    ThingIdx dcthing;

    p_player = &players[plyr];
    if (p_player->DoubleMode)
        dcthing = p_player->DirectControl[byte_153198-1];
    else
        dcthing = p_player->DirectControl[0];
    return dcthing;
}

void set_default_player_control(void)
{
    PlayerInfo *p_locplayer;
    short i;

    p_locplayer = &players[local_player_no];
    p_locplayer->DoubleMode = 0;
    for (i = 0; i < 4; i++)
        p_locplayer->UserInput[i].ControlMode = UInpCtr_Mouse;
}

void player_target_clear(PlayerIdx plyr)
{
    PlayerInfo *p_player;

    p_player = &players[plyr];
    p_player->Target = 0;
    p_player->TargetType = TrgTp_NONE;
}

void kill_my_players(PlayerIdx plyr)
{
    asm volatile ("call ASM_kill_my_players\n"
        : : "a" (plyr));
}

/******************************************************************************/
