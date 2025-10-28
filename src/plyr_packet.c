/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file plyr_packet.c
 *     Players User Input handling.
 * @par Purpose:
 *     Implement functions for storing user input and building packets
 *     which will later influence the game world.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     11 Dec 2024 - 01 Feb 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "plyr_packet.h"

#include <assert.h>
#include "bfscd.h"
#include "bfmusic.h"
#include "ssampply.h"

#include "game_bstype.h"
#include "game.h"
#include "guitext.h"
#include "hud_panel.h"
#include "network.h"
#include "packet.h"
#include "player.h"
#include "sound.h"
#include "thing.h"
#include "swlog.h"
/******************************************************************************/

void net_player_leave(PlayerIdx plyr)
{
    kill_my_players(plyr);
    if ((plyr == net_host_player_no) || (plyr == local_player_no) || (nsvc.I.Type != 1))
    {
        ingame.DisplayMode = DpM_PURPLEMNU;
        StopCD();
        StopAllSamples();
        SetMusicVolume(100, 0);
        LbNetworkSessionStop();
        if (nsvc.I.Type != 1 && byte_1C4A6F)
            LbNetworkHangUp();
    }
    else
    {
        net_players_num--;
        sprintf(player_unknCC9[plyr], "%s %s", unkn2_names[plyr], gui_strings[GSTR_NET_LEFT_GAME]);
        player_unkn0C9[plyr] = -106;
        LbNetworkSessionStop();
        ingame.InNetGame_UNSURE &= ~(1 << plyr);
    }
}

void player_agent_select(PlayerIdx plyr, ThingIdx person)
{
    if (person == (ThingIdx)players[plyr].DirectControl[mouser])
        return;
    if (plyr == local_player_no)
    {
        struct Thing *p_person;
        ushort smp;

        p_person = &things[person];
        if (p_person->SubType == SubTT_PERS_AGENT)
            smp = 44; // 'selected' speech
        else
            smp = 46;
        play_disk_sample(0, smp, 127, 64, 100, 0, 3);
    }
    if (person != (ThingIdx)players[plyr].DirectControl[0])
    {
        player_change_person(person, plyr);
    }
}

void thing_goto_point_rel_fast(struct Thing *p_thing, short x, short y, short z, int plyr)
{
    asm volatile (
      "push %4\n"
      "call ASM_thing_goto_point_rel_fast\n"
        : : "a" (p_thing), "d" (x), "b" (y), "c" (z), "g" (plyr));
}

void thing_goto_point_rel(struct Thing *p_thing, short x, short y, short z)
{
    asm volatile (
      "call ASM_thing_goto_point_rel\n"
        : : "a" (p_thing), "d" (x), "b" (y), "c" (z));
}

void thing_goto_point_fast(struct Thing *p_thing, short x, short y, short z, int plyr)
{
    asm volatile (
      "push %4\n"
      "call ASM_thing_goto_point_fast\n"
        : : "a" (p_thing), "d" (x), "b" (y), "c" (z), "g" (plyr));
}

void thing_goto_point(struct Thing *p_thing, short x, short y, short z)
{
    asm volatile (
      "call ASM_thing_goto_point\n"
        : : "a" (p_thing), "d" (x), "b" (y), "c" (z));
}

void thing_goto_point_on_face_fast(struct Thing *p_thing, short x, short z, short face, int plyr)
{
    asm volatile (
      "push %4\n"
      "call ASM_thing_goto_point_on_face_fast\n"
        : : "a" (p_thing), "d" (x), "b" (z), "c" (face), "g" (plyr));
}

void thing_goto_point_on_face(struct Thing *p_thing, short x, short z, short face)
{
    asm volatile (
      "call ASM_thing_goto_point_on_face\n"
        : : "a" (p_thing), "d" (x), "b" (z), "c" (face));
}

