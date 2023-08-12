/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file command.c
 *     Routines for person command handling.
 * @par Purpose:
 *     Implement functions for person obeying commands.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 12 Jun 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "command.h"

#include <stdio.h>
#include <string.h>
#include "bfmemory.h"
#include "bfmemut.h"
/******************************************************************************/

enum CommandDefFlags {
    CmDF_None = 0x0000,
    /** Group index is required in OtherThing. */
    CmDF_ReqGroup = 0x0001,
    /** Person index is required in OtherThing. */
    CmDF_ReqPersonThing = 0x0002,
    /** Any Thing index is required in OtherThing. */
    CmDF_ReqOtherThing = 0x0004,
    /** A non-thing index is required in OtherThing. */
    CmDF_ReqOtherIndex = 0x0008,
    /** Thing location coordinates are required to find generic OtherThing. */
    CmDF_ReqThingCoord = 0x0010,
    /** Station location Coordinates are required to find target train OtherThing. */
    CmDF_ReqStationCoord = 0x0020,
    /** Dome location Coordinates are required to find target dome building OtherThing. */
    CmDF_ReqDomeCoord = 0x0040,
    /** Building location Coordinates are required to find target building OtherThing. */
    CmDF_ReqBuildingCoord = 0x0080,
    /** Vehicle location Coordinates are required to find the target Vehicle OtherThing. */
    CmDF_ReqVehicleCoord = 0x0100,
    /** Map coordinates are required, to be used directly during command execution. */
    CmDF_ReqCoord = 0x0200,
    /** Second set of map coordinates are required, replacing Arg2,Time,MyThing. */
    CmDF_ReqCoord2 = 0x0400,
    /** Range is required in Arg2. */
    CmDF_ReqRange2 = 0x0800,
    CmDF_ReqArgs2 = 0x1000,
    CmDF_ReqSubType = 0x010000,
    CmDF_ReqMyThing = 0x020000,
    CmDF_ReqMyRadius = 0x040000,
    /** Game turns amount required in MyThing. */
    CmDF_ReqMyTime = 0x080000,
    CmDF_ReqParent = 0x100000,
};

struct CommandDef {
    const char *CmdName;
    void *vefify;
    ulong Flags;
};

