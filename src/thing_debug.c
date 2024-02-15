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
#include "command.h"
#include "weapon.h"
#include "swlog.h"
/******************************************************************************/

TbBool person_command_to_text(char *out, ushort cmd, ubyte a3)
{
    struct Command *p_cmd;
    struct CommandDef *p_cdef;
    const char *cmd_name;
    char *o;
    TbBool result;

    o = out;
    if (a3 == 1)
        return a3 ^ 1;

    p_cmd = &game_commands[cmd];

    if (p_cmd->Flags & 0x01) {
        sprintf(o, "%d ", cmd);
        o += strlen(o);
    }
    if (p_cmd->Flags & 0x02) {
        sprintf(o, "U ");
        o += strlen(o);
    }
    if ( p_cmd->Flags & 0x04) {
        sprintf(o, "IU ");
        o += strlen(o);
    }
    if ( p_cmd->Flags & 0x08) {
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
      sprintf(o, "GO TO POINT X%d Y%d Z%d", p_cmd->X, p_cmd->Y, p_cmd->Z);
      result = 1;
      break;
    case PCmd_GO_TO_PERSON:
      sprintf(o, "GO TO PERSON %d", things[p_cmd->OtherThing].U.UMGun.UniqueID);
      result = 1;
      break;
    case PCmd_KILL_PERSON:
      sprintf(o, "KILL PERSON %d", things[p_cmd->OtherThing].U.UMGun.UniqueID);
      result = 1;
      break;
    case PCmd_KILL_MEM_GROUP:
      sprintf(o, "KILL %d MEM GROUP %d", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_KILL_ALL_GROUP:
      sprintf(o, "KILL ALL GROUP %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_PERSUADE_PERSON:
      sprintf(o, "PERSUADE PERSON %d", things[p_cmd->OtherThing].U.UMGun.UniqueID);
      result = 1;
      break;
    case PCmd_PERSUADE_MEM_GROUP:
      sprintf(o, "PERSUADE %d Mem G %d", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_PERSUADE_ALL_GROUP:
      sprintf(o, "PERSUADE All G %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_BLOCK_PERSON:
      sprintf(o, "BLOCK PERSON %d", things[p_cmd->OtherThing].U.UMGun.UniqueID);
      result = 1;
      break;
    case PCmd_SCARE_PERSON:
      sprintf(o, "SCARE PERSON %d", things[p_cmd->OtherThing].U.UMGun.UniqueID);
      result = 1;
      break;
    case PCmd_FOLLOW_PERSON:
      sprintf(o, "FOLLOW PERSON %d", things[p_cmd->OtherThing].U.UMGun.UniqueID);
      result = 1;
      break;
    case PCmd_SUPPORT_PERSON:
      sprintf(o, "SUPPORT PERSON %d", things[p_cmd->OtherThing].U.UMGun.UniqueID);
      result = 1;
      break;
    case PCmd_PROTECT_PERSON:
      sprintf(o, "PROTECT PERSON %d idx %d",
        things[p_cmd->OtherThing].U.UMGun.UniqueID, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_HIDE:
      sprintf(o, "HIDE");
      result = 1;
      break;
    case PCmd_GET_ITEM:
      sprintf(o, "GET ITEM %d", (unsigned __int16)sthings[p_cmd->OtherThing].UniqueID);
      result = 1;
      break;
    case PCmd_USE_WEAPON:
      sprintf(o, "USE WEAPON %s", weapon_codename(p_cmd->OtherThing));
      result = 1;
      break;
    case PCmd_DROP_SPEC_ITEM:
      sprintf(o, "DROP SPEC ITEM %d", sthings[p_cmd->OtherThing].UniqueID);
      result = 1;
      break;
    case PCmd_AVOID_PERSON:
      sprintf(o, "AVOID PERSON %d", things[p_cmd->OtherThing].U.UMGun.UniqueID);
      result = 1;
      break;
    case PCmd_WAND_AVOID_GROUP:
      sprintf(o, "WAND AVOID GROUP %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_DESTROY_BUILDING:
      sprintf(o, "DESTROY BUILDING %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_UNKN16:
      result = 1;
      break;
    case PCmd_USE_VEHICLE:
      sprintf(o, "USE VEHICLE %d", things[p_cmd->OtherThing].U.UMGun.UniqueID);
      result = 1;
      break;
    case PCmd_EXIT_VEHICLE:
      sprintf(o, "EXIT VEHICLE");
      result = 1;
      break;
    case PCmd_CATCH_TRAIN:
      sprintf(o, "CATCH TRAIN %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_OPEN_DOME:
      sprintf(o, "OPEN DOME %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_CLOSE_DOME:
      sprintf(o, "CLOSE DOME %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_DROP_WEAPON:
      sprintf(o, "DROP WEAPON %s", weapon_codename(p_cmd->OtherThing));
      result = 1;
      break;
    case PCmd_CATCH_FERRY:
      sprintf(o, "CATCH FERRY %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_EXIT_FERRY:
      sprintf(o, "EXIT FERRY %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_PING_EXIST:
      sprintf(o, "PING EXIST");
      result = 1;
      break;
    case PCmd_GOTOPOINT_FACE:
      sprintf(o, "GOTOPOINT FACE %d X%d Z%d",
        p_cmd->OtherThing, p_cmd->X, p_cmd->Z);
      result = 1;
      break;
    case PCmd_SELF_DESTRUCT:
      sprintf(o, "SELF_DESTRUCT");
      result = 1;
      break;
    case PCmd_PROTECT_MEM_G:
      sprintf(o, "PROTECT MEM G %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_RUN_TO_POINT:
      sprintf(o, "RUN TO POINT X%d Z%d", p_cmd->X, p_cmd->Z);
      result = 1;
      break;
    case PCmd_KILL_EVERYONE:
      sprintf(o, "KILL EVERYONE");
      result = 1;
      break;
    case PCmd_GUARD_OFF:
      sprintf(o, "GUARD OFF");
      result = 1;
      break;
    case PCmd_EXECUTE_COMS:
      sprintf(o, "Execute Coms (plyr)");
      result = 1;
      break;
    case PCmd_WAIT_P_V_DEAD:
      sprintf(o, "WAIT P/V DEAD %d", things[p_cmd->OtherThing].U.UMGun.UniqueID);
      result = 1;
      break;
    case PCmd_WAIT_MEM_G_DEAD:
      sprintf(o, "WAIT %d MEM G DEAD %d", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAIT_ALL_G_DEAD:
      sprintf(o, "WAIT ALL G DEAD %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAIT_P_V_I_NEAR:
      sprintf(o, "WAIT P/V/I NEAR %d", things[p_cmd->OtherThing].U.UMGun.UniqueID);
      result = 1;
      break;
    case PCmd_WAIT_MEM_G_NEAR:
      sprintf(o, "WAIT %d MEM G %d NEAR ", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAIT_ALL_G_NEAR:
      sprintf(o, "WAIT ALL G %d NEAR ", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAIT_P_V_I_ARRIVE:
      sprintf(o, "WAIT P/V/I ARRIVES %d X%d Y%d Z%d",
        things[p_cmd->OtherThing].U.UMGun.UniqueID, p_cmd->X, p_cmd->Y, p_cmd->Z);
      result = 1;
      break;
    case PCmd_WAIT_MEM_G_ARRIVE:
      sprintf(o, "WAIT %d MEM G ARRIVE %d", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAIT_ALL_G_ARRIVE:
      sprintf(o, "WAIT ALL G ARRIVE %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAIT_P_PERSUADE:
      sprintf(o, "WAIT P PERSUADED %d", things[p_cmd->OtherThing].U.UMGun.UniqueID);
      result = 1;
      break;
    case PCmd_WAIT_MEM_G_PERSUADE:
      sprintf(o, "WAIT %d MEM G PERSUADED %d", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAIT_ALL_G_PERSUADE:
      sprintf(o, "WAIT ALL G PERSUADED %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAIT_MISSION_SUCC:
      sprintf(o, "WAIT MISSION SUCC %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAIT_MISSION_FAIL:
      sprintf(o, "WAIT MISSION FAIL %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAIT_MISSION_START:
      sprintf(o, "WAIT MISSION START %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAIT_OBJT_DESTROY:
      sprintf(o, "WAIT OBJECT DESTROYED %d X%d Y%d Z%d",
        p_cmd->OtherThing, p_cmd->X, p_cmd->Y, p_cmd->Z);
      result = 1;
      break;
    case PCmd_WAIT_TIME:
      sprintf(o, "WAIT TIME %d", p_cmd->Time);
      result = 1;
      break;
    case PCmd_WAND_P_V_DEAD:
      sprintf(o, "WAND P/V DEAD %d", things[p_cmd->OtherThing].U.UMGun.UniqueID);
      result = 1;
      break;
    case PCmd_WAND_MEM_G_DEAD:
      sprintf(o, "WAND %d MEM G %d DEAD ", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAND_ALL_G_DEAD:
      sprintf(o, "WAND ALL G DEAD %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAND_P_V_I_NEAR:
      sprintf(o, "WAND P/V/I NEAR %d", things[p_cmd->OtherThing].U.UMGun.UniqueID);
      result = 1;
      break;
    case PCmd_WAND_MEM_G_NEAR:
      sprintf(o, "WAND %d MEM G %d NEAR ", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAND_ALL_G_NEAR:
      sprintf(o, "WAND ALL G %d NEAR ", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAND_P_V_I_ARRIVE:
      sprintf(o, "WAND P/V/I ARRIVES %d X%d Y%d Z%d",
        things[p_cmd->OtherThing].U.UMGun.UniqueID, p_cmd->X, p_cmd->Y, p_cmd->Z);
      result = 1;
      break;
    case PCmd_WAND_MEM_G_ARRIVE:
      sprintf(o, "WAND %d MEM G ARRIVE %d", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAND_ALL_G_ARRIVE:
      sprintf(o, "WAND ALL G ARRIVE %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAND_P_PERSUADE:
      sprintf(o, "WAND P PERSUADED %d", things[p_cmd->OtherThing].U.UMGun.UniqueID);
      result = 1;
      break;
    case PCmd_WAND_MEM_G_PERSUADE:
      sprintf(o, "WAND %d MEM G PERSUADED %d", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAND_ALL_G_PERSUADE:
      sprintf(o, "WAND ALL G PERSUADED %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAND_MISSION_SUCC:
      sprintf(o, "WAND MISSION SUCC %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAND_MISSION_FAIL:
      sprintf(o, "WAND MISSION FAIL %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAND_MISSION_START:
      sprintf(o, "WAND MISSION START %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_WAND_OBJT_DESTROY:
      sprintf(o, "WAND OBJECT DESTROYED %d X%d Y%d Z%d",
        p_cmd->OtherThing, p_cmd->X, p_cmd->Y, p_cmd->Z);
      result = 1;
      break;
    case PCmd_WAND_TIME:
      sprintf(o, "WAND TIME %d", p_cmd->Time);
      result = 1;
      break;
    case PCmd_LOOP_COM:
      sprintf(o, "LOOP COM %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_UNTIL_P_V_DEAD:
      sprintf(o, "UNTIL P/V DEAD %d", things[p_cmd->OtherThing].U.UMGun.UniqueID);
      result = 1;
      break;
    case PCmd_UNTIL_MEM_G_DEAD:
      sprintf(o, "UNTIL %d MEM G DEAD %d", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_UNTIL_ALL_G_DEAD:
      sprintf(o, "UNTIL ALL G DEAD %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_UNTIL_P_V_I_NEAR:
      sprintf(o, "UNTIL P/V/I NEAR %d", things[p_cmd->OtherThing].U.UMGun.UniqueID);
      result = 1;
      break;
    case PCmd_UNTIL_MEM_G_NEAR:
      sprintf(o, "UNTIL %d MEM G %d NEAR ", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_UNTIL_ALL_G_NEAR:
      sprintf(o, "UNTIL ALL G %d NEAR ", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_UNTIL_P_V_I_ARRIVE:
      sprintf(o, "UNTIL P/V/I ARRIVES %d X%d Y%d Z%d",
        things[p_cmd->OtherThing].U.UMGun.UniqueID, p_cmd->X, p_cmd->Y, p_cmd->Z);
      result = 1;
      break;
    case PCmd_UNTIL_MEM_G_ARRIVE:
      sprintf(o, "UNTIL %d MEM G ARRIVE %d", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_UNTIL_ALL_G_ARRIVE:
      sprintf(o, "UNTIL ALL G ARRIVE %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_UNTIL_P_PERSUADE:
      sprintf(o, "UNTIL P PERSUADED %d", things[p_cmd->OtherThing].U.UMGun.UniqueID);
      result = 1;
      break;
    case PCmd_UNTIL_MEM_G_PERSUADE:
      sprintf(o, "UNTIL %d MEM G PERSUADED %d", p_cmd->Arg2, p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_UNTIL_ALL_G_PERSUADE:
      sprintf(o, "UNTIL ALL G PERSUADED %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_UNTIL_MISSION_SUCC:
      sprintf(o, "UNTIL MISSION SUCC %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_UNTIL_MISSION_FAIL:
      sprintf(o, "UNTIL MISSION FAIL %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_UNTIL_MISSION_START:
      sprintf(o, "UNTIL MISSION START %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_UNTIL_OBJT_DESTROY:
      sprintf(o, "UNTIL OBJECT DESTROYED %d X%d Y%d Z%d",
        p_cmd->OtherThing, p_cmd->X, p_cmd->Y, p_cmd->Z);
      result = 1;
      break;
    case PCmd_UNTIL_TIME:
      sprintf(o, "UNTIL TIME %d", p_cmd->Time);
      result = 1;
      break;
    case PCmd_WAIT_OBJV:
      sprintf(o, "WAIT OBJ:-");
      result = 1;
      break;
    case PCmd_WAND_OBJV:
      sprintf(o, "WAND OBJ:-");
      result = 1;
      break;
    case PCmd_UNTIL_OBJV:
      sprintf(o, "UNTIL OBJ:-");
      result = 1;
      break;
    case PCmd_WITHIN_AREA:
      sprintf(o, "WITHIN AREA X%d Y%d Z%d", p_cmd->X, p_cmd->Y, p_cmd->Z);
      result = 1;
      break;
    case PCmd_WITHIN_OFF:
      sprintf(o, "WITHIN OFF");
      result = 1;
      break;
    case PCmd_LOCK_BUILDN:
      sprintf(o, "LOCK BUILD %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_UNLOCK_BUILDN:
      sprintf(o, "UNLOCK BUILD %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_SELECT_WEAPON:
      sprintf(o, "SELECT WEAPON %s ", weapon_codename(p_cmd->OtherThing));
      result = 1;
      break;
    case PCmd_HARD_AS_AGENT:
      sprintf(o, "HARD AS AGENT");
      result = 1;
      break;
    case PCmd_UNTIL_G_NOT_SEEN:
      sprintf(o, "UNTIL G %d NOT SEEN", p_cmd->Time);
      result = 1;
      break;
    case PCmd_START_DANGER_MUSIC:
      sprintf(o, "start danger music");
      result = 1;
      break;
    case PCmd_PING_P_V:
      sprintf(o, "PING P/V %d", things[p_cmd->OtherThing].U.UMGun.UniqueID);
      result = 1;
      break;
    case PCmd_CAMERA_TRACK:
      sprintf(o, "CAMERA TRACK %d", things[p_cmd->OtherThing].U.UMGun.UniqueID);
      result = 1;
      break;
    case PCmd_UNTRUCE_GROUP:
      sprintf(o, "UNTRUCE GROUP %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_PLAY_SAMPLE:
      sprintf(o, "PLAY SAMPLE %d", p_cmd->OtherThing);
      result = 1;
      break;
    case PCmd_IGNORE_ENEMIES:
      sprintf(o, "IGNORE ENEMIES");
      result = 1;
      break;
    default:
      result = 0;
      break;
    }
    return result;
}

/******************************************************************************/
