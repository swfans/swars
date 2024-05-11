/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file thing_debug.c
 *     Thing related debug modes.
 * @par Purpose:
 *     Implement dump functions and debug modes for things.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 11 Feb 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "thing.h"

#include <string.h>
#include "bfkeybd.h"
#include "bfbox.h"
#include "command.h"
#include "drawtext.h"
#include "display.h"
#include "game.h"
#include "pathtrig.h"
#include "scandraw.h"
#include "vehicle.h"
#include "weapon.h"
#include "swlog.h"
/******************************************************************************/
TbBool debug_hud_things = false;

extern ubyte execute_commands;
extern struct Thing *p_track_thing;
extern struct Thing *p_track2_thing;
extern int dword_1DC7A4;
extern short word_1DC7A0;
extern short word_1DC7A2;

int select_thing_for_debug(short x, short y, short z, short type)
{
    int ret;
    asm volatile ("call ASM_select_thing_for_debug\n"
        : "=r" (ret) : "a" (x), "d" (y), "b" (z), "c" (type));
    return ret;
}

int unused_func_204(short a1, short a2, short a3, struct Thing *p_person)
{
    int ret;
    asm volatile ("call ASM_unused_func_204\n"
        : "=r" (ret) : "a" (a1), "d" (a2), "b" (a3), "c" (p_person));
    return ret;
}

void draw_unkn_func_07(short x, short y, short a3, short a4, ubyte a5)
{
    asm volatile (
      "push %4\n"
      "call ASM_draw_unkn_func_07\n"
        : : "a" (x), "d" (y), "b" (a3), "c" (a4), "g" (a5));
}

// TODO separate get_person_commands_debug_hud_inputs() from the below
void person_commands_debug_hud(int x, int y, int w, int h, short person, ubyte col1, ubyte col2, ubyte col3)
{
#if 0
    asm volatile (
      "push %7\n"
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_person_commands_debug_hud\n"
        : : "a" (x), "d" (y), "b" (w), "c" (h), "g" (person), "g" (col1), "g" (col2), "g" (col3));
    return;
#endif
    struct Thing *p_person;
    ushort cmds_count;
    short cmd, cmdhead;
#if 0
    short hilight_cmd;
#endif
    short cy;
    short ms_y;
    char locstr[52];
    short box_x, box_y;
    short box_width, box_height;
    short row_height;
    int j;

#if 0
    hilight_cmd = -1;
#endif
    box_width = 200;
    p_person = &things[person];
    box_height = 150;
    box_x = 400;
    box_y = 100;
    row_height = 16;
    cmdhead = p_person->U.UPerson.ComHead;
    if (lbDisplay.GraphicsScreenHeight >= 400)
        box_width = 100;
    cmds_count = 0;
    for (cmd = cmdhead; cmd; cmds_count++)
        cmd = game_commands[cmd].Next;

    if (cmds_count == 0)
        return;

    if (16 * cmds_count + 8 < box_height)
        box_height = 16 * cmds_count + 8;
    if ((word_1DC7A0 >> 2) > word_1DC7A2 - 4)
        word_1DC7A0 = 0;
    word_1DC7A2 = cmds_count;
    lbDisplay.DrawFlags = 0x0004;
    if (lbDisplay.GraphicsScreenHeight < 400)
        LbDrawBox(box_x/2, box_y/2, box_width/2, box_height/2, col3);
    else
        LbDrawBox(box_x, box_y, box_width, box_height, col3);
    lbDisplay.DrawFlags = 0;
    draw_unkn_func_07(box_x, box_y, box_width, box_height, col1);
    cmd = cmdhead;
    for (j = word_1DC7A0 >> 2; cmd; j--)
    {
        if (j == 0)
            break;
        cmd = game_commands[cmd].Next;
    }
    cy = box_y + 4 - 4 * (word_1DC7A0 & 3);
    person_command_to_text(0, 0, 1);

    while (cmd != 0)
    {
        if (cy > box_y)
        {
            if (mouse_move_over_box_coords(box_x + 8, cy, box_x + 8 + box_width, cy + row_height - 1))
            {
                if (lbDisplay.GraphicsScreenHeight < 400)
                    LbDrawBox((box_x + 4)/2, (cy - 2)/2, (box_width - 8)/2, (row_height - 1)/2, col1);
                else
                    LbDrawBox(box_x + 4, cy - 2, box_width - 8, row_height - 1, col1);
            }
            if ((p_person != NULL) && (p_person->U.UPerson.ComCur == cmd))
            {
                if (lbDisplay.GraphicsScreenHeight < 400)
                    LbDrawBox((box_x + 4)/2, (cy - 2)/2, (box_width - 8)/2, (row_height - 1)/2, colour_lookup[3]);
                else
                    LbDrawBox(box_x + 4, cy - 2, box_width - 8, row_height - 1, colour_lookup[3]);
            }

#if 0
            if (mouse_move_over_box_coords(box_x + 8, cy, box_x + 8 + box_width, cy + row_height - 1))
            {
                hilight_cmd = cmd;
            }
#endif
            if (p_person != NULL)
                unused_func_204(box_x + 8 - 20, cy + 5, cmd, p_person);
            if (person_command_to_text(locstr, cmd, 0))
                draw_text(box_x + 8, cy, locstr, col2);
            else
                cy -= row_height;
            if (cy + 28 > box_height + box_y)
                break;
        }
        cy += row_height;
        cmd = game_commands[cmd].Next;
        ++j;
    }

    ms_y = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MMouseY : lbDisplay.MMouseY;
    if ((ms_y < box_y - 20) && (word_1DC7A0 > 0))
        word_1DC7A0--;
    if ((ms_y > box_height + box_y + 10) && (word_1DC7A0 >> 2 < word_1DC7A2 - (box_height >> 4)))
        word_1DC7A0++;
}