struct CommandDef command_defs[] = {
    /* Means end of command chain. */
    {"TNG_CMD_NONE",				NULL,				CmDF_None },
    {"TNG_CMD_STAY",				NULL,				CmDF_None },
    {"TNG_CMD_GO_TO_POINT",			NULL,				CmDF_ReqCoord|CmDF_ReqRange2 },
    {"TNG_CMD_GO_TO_PERSON",		NULL,				CmDF_ReqPersonThing|CmDF_ReqRange2 },
    {"TNG_CMD_KILL_PERSON",			NULL,				CmDF_ReqPersonThing },
    {"TNG_CMD_KILL_MEM_GROUP",		NULL,				CmDF_ReqGroup },
    {"TNG_CMD_KILL_ALL_GROUP",		NULL,				CmDF_ReqGroup },
    {"TNG_CMD_PERSUADE_PERSON",		NULL,				CmDF_ReqPersonThing },
    {"TNG_CMD_PERSUADE_MEM_GROUP",	NULL,				CmDF_ReqGroup },
    {"TNG_CMD_PERSUADE_ALL_GROUP",	NULL,				CmDF_ReqGroup },
    {"TNG_CMD_BLOCK_PERSON",		NULL,				CmDF_ReqPersonThing },
    {"TNG_CMD_SCARE_PERSON",		NULL,				CmDF_ReqPersonThing },
    {"TNG_CMD_FOLLOW_PERSON",		NULL,				CmDF_ReqPersonThing },
    {"TNG_CMD_SUPPORT_PERSON",		NULL,				CmDF_ReqPersonThing },
    {"TNG_CMD_PROTECT_PERSON",		NULL,				CmDF_ReqPersonThing },
    {"TNG_CMD_HIDE",				NULL,				CmDF_None },
    {"TNG_CMD_GET_ITEM",			NULL,				CmDF_ReqOtherIndex },
    {"TNG_CMD_USE_WEAPON",			NULL,				CmDF_ReqOtherIndex|CmDF_ReqCoord }, // OtherIndex = weapon
    {"TNG_CMD_DROP_SPEC_ITEM",		NULL,				CmDF_ReqOtherThing },
    {"TNG_CMD_AVOID_PERSON",		NULL,				CmDF_ReqPersonThing },
    {"TNG_CMD_WAND_AVOID_GROUP",	NULL,				CmDF_ReqGroup },
    {"TNG_CMD_DESTROY_BUILDING",	NULL,				CmDF_ReqBuildingCoord },
    {"TNG_CMD_UNKN16",				NULL,				CmDF_None },
    {"TNG_CMD_USE_VEHICLE",			NULL,				CmDF_ReqVehicleCoord },
    {"TNG_CMD_EXIT_VEHICLE",		NULL,				CmDF_None },
    {"TNG_CMD_CATCH_TRAIN",			NULL,				CmDF_ReqStationCoord },
    {"TNG_CMD_OPEN_DOME",			NULL,				CmDF_ReqDomeCoord },
    {"TNG_CMD_CLOSE_DOME",			NULL,				CmDF_ReqDomeCoord },
    {"TNG_CMD_DROP_WEAPON",			NULL,				CmDF_ReqOtherIndex }, // OtherIndex = weapon
    {"TNG_CMD_CATCH_FERRY",			NULL,				CmDF_None },
    {"TNG_CMD_EXIT_FERRY",			NULL,				CmDF_ReqOtherThing },
    {"TNG_CMD_PING_EXIST",			NULL,				CmDF_None },
    {"TNG_CMD_GOTOPOINT_FACE",		NULL,				CmDF_ReqOtherIndex|CmDF_ReqCoord|CmDF_ReqRange2 }, // OtherIndex = face
    {"TNG_CMD_SELF_DESTRUCT",		NULL,				CmDF_None },
    {"TNG_CMD_PROTECT_MEM_G",		NULL,				CmDF_ReqGroup },
    {"TNG_CMD_RUN_TO_POINT",		NULL,				CmDF_None },
    {"TNG_CMD_KILL_EVERYONE",		NULL,				CmDF_ReqCoord|CmDF_ReqCoord2 },
    {"TNG_CMD_GUARD_OFF",			NULL,				CmDF_None },
    {"TNG_CMD_EXECUTE_COMS",		NULL,				CmDF_None },
    {"TNG_CMD_UNKN27",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN28",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN29",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN2A",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN2B",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN2C",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN2D",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN2E",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN2F",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN30",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN31",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN32",				NULL,				CmDF_None },
    {"TNG_CMD_WAIT_P_V_DEAD",		NULL,				CmDF_None },
    {"TNG_CMD_WAIT_MEM_G_DEAD",		NULL,				CmDF_ReqGroup },
    {"TNG_CMD_WAIT_ALL_G_DEAD",		NULL,				CmDF_ReqGroup },
    {"TNG_CMD_WAIT_P_V_I_NEAR",		NULL,				CmDF_None },
    {"TNG_CMD_WAIT_MEM_G_NEAR",		NULL,				CmDF_ReqGroup },
    {"TNG_CMD_WAIT_ALL_G_NEAR",		NULL,				CmDF_ReqGroup },
    {"TNG_CMD_WAIT_P_V_I_ARRIVES",	NULL,				CmDF_None },
    {"TNG_CMD_WAIT_MEM_G_ARRIVE",	NULL,				CmDF_ReqGroup },
    {"TNG_CMD_WAIT_ALL_G_ARRIVE",	NULL,				CmDF_ReqGroup },
    {"TNG_CMD_WAIT_P_PERSUADED",	NULL,				CmDF_None },
    {"TNG_CMD_WAIT_MEM_G_PERSUADED",NULL,				CmDF_ReqGroup },
    {"TNG_CMD_WAIT_ALL_G_PERSUADED",NULL,				CmDF_ReqGroup },
    {"TNG_CMD_WAIT_MISSION_SUCC",	NULL,				CmDF_None },
    {"TNG_CMD_WAIT_MISSION_FAIL",	NULL,				CmDF_None },
    {"TNG_CMD_WAIT_MISSION_START",	NULL,				CmDF_None },
    {"TNG_CMD_WAIT_OBJECT_DESTROYED",NULL,				CmDF_ReqBuildingCoord },
    {"TNG_CMD_WAIT_TIME",			NULL,				CmDF_None },
    {"TNG_CMD_UNKN44",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN45",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN46",				NULL,				CmDF_None },
    {"TNG_CMD_WAND_P_V_DEAD",		NULL,				CmDF_None },
    {"TNG_CMD_WAND_MEM_G_DEAD",		NULL,				CmDF_ReqGroup },
    {"TNG_CMD_WAND_ALL_G_DEAD",		NULL,				CmDF_ReqGroup },
    {"TNG_CMD_WAND_P_V_I_NEAR",		NULL,				CmDF_None },
    {"TNG_CMD_WAND_MEM_G_NEAR",		NULL,				CmDF_ReqGroup },
    {"TNG_CMD_WAND_ALL_G_NEAR",		NULL,				CmDF_ReqGroup },
    {"TNG_CMD_WAND_P_V_I_ARRIVES",	NULL,				CmDF_None },
    {"TNG_CMD_WAND_MEM_G_ARRIVE",	NULL,				CmDF_ReqGroup },
    {"TNG_CMD_WAND_ALL_G_ARRIVE",	NULL,				CmDF_ReqGroup },
    {"TNG_CMD_WAND_P_PERSUADED",	NULL,				CmDF_None },
    {"TNG_CMD_WAND_MEM_G_PERSUADED",NULL,				CmDF_ReqGroup },
    {"TNG_CMD_WAND_ALL_G_PERSUADED",NULL,				CmDF_ReqGroup },
    {"TNG_CMD_WAND_MISSION_SUCC",	NULL,				CmDF_None },
    {"TNG_CMD_WAND_MISSION_FAIL",	NULL,				CmDF_None },
    {"TNG_CMD_WAND_MISSION_START",	NULL,				CmDF_None },
    {"TNG_CMD_WAND_OBJECT_DESTROYED",NULL,				CmDF_ReqBuildingCoord },
    {"TNG_CMD_WAND_TIME",			NULL,				CmDF_None },
    {"TNG_CMD_UNKN58",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN59",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN5A",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN5B",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN5C",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN5D",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN5E",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN5F",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN60",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN61",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN62",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN63",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN64",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN65",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN66",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN67",				NULL,				CmDF_None },
    {"TNG_CMD_ADD_STATIC",			NULL,				CmDF_ReqCoord },
    {"TNG_CMD_WAIT_TIME2",			NULL,				CmDF_ReqMyTime },
    {"TNG_CMD_UNKN6A",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN6B",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN6C",				NULL,				CmDF_None },
    {"TNG_CMD_UNKN6D",				NULL,				CmDF_None },
    {"TNG_CMD_LOOP_COM",			NULL,				CmDF_ReqOtherIndex }, // OtherIndex = command
    {"TNG_CMD_UNTIL_P_V_DEAD",		NULL,				CmDF_None },
    {"TNG_CMD_UNTIL_MEM_G_DEAD",	NULL,				CmDF_ReqGroup },
    {"TNG_CMD_UNTIL_ALL_G_DEAD",	NULL,				CmDF_ReqGroup },
    {"TNG_CMD_UNTIL_P_V_I_NEAR",	NULL,				CmDF_None },
    {"TNG_CMD_UNTIL_MEM_G_NEAR",	NULL,				CmDF_ReqGroup },
    {"TNG_CMD_UNTIL_ALL_G_NEAR",	NULL,				CmDF_ReqGroup },
    {"TNG_CMD_UNTIL_P_V_I_ARRIVES",	NULL,				CmDF_None },
    {"TNG_CMD_UNTIL_MEM_G_ARRIVE",	NULL,				CmDF_ReqGroup },
    {"TNG_CMD_UNTIL_ALL_G_ARRIVE",	NULL,				CmDF_ReqGroup },
    {"TNG_CMD_UNTIL_P_PERSUADED",	NULL,				CmDF_None },
    {"TNG_CMD_UNTIL_MEM_G_PERSUADED",NULL,				CmDF_ReqGroup },
    {"TNG_CMD_UNTIL_ALL_G_PERSUADED",NULL,				CmDF_ReqGroup },
    {"TNG_CMD_UNTIL_MISSION_SUCC",	NULL,				CmDF_None },
    {"TNG_CMD_UNTIL_MISSION_FAIL",	NULL,				CmDF_None },
    {"TNG_CMD_UNTIL_MISSION_START",	NULL,				CmDF_None },
    {"TNG_CMD_UNTIL_OBJECT_DESTROYED",NULL,				CmDF_ReqBuildingCoord },
    {"TNG_CMD_UNTIL_TIME",			NULL,				CmDF_None },
    {"TNG_CMD_WAIT_OBJ",			NULL,				CmDF_None },
    {"TNG_CMD_WAND_OBJ",			NULL,				CmDF_None },
    {"TNG_CMD_UNTIL_OBJ",			NULL,				CmDF_None },
    {"TNG_CMD_WITHIN_AREA",			NULL,				CmDF_None },
    {"TNG_CMD_WITHIN_OFF",			NULL,				CmDF_None },
    {"TNG_CMD_LOCK_BUILD",			NULL,				CmDF_ReqBuildingCoord },
    {"TNG_CMD_UNLOCK_BUILD",		NULL,				CmDF_ReqBuildingCoord },
    {"TNG_CMD_SELECT_WEAPON",		NULL,				CmDF_ReqOtherIndex }, // OtherIndex = weapon
    {"TNG_CMD_HARD_AS_AGENT",		NULL,				CmDF_None },
    {"TNG_CMD_UNTIL_G_NOT_SEEN",	NULL,				CmDF_ReqGroup },
    {"TNG_CMD_START_DANGER_MUSIC",	NULL,				CmDF_None },
    {"TNG_CMD_PING_P_V",			NULL,				CmDF_ReqOtherThing },
    {"TNG_CMD_CAMERA_TRACK",		NULL,				CmDF_ReqOtherThing },
    {"TNG_CMD_UNTRUCE_GROUP",		NULL,				CmDF_ReqGroup },
    {"TNG_CMD_PLAY_SAMPLE",			NULL,				CmDF_ReqOtherIndex }, // OtherIndex = sample
    {"TNG_CMD_IGNORE_ENEMIES",		NULL,				CmDF_None },
    {"TNG_CMD_RESET_STATE",			NULL,				CmDF_None },
    {"TNG_CMD_UNKN91",				NULL,				CmDF_ReqOtherIndex|CmDF_ReqArgs2 },
    {NULL,							NULL,				CmDF_None },
};

void snprint_command(char *buf, ulong buflen, ushort cmd)
{
    struct Command *p_cmd;
    struct CommandDef *p_cdef;

    char *s;
    ubyte nparams;

    p_cmd = &game_commands[cmd];

    s = buf;

    if (p_cmd->Type > PCmd_TYPES_COUNT) {
        sprintf(s, "%s%hu()", "INVALID", cmd);
        return;
    }

    p_cdef = &command_defs[p_cmd->Type];

    sprintf(s, "%s( ", p_cdef->CmdName);
    s += strlen(s);
    nparams = 0;

    if (((p_cdef->Flags & CmDF_ReqSubType) != 0) ||
      (p_cmd->SubType != 0)) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "SubType(%d)", (int)p_cmd->SubType);
        s += strlen(s);
        nparams++;
    }