ubyte select_new_weapon(ushort index, short dir)
{
    ubyte ret;
    asm volatile ("call ASM_select_new_weapon\n"
        : "=r" (ret) : "a" (index), "d" (dir));
    return ret;
}

void unkn_player_group_add(sbyte a1, ubyte a2)
{
    asm volatile ("call ASM_unkn_player_group_add\n"
        :  : "a" (a1), "d" (a2));
}

void unkn_player_group_prot(sbyte a1, ubyte a2)
{
    asm volatile ("call ASM_unkn_player_group_prot\n"
        :  : "a" (a1), "d" (a2));
}

void peep_change_weapon(struct Thing *p_person)
{
    asm volatile (
      "call ASM_peep_change_weapon\n"
        : : "a" (p_person));
}

void player_agent_weapon_switch(PlayerIdx plyr, ThingIdx person, short shift)
{
    struct Thing *p_person;

    p_person = &things[person];

    p_person->U.UPerson.CurrentWeapon = select_new_weapon(person, shift);
    peep_change_weapon(p_person);
    p_person->U.UPerson.AnimMode = gun_out_anim(p_person, 0);
    reset_person_frame(p_person);
    p_person->Speed = calc_person_speed(p_person);
    p_person->U.UPerson.TempWeapon = p_person->U.UPerson.CurrentWeapon;

    if ((plyr == local_player_no) && (p_person->U.UPerson.CurrentWeapon != 0))
    {
        ushort smp;
        // Weapon name speech
        if (background_type == 1)
            smp = weapon_sound_z[p_person->U.UPerson.CurrentWeapon];
        else
            smp = weapon_sound[p_person->U.UPerson.CurrentWeapon];
        play_disk_sample(local_player_no, smp, 127, 64, 100, 0, 3);
    }
}

void player_agent_init_drop_item(PlayerIdx plyr, struct Thing *p_person, ushort weapon)
{
    if ((weapon == 0) || (weapon == p_person->U.UPerson.CurrentWeapon)) {
        p_person->U.UPerson.AnimMode = 0;
        reset_person_frame(p_person);
    }
    if (p_person->State == PerSt_PROTECT_PERSON)
        p_person->Flag2 |= TgF2_Unkn10000000;
    person_init_drop(p_person, weapon);
    p_person->Speed = calc_person_speed(p_person);
}

void person_grp_switch_to_specific_weapon(struct Thing *p_person, PlayerIdx plyr,
  WeaponType wtype, ubyte first_flag)
{
    struct Thing *p_owntng;
    ushort plagent;
    ubyte flag;

    p_owntng = p_person;
    if (p_person->State == PerSt_PROTECT_PERSON)
        p_owntng = &things[p_person->Owner];

    peep_change_weapon(p_person);
    flag = thing_select_specific_weapon(p_person, wtype, first_flag);
    p_person->U.UPerson.TempWeapon = p_person->U.UPerson.CurrentWeapon;

    if ((plyr == local_player_no) && (p_person->U.UPerson.CurrentWeapon != 0))
        play_disk_sample(local_player_no, 0x2Cu, 127, 64, 100, 0, 3);

    for (plagent = 0; plagent < playable_agents; plagent++)
    {
        struct Thing *p_agent;

        p_agent = players[plyr].MyAgent[plagent];
        if ((p_agent->State != PerSt_PROTECT_PERSON) || (p_agent->Owner != p_owntng->ThingOffset)) {
            if (p_agent != p_owntng)
                continue;
        }
        if (p_agent == p_person)
            continue;

        if (!person_carries_weapon(p_agent, wtype) || (flag == WepSel_HIDE))
        {
            stop_looped_weapon_sample(p_agent, p_agent->U.UPerson.CurrentWeapon);
            if (flag == WepSel_HIDE)
            {
                player_agent_update_prev_weapon(p_agent);
                p_agent->U.UPerson.CurrentWeapon = WEP_NULL;
            }
            else if (p_agent->U.UPerson.TempWeapon != WEP_NULL)
            {
                thing_select_specific_weapon(p_agent, p_agent->U.UPerson.TempWeapon, flag);
            }
            else
            {
                choose_best_weapon_for_range(p_agent, 1280);
                p_agent->U.UPerson.AnimMode = gun_out_anim(p_agent, 0);
                reset_person_frame(p_agent);
                p_agent->Speed = calc_person_speed(p_agent);
            }
        }
        else
        {
            peep_change_weapon(p_agent);
            thing_select_specific_weapon(p_agent, wtype, flag);
        }
        p_agent->U.UPerson.TempWeapon = p_agent->U.UPerson.CurrentWeapon;
    }
}