void things_debug_hud(void)
{
#if 0
    asm volatile ("call ASM_things_debug_hud\n"
        :  :  : "eax" );
    return;
#endif
    short thing;
    short path;
    short pasngr;
    char locstr[100];

    thing = select_thing_for_debug(mouse_map_x, 0, mouse_map_z, -1);
    // Lock on current thing
    if (lbKeyOn[KC_W])
    {
        lbKeyOn[KC_W] = 0;
        if (lbShift & KMod_SHIFT) {
            dword_1DC7A4 = 0;
        } else if (thing > 0) {
            dword_1DC7A4 = thing;
        }
    }

    if (dword_1DC7A4)
        thing = dword_1DC7A4;

    if (lbShift == KMod_SHIFT)
        return;
    if (thing <= 0 || thing >= THINGS_LIMIT)
        return;

    p_track_thing = &things[thing];
    p_track2_thing = &things[thing];
    func_6fe80(mouse_map_x, mouse_map_y, mouse_map_z,
      p_track_thing->X >> 8, p_track_thing->Y >> 5, p_track_thing->Z >> 8,
      colour_lookup[1]);
    // Show commands list
    if (p_track_thing->Type == TT_PERSON)
          person_commands_debug_hud(356, 80, 280, 150, thing, colour_lookup[1], colour_lookup[2], colour_lookup[4]);
    else if ((p_track_thing->Type == TT_VEHICLE) && (p_track_thing->U.UVehicle.PassengerHead > 0))
          person_commands_debug_hud(356, 80, 280, 150, p_track_thing->U.UVehicle.PassengerHead, colour_lookup[1], colour_lookup[2], colour_lookup[4]);

    if (execute_commands)
    {
        snprintf(locstr, sizeof(locstr), "State %d.%d ",
          (int)p_track_thing->State, (int)p_track_thing->SubState);
        switch (p_track_thing->Type)
        {
        case TT_VEHICLE:
            snprint_vehicle_state(locstr+strlen(locstr), sizeof(locstr)-strlen(locstr), p_track_thing);
            snprintf(locstr+strlen(locstr), sizeof(locstr)-strlen(locstr), " Pasng %d G %d comcur %x EG %d th %d wb %d",
              (int)veh_passenger_count(p_track_thing),
              (int)p_track_thing->U.UVehicle.Group,
              (uint)p_track_thing->U.UVehicle.ComCur,
              (int)p_track_thing->U.UVehicle.EffectiveGroup,
              (int)p_track_thing->ThingOffset,
              (int)p_track_thing->U.UVehicle.WobbleZP);
            break;
        case TT_PERSON:
            snprint_person_state(locstr+strlen(locstr), sizeof(locstr)-strlen(locstr), p_track_thing);
            snprintf(locstr+strlen(locstr), sizeof(locstr)-strlen(locstr), " Mood %d G %d comcur %x EG %d th %d am %d",
              (int)p_track_thing->U.UPerson.Mood,
              (int)p_track_thing->U.UPerson.Group,
              (uint)p_track_thing->U.UPerson.ComCur,
              (int)p_track_thing->U.UPerson.EffectiveGroup,
              (int)p_track_thing->ThingOffset,
              (int)p_track_thing->U.UPerson.AnimMode);
            break;
        default:
            snprintf(locstr+strlen(locstr), sizeof(locstr)-strlen(locstr), " th %d",
              (int)p_track_thing->ThingOffset);
            break;
        }
        draw_text(30, 30, locstr, colour_lookup[1]);

        switch (p_track_thing->Type)
        {
        case TT_VEHICLE:
            sprintf(locstr, "F  %08x Spd %d OF %d He %d/%d Armr %d SF %d F %d",
              (uint)p_track_thing->Flag,
              (int)p_track_thing->Speed,
              (int)p_track_thing->U.UVehicle.OnFace,
              (int)p_track_thing->Health,
              (int)p_track_thing->U.UVehicle.MaxHealth,
              (int)p_track_thing->U.UVehicle.Armour,
              (int)p_track_thing->StartFrame,
              (int)p_track_thing->Frame);
            break;
        case TT_PERSON:
            sprintf(locstr, "F  %08x Spd %d OF %d He %d/%d Se %d/%d SF %d F %d",
              (uint)p_track_thing->Flag,
              (int)p_track_thing->Speed,
              (int)p_track_thing->U.UPerson.OnFace,
              (int)p_track_thing->Health,
              (int)p_track_thing->U.UPerson.MaxHealth,
              (int)p_track_thing->U.UPerson.ShieldEnergy,
              (int)p_track_thing->U.UPerson.MaxShieldEnergy,
              (int)p_track_thing->StartFrame,
              (int)p_track_thing->Frame);
            break;
        default:
            sprintf(locstr, "F  %08x Spd %d He %d SF %d F %d",
              (uint)p_track_thing->Flag,
              (int)p_track_thing->Speed,
              (int)p_track_thing->Health,
              (int)p_track_thing->StartFrame,
              (int)p_track_thing->Frame);
            break;
        }
        draw_text(30, 45, locstr, colour_lookup[1]);

        switch (p_track_thing->Type)
        {
        case TT_VEHICLE:
            sprintf(locstr, "F2 %08x mi %d TN %d poww %x wt %d PC %d",
              (uint)p_track_thing->Flag2,
              (int)p_track_thing->U.UVehicle.MatrixIndex,
              (int)p_track_thing->U.UVehicle.TNode,
              (uint)p_track_thing->U.UVehicle.PissedOffWithWaiting,
              (int)p_track_thing->U.UVehicle.WeaponTurn,
              my_paths[p_track_thing->U.UVehicle.PathIndex].Flag - p_track_thing->PathOffset);
            break;
        case TT_PERSON:
            sprintf(locstr, "F2 %08x F3 %x cw %d wc %x WE %d wti %d wt %d PC %d",
              (uint)p_track_thing->Flag2,
              (uint)p_track_thing->U.UPerson.Flag3,
              (int)p_track_thing->U.UPerson.CurrentWeapon,
              (uint)p_track_thing->U.UPerson.WeaponsCarried,
              (int)p_track_thing->U.UPerson.Energy,
              (int)p_track_thing->U.UPerson.WeaponTimer,
              (int)p_track_thing->U.UPerson.WeaponTurn,
              my_paths[p_track_thing->U.UPerson.PathIndex].Flag - p_track_thing->PathOffset);
            break;
        default:
            sprintf(locstr, "F2 %08x",
              (uint)p_track_thing->Flag2);
            break;
        }
        draw_text(30, 60, locstr, colour_lookup[1]);

        sprintf(locstr, "Targ2 %d pTarg %x gotoTI %d",
          (int)p_track_thing->U.UPerson.Target2,
          (uint)p_track_thing->PTarget,
          (int)p_track_thing->GotoThingIndex);
        draw_text(30, 75, locstr, colour_lookup[1]);

        if (p_track_thing->Flag & TngF_Unkn00040000)
            draw_text(30, 90, "Da", colour_lookup[1]);
        if (p_track_thing->Flag & TngF_Unkn00010000)
            draw_text(50, 90, "Re", colour_lookup[1]);
        if (p_track_thing->Flag & TngF_Unkn00020000)
            draw_text(70, 90, "Si", colour_lookup[1]);
        if (p_track_thing->Flag & TngF_Unkn0002)
            draw_text(90, 90, "De", colour_lookup[1]);
        if (p_track_thing->Flag & TngF_Unkn0400)
            draw_text(110, 90, "Ch", colour_lookup[1]);
        if (p_track_thing->Flag & TngF_Unkn0040)
            draw_text(130, 90, "CI", colour_lookup[1]);
        if (p_track_thing->Flag & TngF_Unkn20000000)
            draw_text(150, 90, "SAP", colour_lookup[1]);
        if (p_track_thing->Flag & TngF_Unkn0200)
            draw_text(190, 90, "Sta", colour_lookup[2]);
        if (p_track_thing->Flag & TngF_Unkn0800)
            draw_text(260, 90, "TRIG", colour_lookup[1]);

        switch (p_track_thing->Type)
        {
        case TT_VEHICLE:
                pasngr = p_track_thing->U.UVehicle.PassengerHead;
                if (pasngr == 0) {
                    sprintf(locstr, "%s: empty",
                      thing_type_name(p_track_thing->Type, p_track_thing->SubType));
                } else if (p_track_thing->U.UVehicle.PassengerHead > 0) {
                    struct Thing *p_pasngr;

                    p_pasngr = &things[pasngr];
                    sprintf(locstr, "%s: passenger %s offs %d lastdist %d",
                      thing_type_name(p_track_thing->Type, p_track_thing->SubType),
                      thing_type_name(p_pasngr->Type, p_pasngr->SubType),
                      (int)pasngr, (int)p_pasngr->U.UPerson.LastDist);
                } else { // Not expected to happen; but just in case
                    struct SimpleThing *p_spasngr;

                    p_spasngr = &sthings[pasngr];
                    sprintf(locstr, "%s: passenger %s offs %d",
                      thing_type_name(p_track_thing->Type, p_track_thing->SubType),
                      thing_type_name(p_spasngr->Type, p_spasngr->SubType), (int)pasngr);
                }
            draw_text(360, 90, locstr, colour_lookup[2]);
            break;
        case TT_PERSON:
            sprintf(locstr, "%s: lastdist %d VX,VZ (%d,%d)",
              thing_type_name(p_track_thing->Type, p_track_thing->SubType),
              (int)p_track_thing->U.UPerson.LastDist,
              (int)p_track_thing->VX,
              (int)p_track_thing->VZ);
            draw_text(360, 90, locstr, colour_lookup[3]);
            break;
        default:
            sprintf(locstr, "%s",
              thing_type_name(p_track_thing->Type, p_track_thing->SubType));
            draw_text(360, 90, locstr, colour_lookup[7]);
            break;
        }

        sprintf(locstr, "b %d l %d a %d br %d sk %d",
          (int)cybmod_chest_level(&p_track_thing->U.UPerson.UMod),
          (int)cybmod_legs_level(&p_track_thing->U.UPerson.UMod),
          (int)cybmod_arms_level(&p_track_thing->U.UPerson.UMod),
          (int)cybmod_brain_level(&p_track_thing->U.UPerson.UMod),
          (int)cybmod_skin_level(&p_track_thing->U.UPerson.UMod));
        draw_text(30, 105, locstr, colour_lookup[1]);

        sprintf(locstr, "T1 %d T2 %d ct %d RT %d BC %d",
          (int)p_track_thing->Timer1,
          (int)p_track_thing->U.UPerson.Timer2,
          (int)p_track_thing->U.UPerson.ComTimer,
          (int)p_track_thing->U.UPerson.RecoilTimer,
          (int)p_track_thing->U.UPerson.BumpCount);
        draw_text(30, 120, locstr, colour_lookup[1]);

        path = p_track_thing->U.UPerson.PathIndex;
        if (path != 0)
        {
            short cy;

            cy = 140;
            while (path != 0)
            {
                sprintf(locstr, " n %d  f %d  x %d z %d",
                  (int)path,
                  (int)my_paths[path].Flag,
                  (int)my_paths[path].X[0] >> 8,
                  (int)my_paths[path].Z[0] >> 8);
                draw_text(52, cy, locstr, colour_lookup[0]);
                draw_text(50, cy, locstr, colour_lookup[1]);
                path = my_paths[path].Next;
                cy += 15;
            }
        }

        if (p_track_thing->U.UPerson.Target2 != 0)
        {
            struct Thing *p_target;
            p_target = &things[p_track_thing->U.UPerson.Target2];
            unkn_draw_transformed_point(50, 41, p_target->X >> 8, p_target->Y >> 8, p_target->Z >> 8, colour_lookup[2]);
        }

        if (p_track_thing->GotoThingIndex != 0)
        {
            struct Thing *p_gotng;
            p_gotng = &things[p_track_thing->GotoThingIndex];
            unkn_draw_transformed_point(140, 41, p_gotng->X >> 8, p_gotng->Y >> 8, p_gotng->Z >> 8, colour_lookup[3]);
        }

        if (p_track_thing->PTarget != NULL)
        {
            struct Thing *p_target;
            p_target = p_track_thing->PTarget;
            func_6fd1c(
              p_track_thing->X >> 8,
              p_track_thing->Y >> 8,
              p_track_thing->Z >> 8,
              p_target->X >> 8,
              p_target->Y >> 8,
              p_target->Z >> 8,
              colour_lookup[4]);
        }

        unkn_path_func_001(p_track_thing, 0);
    }
}