    if (((p_cdef->Flags & CmDF_ReqCoord) != 0) ||
      (p_cmd->X|p_cmd->Y|p_cmd->Z) != 0) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "Coord(%hd,%hd,%hd)", p_cmd->X, p_cmd->Y, p_cmd->Z);
        s += strlen(s);
        nparams++;
    }

    if (((p_cdef->Flags & CmDF_ReqOtherThing) != 0) ||
      (p_cmd->OtherThing != 0)) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "Thing(%hd)", p_cmd->OtherThing);
        s += strlen(s);
        nparams++;
    }

    if (((p_cdef->Flags & CmDF_ReqArgs2) != 0) ||
      (p_cmd->Arg1 != 0) || (p_cmd->Arg2 != 0)) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "Args(%hd,%hd)", p_cmd->Arg1, p_cmd->Arg2);
        s += strlen(s);
        nparams++;
    }

    if (((p_cdef->Flags & CmDF_ReqMyThing) != 0) ||
      (p_cmd->MyThing != 0)) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "MyThing(%hd)", p_cmd->MyThing);
        s += strlen(s);
        nparams++;
    }

    if (((p_cdef->Flags & CmDF_ReqParent) != 0) ||
      (p_cmd->Parent != 0)) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "Parent(%hd)", p_cmd->Parent);
        s += strlen(s);
        nparams++;
    }

    {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "Time(%hd)", p_cmd->Time);
        s += strlen(s);
        nparams++;
    }

    {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "Flags(0x%02lX)", p_cmd->Flags);
        s += strlen(s);
        nparams++;
    }

    if ((p_cmd->field_1A != 0) || (p_cmd->field_1E != 0)) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "Padding(%ld,%hd)", p_cmd->field_1A, p_cmd->field_1E);
        s += strlen(s);
        nparams++;
    }


    snprintf(s, buflen - (s-buf), " )");
}

/******************************************************************************/