short net_unkn_check_1(void)
{
    short ret;
    asm volatile ("call ASM_net_unkn_check_1\n"
        : "=r" (ret) : );
    return ret;
}

void player_chat_message_add_key(ushort a1, int a2)
{
    asm volatile ("call ASM_player_chat_message_add_key\n"
        :  : "a" (a1), "d" (a2));
}

void plgroup_set_mood(PlayerIdx plyr, struct Thing *p_member, short mood)
{
    struct Thing *p_owntng;
    ushort plagent;

    p_owntng = p_member;
    if (p_member->State == PerSt_PROTECT_PERSON)
        p_owntng = &things[p_member->Owner];

    p_member->U.UPerson.Mood = limit_mood(p_member, mood);
    p_member->Speed = calc_person_speed(p_member);

    for (plagent = 0; plagent < playable_agents; plagent++)
    {
        struct Thing *p_agent;

        p_agent = players[plyr].MyAgent[plagent];
        if ((p_agent <= &things[0]) || (p_agent >= &things[THINGS_LIMIT]))
            continue;

        if ((p_agent->State != PerSt_PROTECT_PERSON) || (p_agent->Owner != p_owntng->ThingOffset)) {
            if (p_agent != p_owntng)
                continue;
        }
        if (p_agent == p_member) // already updated
            continue;

        p_agent->U.UPerson.Mood = limit_mood(p_agent, mood);
        p_agent->Speed = calc_person_speed(p_agent);
    }
}

void player_agent_init_goto_gnd_point_abs(PlayerIdx plyr, struct Thing *p_person,
  MapCoord cor_x, MapCoord cor_y, MapCoord cor_z)
{
    if (plyr == local_player_no)
        show_goto_point(1);
    p_person->U.UPerson.Flag3 &= ~PrsF3_Unkn04;
    thing_goto_point(p_person, cor_x, cor_y, cor_z);
}

void player_agent_select_specific_weapon(PlayerIdx plyr, struct Thing *p_person,
  WeaponType wtype, ubyte flag)
{
    thing_select_specific_weapon(p_person, wtype, flag);
    peep_change_weapon(p_person);
    p_person->U.UPerson.AnimMode = gun_out_anim(p_person, 0);
    reset_person_frame(p_person);
    p_person->Speed = calc_person_speed(p_person);
    p_person->U.UPerson.TempWeapon = p_person->U.UPerson.CurrentWeapon;
    if ((plyr == local_player_no) && (p_person->U.UPerson.CurrentWeapon != 0))
    {
        ushort smp;
        if (background_type == 1)
            smp = weapon_sound_z[p_person->U.UPerson.CurrentWeapon];
        else
            smp = weapon_sound[p_person->U.UPerson.CurrentWeapon];
        play_disk_sample(local_player_no, smp, 127, 64, 100, 0, 3);
    }
}

void player_set_control_mode(PlayerIdx plyr, ushort ctrmode)
{
    players[plyr].UserInput[0].ControlMode = ctrmode;
}