TbBool person_command_to_text(char *out, ushort cmd, ubyte a3)
{
    struct Command *p_cmd;
    struct CommandDef *p_cdef;
    struct Thing *p_thing;
    struct SimpleThing *p_sthing;
    const char *cmd_name;
    char *o;
    TbBool result;

    o = out;
    if (a3 == 1)
        return a3 ^ 1;

    p_cmd = &game_commands[cmd];

    if (p_cmd->Flags & PCmdF_Unkn0001) {
        sprintf(o, "%d ", cmd);
        o += strlen(o);
    }
    if (p_cmd->Flags & PCmdF_RunUntil) {
        sprintf(o, "U ");
        o += strlen(o);
    }
    if ( p_cmd->Flags & PCmdF_IsUntil) {
        sprintf(o, "IU ");
        o += strlen(o);
    }
    if ( p_cmd->Flags & PCmdF_Unkn0008) {
        sprintf(o, "!!!");
        o += strlen(o);
    }

    p_cdef = &command_defs[p_cmd->Type];
    cmd_name = p_cdef->CmdName;
    if (strncmp(cmd_name, "TNG_CMD_", 8) == 0)
        cmd_name += 8;

    switch (p_cmd->Type)
    {
    case PCmd_GO_TO_POINT:
      sprintf(o, "%s X%d Y%d Z%d", cmd_name, p_cmd->X, p_cmd->Y, p_cmd->Z);
      result = 1;
      break;
    case PCmd_KILL_MEM_GROUP:
      sprintf(o, "KILL %d MEM GROUP %d", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_PERSUADE_MEM_GROUP:
      sprintf(o, "PERSUADE %d MEM G %d", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_KILL_ALL_GROUP:
    case PCmd_PERSUADE_ALL_GROUP:
    case PCmd_WAND_AVOID_GROUP:
    case PCmd_PROTECT_MEM_G:
      sprintf(o, "%s %d", cmd_name, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_GO_TO_PERSON:
    case PCmd_KILL_PERSON:
    case PCmd_PERSUADE_PERSON:
    case PCmd_BLOCK_PERSON:
    case PCmd_SCARE_PERSON:
    case PCmd_FOLLOW_PERSON:
    case PCmd_SUPPORT_PERSON:
    case PCmd_PROTECT_PERSON:
    case PCmd_AVOID_PERSON:
      p_thing = &things[p_cmd->OtherThing];
      sprintf(o, "%s %d idx %d", cmd_name, p_thing->U.UPerson.UniqueID, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_HIDE:
    case PCmd_UNKN16:
      sprintf(o, "%s", cmd_name);
      result = 1;
      break;
    case PCmd_GET_ITEM:
    case PCmd_DROP_SPEC_ITEM:
      p_sthing = &sthings[p_cmd->OtherThing];
      sprintf(o, "%s %d", cmd_name, p_sthing->UniqueID);
      result = 1;
      break;
    case PCmd_USE_WEAPON:
    case PCmd_DROP_WEAPON:
    case PCmd_SELECT_WEAPON:
      sprintf(o, "%s %s", cmd_name, weapon_codename(p_cmd->OtherThing));
      result = 1;
      break;
    case PCmd_DESTROY_BUILDING:
      sprintf(o, "%s %d", cmd_name, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_USE_VEHICLE:
      p_thing = &things[p_cmd->OtherThing];
      sprintf(o, "%s %d", cmd_name, p_thing->U.UVehicle.UniqueID);
      result = 1;
      break;
    case PCmd_EXIT_VEHICLE:
    case PCmd_PING_EXIST:
    case PCmd_SELF_DESTRUCT:
    case PCmd_KILL_EVERYONE:
    case PCmd_GUARD_OFF:
      sprintf(o, "%s", cmd_name);
      result = 1;
      break;
    case PCmd_CATCH_TRAIN:
      sprintf(o, "%s %d", cmd_name, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_OPEN_DOME:
    case PCmd_CLOSE_DOME:
      sprintf(o, "%s %d", cmd_name, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_CATCH_FERRY:
    case PCmd_EXIT_FERRY:
      sprintf(o, "%s %d", cmd_name, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_GOTOPOINT_FACE:
      sprintf(o, "%s %d X%d Z%d", cmd_name, p_cmd->OtherThing, p_cmd->X, p_cmd->Z);
      result = 1;
      break;
    case PCmd_RUN_TO_POINT:
      sprintf(o, "%s X%d Z%d", cmd_name, p_cmd->X, p_cmd->Z);
      result = 1;
      break;
    case PCmd_EXECUTE_COMS:
      sprintf(o, "%s (plyr)", cmd_name);
      result = 1;
      break;
    case PCmd_WAIT_P_V_DEAD:
      p_thing = &things[p_cmd->OtherThing];
      sprintf(o, "WAIT P/V %d DEAD", p_thing->U.UPerson.UniqueID);
      result = 1;
      break;
    case PCmd_WAIT_MEM_G_DEAD:
      sprintf(o, "WAIT %d MEM G %d DEAD", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAIT_ALL_G_DEAD:
    case PCmd_WAIT_ALL_G_ARRIVE:
    case PCmd_WAIT_ALL_G_PERSUADE:
    case PCmd_WAND_ALL_G_DEAD:
    case PCmd_WAND_ALL_G_ARRIVE:
    case PCmd_WAND_ALL_G_PERSUADE:
    case PCmd_UNTIL_ALL_G_DEAD:
    case PCmd_UNTIL_ALL_G_ARRIVE:
    case PCmd_UNTIL_ALL_G_PERSUADE:
      sprintf(o, "%s %d", cmd_name, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAIT_P_V_I_NEAR:
      p_thing = &things[p_cmd->OtherThing];
      sprintf(o, "WAIT P/V/I %d NEAR", p_thing->U.UPerson.UniqueID);
      result = 1;
      break;
    case PCmd_WAIT_MEM_G_NEAR:
      sprintf(o, "WAIT %d MEM G %d NEAR", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAIT_ALL_G_NEAR:
      sprintf(o, "WAIT ALL G %d NEAR", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAIT_P_V_I_ARRIVE:
      p_thing = &things[p_cmd->OtherThing];
      sprintf(o, "WAIT P/V/I %d ARRIVES X%d Y%d Z%d",
        p_thing->U.UPerson.UniqueID, p_cmd->X, p_cmd->Y, p_cmd->Z);
      result = 1;
      break;
    case PCmd_WAIT_MEM_G_ARRIVE:
      sprintf(o, "WAIT %d MEM G %d ARRIVE", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAIT_P_PERSUADE:
      p_thing = &things[p_cmd->OtherThing];
      sprintf(o, "%s %d", cmd_name, p_thing->U.UPerson.UniqueID);
      result = 1;
      break;
    case PCmd_WAIT_MEM_G_PERSUADE:
      sprintf(o, "WAIT %d MEM G %d PERSUADED", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAIT_MISSION_FAIL:
    case PCmd_WAIT_MISSION_START:
    case PCmd_WAIT_MISSION_SUCC:
    case PCmd_WAND_MISSION_SUCC:
    case PCmd_WAND_MISSION_FAIL:
    case PCmd_WAND_MISSION_START:
    case PCmd_UNTIL_MISSION_SUCC:
    case PCmd_UNTIL_MISSION_FAIL:
    case PCmd_UNTIL_MISSION_START:
      sprintf(o, "%s %d", cmd_name, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAIT_OBJT_DESTROY:
      sprintf(o, "%s %d X%d Y%d Z%d", cmd_name,
        p_cmd->OtherThing, p_cmd->X, p_cmd->Y, p_cmd->Z);
      result = 1;
      break;
    case PCmd_WAIT_TIME:
      sprintf(o, "%s %d", cmd_name, p_cmd->Time);
      result = 1;
      break;
    case PCmd_WAND_P_V_DEAD:
      p_thing = &things[p_cmd->OtherThing];
      sprintf(o, "WAND P/V %d DEAD", p_thing->U.UPerson.UniqueID);
      result = 1;
      break;
    case PCmd_WAND_MEM_G_DEAD:
      sprintf(o, "WAND %d MEM G %d DEAD", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAND_P_V_I_NEAR:
      p_thing = &things[p_cmd->OtherThing];
      sprintf(o, "WAND P/V/I %d NEAR", p_thing->U.UPerson.UniqueID);
      result = 1;
      break;
    case PCmd_WAND_MEM_G_NEAR:
      sprintf(o, "WAND %d MEM G %d NEAR", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAND_ALL_G_NEAR:
      sprintf(o, "WAND ALL G %d NEAR", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAND_P_V_I_ARRIVE:
      p_thing = &things[p_cmd->OtherThing];
      sprintf(o, "WAND P/V/I %d ARRIVES X%d Y%d Z%d",
        p_thing->U.UPerson.UniqueID, p_cmd->X, p_cmd->Y, p_cmd->Z);
      result = 1;
      break;
    case PCmd_WAND_MEM_G_ARRIVE:
      sprintf(o, "WAND %d MEM G %d ARRIVE", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAND_P_PERSUADE:
      p_thing = &things[p_cmd->OtherThing];
      sprintf(o, "%s %d", cmd_name, p_thing->U.UPerson.UniqueID);
      result = 1;
      break;
    case PCmd_WAND_MEM_G_PERSUADE:
      sprintf(o, "WAND %d MEM G %d PERSUADED", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAND_OBJT_DESTROY:
      sprintf(o, "%s %d X%d Y%d Z%d",
        cmd_name, p_cmd->OtherThing, p_cmd->X, p_cmd->Y, p_cmd->Z);
      result = 1;
      break;
    case PCmd_WAND_TIME:
      sprintf(o, "%s %d", cmd_name, p_cmd->Time);
      result = 1;
      break;
    case PCmd_LOOP_COM:
      sprintf(o, "%s %d", cmd_name, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_UNTIL_P_V_DEAD:
      p_thing = &things[p_cmd->OtherThing];
      sprintf(o, "UNTIL P/V %d DEAD", p_thing->U.UPerson.UniqueID);
      result = 1;
      break;
    case PCmd_UNTIL_MEM_G_DEAD:
      sprintf(o, "UNTIL %d MEM G %d DEAD", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_UNTIL_P_V_I_NEAR:
      p_thing = &things[p_cmd->OtherThing];
      sprintf(o, "UNTIL P/V/I %d NEAR", p_thing->U.UPerson.UniqueID);
      result = 1;
      break;
    case PCmd_UNTIL_MEM_G_NEAR:
      sprintf(o, "UNTIL %d MEM G %d NEAR", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_UNTIL_ALL_G_NEAR:
      sprintf(o, "UNTIL ALL G %d NEAR", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_UNTIL_P_V_I_ARRIVE:
      p_thing = &things[p_cmd->OtherThing];
      sprintf(o, "UNTIL P/V/I %d ARRIVES X%d Y%d Z%d",
        p_thing->U.UPerson.UniqueID, p_cmd->X, p_cmd->Y, p_cmd->Z);
      result = 1;
      break;
    case PCmd_UNTIL_MEM_G_ARRIVE:
      sprintf(o, "UNTIL %d MEM G %d ARRIVE", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_UNTIL_P_PERSUADE:
      p_thing = &things[p_cmd->OtherThing];
      sprintf(o, "%s %d", cmd_name, p_thing->U.UPerson.UniqueID);
      result = 1;
      break;
    case PCmd_UNTIL_MEM_G_PERSUADE:
      sprintf(o, "UNTIL %d MEM G %d PERSUADED", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_UNTIL_OBJT_DESTROY:
      sprintf(o, "%s %d X%d Y%d Z%d", cmd_name, p_cmd->OtherThing, p_cmd->X, p_cmd->Y, p_cmd->Z);
      result = 1;
      break;
    case PCmd_UNTIL_TIME:
      sprintf(o, "%s %d", cmd_name, p_cmd->Time);
      result = 1;
      break;
    case PCmd_WAIT_OBJV:
    case PCmd_WAND_OBJV:
    case PCmd_UNTIL_OBJV:
      sprintf(o, "%s", cmd_name);
      result = 1;
      break;
    case PCmd_WITHIN_AREA:
      sprintf(o, "%s X%d Y%d Z%d", cmd_name, p_cmd->X, p_cmd->Y, p_cmd->Z);
      result = 1;
      break;
    case PCmd_WITHIN_OFF:
    case PCmd_HARD_AS_AGENT:
    case PCmd_START_DANGER_MUSIC:
    case PCmd_IGNORE_ENEMIES:
      sprintf(o, "%s", cmd_name);
      result = 1;
      break;
    case PCmd_LOCK_BUILDN:
    case PCmd_UNLOCK_BUILDN:
      sprintf(o, "%s %d", cmd_name, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_UNTIL_G_NOT_SEEN:
      sprintf(o, "UNTIL G %d NOT SEEN", p_cmd->Time);
      result = 1;
      break;
    case PCmd_PING_P_V:
      p_thing = &things[p_cmd->OtherThing];
      sprintf(o, "PING P/V %d", p_thing->U.UPerson.UniqueID);
      result = 1;
      break;
    case PCmd_CAMERA_TRACK:
      p_thing = &things[p_cmd->OtherThing];
      sprintf(o, "%s %d", cmd_name, p_thing->U.UPerson.UniqueID);
      result = 1;
      break;
    case PCmd_UNTRUCE_GROUP:
      sprintf(o, "%s %d", cmd_name, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_PLAY_SAMPLE:
      sprintf(o, "%s %d", cmd_name, p_cmd->OtherThing);
      result = 1;
      break;
    default:
      result = 0;
      break;
    }
    return result;
}

/******************************************************************************/