void process_packet(PlayerIdx plyr, struct Packet *p_pckt, ushort i)
{
    struct Thing *p_thing;
    struct Thing *p_sectng;
    int n;
    short result;

    result = PARes_EBADRQC;
    switch (p_pckt->Action & 0x7FFF)
    {
    case PAct_MISSN_ABORT:
        if (in_network_game) {
            net_player_leave(plyr);
            result = PARes_DONE;
            break;
        }
        exit_game = 1;
        result = PARes_DONE;
        break;
    case PAct_AGENT_GOTO_GND_PT_ABS:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        player_agent_init_goto_gnd_point_abs(plyr, p_thing, p_pckt->X, p_pckt->Y, p_pckt->Z);
        result = PARes_DONE;
        break;
    case PAct_AGENT_GOTO_GND_PT_REL:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        thing_goto_point_rel(p_thing, p_pckt->X, p_pckt->Y, p_pckt->Z);
        result = PARes_DONE;
        break;
    case PAct_SELECT_NEXT_WEAPON:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        if (p_thing->State == PerSt_DROP_ITEM) {
            result = PARes_TNGBADST;
            break;
        }
        if ((p_thing->Flag2 & TgF2_KnockedOut) != 0) {
            result = PARes_TNGBADST;
            break;
        }
        player_agent_weapon_switch(plyr, p_pckt->Data, 1);
        result = PARes_DONE;
        break;
    case PAct_DROP_SELC_WEAPON_SECR:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        if (p_thing->State == PerSt_DROP_ITEM) {
            result = PARes_EALREADY;
            break;
        }
        if ((p_thing->Flag2 & TgF2_KnockedOut) != 0) {
            result = PARes_TNGBADST;
            break;
        }
        if (p_thing->U.UPerson.CurrentWeapon == 0) {
            result = PARes_TNGBADST;
            break;
        }
        player_agent_init_drop_item(plyr, p_thing, 0);
        result = PARes_DONE;
        break;
    case PAct_PICKUP:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        person_init_pickup(p_thing, p_pckt->X);
        result = PARes_DONE;
        break;
    case PAct_ENTER_VEHICLE:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        p_sectng = get_thing_safe(p_pckt->X, TT_VEHICLE);
        if (p_sectng == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        if ((p_thing->Flag2 & TgF2_Unkn0800) != 0) {
            result = PARes_TNGBADST;
            break;
        }
        person_enter_vehicle(p_thing, p_sectng);
        result = PARes_DONE;
        break;
    case PAct_LEAVE_VEHICLE:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        if ((p_thing->Flag2 & TgF2_Unkn0800) != 0) {
            result = PARes_TNGBADST;
            break;
        }
        person_attempt_to_leave_vehicle(p_thing);
        result = PARes_DONE;
        break;
    case PAct_SELECT_AGENT:
        p_sectng = get_thing_safe(p_pckt->X, TT_PERSON);
        if (p_sectng == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_sectng, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        player_agent_select(plyr, p_pckt->X);
        result = PARes_DONE;
        break;
    case PAct_AGENT_GOTO_GND_PT_REL_FF:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        thing_goto_point_rel_fast(p_thing, p_pckt->X, p_pckt->Y, p_pckt->Z, plyr);
        result = PARes_DONE;
        break;
    case PAct_SHOOT_AT_GND_POINT:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        thing_shoot_at_point(p_thing, p_pckt->X, p_pckt->Y, p_pckt->Z, 0);
        result = PARes_DONE;
        break;
    case PAct_SELECT_PREV_WEAPON:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        if (p_thing->State == PerSt_DROP_ITEM) {
            result = PARes_TNGBADST;
            break;
        }
        if ((p_thing->Flag2 & TgF2_KnockedOut) != 0) {
            result = PARes_TNGBADST;
            break;
        }
        player_agent_weapon_switch(plyr, p_pckt->Data, -1);
        result = PARes_DONE;
        break;
    case PAct_PROTECT_INC:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        call_protect(p_thing, plyr);
        n = count_protect(p_thing, plyr);
        if (plyr == local_player_no && n)
            play_sample_using_heap(0, 61, 127, 64, 5 * n + 90, 0, 3);
        result = PARes_DONE;
        break;
    case PAct_PROTECT_TOGGLE:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        call_unprotect(p_thing, plyr, 0);
        n = count_protect(p_thing, plyr);
        if (plyr == local_player_no && n)
            play_sample_using_heap(0, 61, 127, 64, 5 * n + 90, 0, 3);
        result = PARes_DONE;
        break;
    case PAct_SHOOT_AT_THING:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        thing_shoot_at_thing(p_thing, p_pckt->X);
        result = PARes_DONE;
        break;
    case PAct_GET_ITEM:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        person_init_get_item(p_thing, p_pckt->X, plyr);
        result = PARes_DONE;
        break;
    case PAct_PLANT_MINE_AT_GND_PT:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        if (p_thing->State == PerSt_DROP_ITEM) {
            result = PARes_EALREADY;
            break;
        }
        if ((p_thing->Flag2 & TgF2_KnockedOut) != 0) {
            result = PARes_TNGBADST;
            break;
        }
        person_init_plant_mine(p_thing, p_pckt->X, p_pckt->Y, p_pckt->Z, 0);
        result = PARes_DONE;
        break;
    case PAct_SELECT_SPECIFIC_WEAPON:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (p_pckt->Y > WepSel_HIDE) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        if (p_thing->State == PerSt_DROP_ITEM) {
            result = PARes_TNGBADST;
            break;
        }
        if ((p_thing->Flag2 & TgF2_KnockedOut) != 0) {
            result = PARes_TNGBADST;
            break;
        }
        player_agent_select_specific_weapon(plyr, p_thing, p_pckt->X, p_pckt->Y);
        result = PARes_DONE;
        break;
    case PAct_DROP_HELD_WEAPON_SECR:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        if (p_thing->State == PerSt_DROP_ITEM) {
            result = PARes_EALREADY;
            break;
        }
        if ((p_thing->Flag2 & TgF2_KnockedOut) != 0) {
            result = PARes_TNGBADST;
            break;
        }
        player_agent_init_drop_item(plyr, p_thing, p_pckt->X);
        result = PARes_DONE;
        break;
    case PAct_AGENT_SET_MOOD:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        p_thing->U.UPerson.Mood = limit_mood(p_thing, p_pckt->X);
        p_thing->Speed = calc_person_speed(p_thing);
        result = PARes_DONE;
        break;
    case PAct_GO_ENTER_VEHICLE:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        p_sectng = get_thing_safe(p_pckt->X, TT_VEHICLE);
        if (p_sectng == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        person_go_enter_vehicle(p_thing, p_sectng);
        result = PARes_DONE;
        break;
    case PAct_FOLLOW_PERSON:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        p_sectng = get_thing_safe(p_pckt->X, TT_PERSON);
        if (p_sectng == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        if ((p_thing->Flag2 & TgF2_Unkn0800) != 0) {
            result = PARes_TNGBADST;
            break;
        }
        person_init_follow_person(p_thing, p_sectng);
        result = PARes_DONE;
        break;
    case PAct_CONTROL_MODE:
        if ((p_pckt->Data & ~UInpCtr_AllFlagsMask) >= UInpCtr_MODES_COUNT) {
            result = PARes_EINVAL;
            break;
        }
        player_set_control_mode(plyr, p_pckt->Data);
        result = PARes_DONE;
        break;
    case PAct_AGENT_GOTO_FACE_PT_ABS:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        if (plyr == local_player_no)
            show_goto_point(1);
        p_thing->U.UPerson.Flag3 &= ~PrsF3_Unkn04;
        thing_goto_point_on_face(p_thing, p_pckt->X, p_pckt->Z, p_pckt->Y);
        result = PARes_DONE;
        break;
    case PAct_AGENT_GOTO_GND_PT_ABS_FF:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        if (plyr == local_player_no)
            show_goto_point(1);
        p_thing->U.UPerson.Flag3 &= ~PrsF3_Unkn04;
        thing_goto_point_fast(p_thing, p_pckt->X, p_pckt->Y, p_pckt->Z, plyr);
        result = PARes_DONE;
        break;
    case PAct_AGENT_GOTO_FACE_PT_ABS_FF:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        if (plyr == local_player_no)
            show_goto_point(1);
        p_thing->U.UPerson.Flag3 &= ~PrsF3_Unkn04;
        thing_goto_point_on_face_fast(p_thing, p_pckt->X, p_pckt->Z, p_pckt->Y, plyr);
        result = PARes_DONE;
        break;
    case PAct_GO_ENTER_VEHICLE_FF:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        p_sectng = get_thing_safe(p_pckt->X, TT_VEHICLE);
        if (p_sectng == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        person_go_enter_vehicle_fast(p_thing, p_sectng, plyr);
        result = PARes_DONE;
        break;
    case PAct_GET_ITEM_FAST:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        person_init_get_item_fast(p_thing, p_pckt->X, plyr);
        result = PARes_DONE;
        break;
    case PAct_SHIELD_TOGGLE:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        if ((p_thing->Flag2 & TgF2_Unkn0800) != 0) {
            result = PARes_TNGBADST;
            break;
        }
        person_shield_toggle(p_thing, plyr);
        result = PARes_DONE;
        break;
    case PAct_PLANT_MINE_AT_GND_PT_FF:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        if (p_thing->State == PerSt_DROP_ITEM) {
            result = PARes_EALREADY;
            break;
        }
        if ((p_thing->Flag2 & TgF2_KnockedOut) != 0) {
            result = PARes_TNGBADST;
            break;
        }
        person_init_plant_mine_fast(p_thing, p_pckt->X, p_pckt->Y, p_pckt->Z, 0);
        result = PARes_DONE;
        break;
    case PAct_SHOOT_AT_GND_POINT_FF:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        thing_shoot_at_point(p_thing, p_pckt->X, p_pckt->Y, p_pckt->Z, 1);
        result = PARes_DONE;
        break;
    case PAct_PEEPS_SCATTER:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        if ((p_thing->Flag2 & TgF2_Unkn0800) != 0) {
            result = PARes_TNGBADST;
            break;
        }
        make_peeps_scatter(p_thing, p_pckt->X, p_pckt->Z);
        result = PARes_DONE;
        break;
    case PAct_SELECT_GRP_SPEC_WEAPON:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (p_pckt->Y > WepSel_SKIP) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        if (p_thing->State == PerSt_DROP_ITEM) {
            result = PARes_TNGBADST;
            break;
        }
        if ((p_thing->Flag2 & TgF2_KnockedOut) != 0) {
            result = PARes_TNGBADST;
            break;
        }
        person_grp_switch_to_specific_weapon(p_thing, plyr, p_pckt->X, p_pckt->Y);
        result = PARes_DONE;
        break;
    case PAct_AGENT_USE_MEDIKIT:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        if (!person_carries_any_medikit(p_thing->ThingOffset)) {
            result = PARes_TNGBADST;
            break;
        }
        person_use_medikit(p_thing, plyr);
        result = PARes_DONE;
        break;
    case PAct_GROUP_SET_MOOD:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        plgroup_set_mood(plyr, p_thing, p_pckt->X);
        result = PARes_DONE;
        break;
    case PAct_AGENT_UNKGROUP_PROT: // Unfinished mess; remove pending
        unkn_player_group_prot(p_pckt->Data, plyr);
        result = PARes_DONE;
        break;
    case PAct_AGENT_UNKGROUP_ADD: // Unfinished mess; remove pending
        unkn_player_group_add(p_pckt->Data, plyr);
        result = PARes_DONE;
        break;
    case PAct_CHAT_MESSAGE_KEY:
        player_chat_message_add_key(plyr, p_pckt->Data);
        result = PARes_DONE;
        break;
    case PAct_SHOOT_AT_FACE_POINT:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        thing_shoot_at_point(p_thing, p_pckt->X, p_pckt->Y, p_pckt->Z, 2);
        result = PARes_DONE;
        break;
    case PAct_SHOOT_AT_FACE_POINT_FF:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        thing_shoot_at_point(p_thing, p_pckt->X, p_pckt->Y, p_pckt->Z, 3);
        result = PARes_DONE;
        break;
    case PAct_PLANT_MINE_AT_FACE_PT:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        if (p_thing->State == PerSt_DROP_ITEM) {
            result = PARes_EALREADY;
            break;
        }
        if ((p_thing->Flag2 & TgF2_KnockedOut) != 0) {
            result = PARes_TNGBADST;
            break;
        }
        person_init_plant_mine(p_thing, p_pckt->X, 0, p_pckt->Z, p_pckt->Y);
        result = PARes_DONE;
        break;
    case PAct_PLANT_MINE_AT_FACE_PT_FF:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        if (p_thing->State == PerSt_DROP_ITEM) {
            result = PARes_EALREADY;
            break;
        }
        if ((p_thing->Flag2 & TgF2_KnockedOut) != 0) {
            result = PARes_TNGBADST;
            break;
        }
        person_init_plant_mine_fast(p_thing, p_pckt->X, 0, p_pckt->Z, p_pckt->Y);
        result = PARes_DONE;
        break;
    case PAct_AGENT_SELF_DESTRUCT:
        p_thing = get_thing_safe(p_pckt->Data, TT_PERSON);
        if (p_thing == INVALID_THING) {
            result = PARes_EINVAL;
            break;
        }
        if (person_slot_as_player_agent(p_thing, plyr) < 0) {
            result = PARes_EBADSLT;
            break;
        }
        if ((p_thing->Flag2 & TgF2_Unkn0800) != 0) {
            result = PARes_TNGBADST;
            break;
        }
        person_self_destruct(p_thing);
        result = PARes_DONE;
        break;
    case PAct_NONE:
        result = PARes_DONE;
        break;
    }
    if (result > PARes_SUCCESS) {
        LOGWARN("Player %d action %s: %s", (int)plyr,
          get_packet_action_name(p_pckt->Action & 0x7FFF),
          get_packet_action_result_text(result));
    }
}

void process_packets(void)
{
#if 0
    asm volatile ("call ASM_process_packets\n"
        :  :  : "eax" );
    return;
#endif
    ushort v53;
    PlayerIdx plyr;

    if (pktrec_mode == PktR_NONE)
        v53 = 4;
    else if (pktrec_mode <= PktR_PLAYBACK)
        v53 = 1;

    if (in_network_game && (net_players_num > 1))
        net_unkn_check_1();

    for (plyr = 0; plyr < PLAYERS_LIMIT; plyr++)
    {
        struct Packet *packet;
        ushort i;

        if (((1 << plyr) & ingame.InNetGame_UNSURE) == 0)
            continue;
        packet = &packets[plyr];
        for (i = 0; i < v53; i++)
        {
            struct Thing *p_thing;

            p_thing = get_thing_safe(packet->Data, TT_PERSON);

            if (((1 << plyr) & ingame.InNetGame_UNSURE) == 0)
                packet->Action = PAct_NONE;

            if (p_thing != INVALID_THING)
            {
                if ((packet->Action & 0x8000) == 0)
                    p_thing->Flag &= ~TngF_Unkn0800;
                else
                    p_thing->Flag |= TngF_Unkn0800;
            }

            process_packet(plyr, packet, i);

            packet->Action = PAct_NONE;
            packet = (struct Packet *)((char *)packet + 10);
        }
    }
}

/******************************************************************************/
