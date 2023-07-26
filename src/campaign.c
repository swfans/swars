/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file campaign.c
 *     Routines campaign progress and missions definitions.
 * @par Purpose:
 *     Implement general functions for handling campaign progress.
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
#include "campaign.h"

#include "bffile.h"
#include "bfmemory.h"
#include "bfmemut.h"
#include "game_data.h"
#include "game.h"
#include "swlog.h"
/******************************************************************************/

enum ObjectiveDefFlags {
    ObDF_None = 0x0000,
    // What to place in Thing/UniqueId
    ObDF_ReqGroup = 0x0001,
    ObDF_ReqThing = 0x0002, // May use Thing on UniqueId, depending on LevelNo and type; so set both
    ObDF_ReqCount = 0x0004,
    // What to place in Coord/Radius
    ObDF_ReqCoord = 0x0010,
    ObDF_ReqRadius = 0x0020,
    // What to place in Arg2
    ObDF_ReqAmount = 0x0040,
    ObDF_ReqSecTng = 0x0080, // TODO this can cause bugs, use only Thing for thing index
    ObDF_ReqSecGrp = 0x0100,
};

struct ObjectiveDef {
    const char *CmdName;
    const char *DefText;
    ushort Flags;
};

struct ObjectiveDef objectv_defs[] = {
    /* Unreachable. */
    {"GAME_OBJ_NONE",		"DO NOTHING",		ObDF_None },
    /* Require the target person to reach DEAD state. */
    {"GAME_OBJ_P_DEAD",		"ASSASSINATE",		ObDF_ReqThing },
    /* Require whole group to be neutralized. */
    {"GAME_OBJ_ALL_G_DEAD",	"ELIMINATE GROUP",	ObDF_ReqGroup },
    /* Require at least specified amount of group members to reach DEAD state. */
    {"GAME_OBJ_MEM_G_DEAD",	"KILL GROUP MEM",	ObDF_ReqGroup|ObDF_ReqAmount },
    /* Unreachable. Require person near? */
    {"GAME_OBJ_P_NEAR",		"RENDEZVOUS",		ObDF_ReqThing|ObDF_ReqCoord|ObDF_ReqRadius },
    /* Unreachable. Require specified amount of group members near? */
    {"GAME_OBJ_MEM_G_NEAR",	"RENDEZVOUS2",		ObDF_ReqGroup|ObDF_ReqCoord|ObDF_ReqRadius },
    /* Require the target person to be within given radius around given coordinates. */
    {"GAME_OBJ_P_ARRIVES",	"GOTO LOCATION",	ObDF_ReqThing|ObDF_ReqCoord|ObDF_ReqRadius },
    /* Require at least specified amount of group members to be within radius around given coords. */
    {"GAME_OBJ_MEM_G_ARRIVES", "GOTO LOCATION", ObDF_ReqGroup|ObDF_ReqCoord|ObDF_ReqRadius|ObDF_ReqAmount },
    /* Require all of group members to be within radius around given coords. */
    {"GAME_OBJ_ALL_G_ARRIVES", "ALL GOTO LOCATION", ObDF_ReqGroup|ObDF_ReqCoord|ObDF_ReqRadius },
    /* Require target person to be within the group belonging to local player. */
    {"GAME_OBJ_PERSUADE_P",	"PERSUADE",			ObDF_ReqThing },
    /* Require at least specified amount of group members to be within the local player group. */
    {"GAME_OBJ_PERSUADE_MEM_G", "PERSUADE GANG MEM", ObDF_ReqGroup|ObDF_ReqAmount },
    /* Require all of group members to be persuaded. */
    {"GAME_OBJ_PERSUADE_ALL_G", "PERSUADE ALL GANG", ObDF_ReqGroup },
    /* Require specified amount of game turns to pass. */
    {"GAME_OBJ_TIME",		"TIMER",			ObDF_ReqCount },
    /* Require specified carried item to change owner to a person belonging to local player. */
    {"GAME_OBJ_GET_ITEM",	"COLLECT ITEM",		ObDF_ReqThing },
    /* Unreachable. Require specified item to be used? */
    {"GAME_OBJ_USE_ITEM",	"USE ITEM",			ObDF_ReqThing },
    /* Unreachable. Require acquiring specified amount of funds? */
    {"GAME_OBJ_FUNDS",		"GET BULLION",		ObDF_None },
    /* Require given thing to have DESTROYED flag set.
     * Coords need to be provided, rather than UniqueId, to find the object thing in case it changed. */
    {"GAME_OBJ_DESTROY_OBJECT", "DESTROY BUILDING", ObDF_ReqThing|ObDF_ReqCoord },
    /** Require the target person to either be DESTROYED or change owner to local player group. */
    {"GAME_OBJ_PKILL_P",	"NEUTRALISE",		ObDF_ReqThing },
    /* Require FIRST group member to either be DESTROYED or change owner to local player group.
      Suspicious implementation, looks like copy-paste error. */
    {"GAME_OBJ_PKILL_G",	"NEUTRALISE MEM",	ObDF_ReqGroup|ObDF_ReqAmount },
    /* Require all of group members to either be DESTROYED or change owner to local player group. */
    {"GAME_OBJ_PKILL_ALL_G", "NEUTRALISE G",	ObDF_ReqGroup },
    /* Unreachable. Require using P.A. Network? */
    {"GAME_OBJ_USE_PANET",	"USE P.A.NET",		ObDF_None },
    /* Unreachable. */
    {"GAME_OBJ_UNUSED_21",	"UNEXPECT 21",		ObDF_None },
    /* Fail if any of group members reach DEAD state. The only negative objective. */
    {"GAME_OBJ_PROTECT_G",	"PROTECT GROUP",	ObDF_ReqGroup },
    /* Require all of group members to change owner to specified person. */
    {"GAME_OBJ_P_PERS_G",	"PEEP PERSUADE ALL", ObDF_ReqGroup|ObDF_ReqSecTng },
    /* Require all of group members to either be DEAD or within specified vehicle. */
    {"GAME_OBJ_ALL_G_USE_V", "USE VEHICLE",		ObDF_ReqThing|ObDF_ReqSecGrp },
    /* Unreachable. */
    {"GAME_OBJ_UNUSED_25",	"UNEXPECT 25",		ObDF_None },
    /* Unreachable. */
    {"GAME_OBJ_UNUSED_26",	"UNEXPECT 26",		ObDF_None },
};

/* deprecated */
const char *gameobjctv_names[] = {
    "GAME_OBJ_NONE",
    "GAME_OBJ_P_DEAD",
    "GAME_OBJ_ALL_G_DEAD",
    "GAME_OBJ_MEM_G_DEAD",
    "GAME_OBJ_P_NEAR",
    "GAME_OBJ_MEM_G_NEAR",
    "GAME_OBJ_P_ARRIVES",
    "GAME_OBJ_MEM_G_ARRIVES",
    "GAME_OBJ_ALL_G_ARRIVES",
    "GAME_OBJ_PERSUADE_P",
    "GAME_OBJ_PERSUADE_MEM_G",
    "GAME_OBJ_PERSUADE_ALL_G",
    "GAME_OBJ_TIME",
    "GAME_OBJ_GET_ITEM",
    "GAME_OBJ_USE_ITEM",
    "GAME_OBJ_FUNDS",
    "GAME_OBJ_DESTROY_OBJECT",
    "GAME_OBJ_PKILL_P",
    "GAME_OBJ_PKILL_G",
    "GAME_OBJ_PKILL_ALL_G",
    "GAME_OBJ_USE_PANET",
    "GAME_OBJ_UNUSED_21",
    "GAME_OBJ_PROTECT_G",
    "GAME_OBJ_P_PERS_G",
    "GAME_OBJ_ALL_G_USE_V",
    "GAME_OBJ_UNUSED_25",
    "GAME_OBJ_UNUSED_26",
};

extern ushort mission_strings_len;
extern ushort display_mode;

void load_missions(int num)
{
#if 0
    asm volatile ("call ASM_load_missions\n"
        : : "a" (num));
#else
    TbFileHandle fh;
    char locstr[52];
    ulong fmtver;
    int i;

    fmtver = 1;
    sprintf(locstr, "%s/all%03d.mis", game_dirs[DirPlace_Levels].directory, num);
    fh = LbFileOpen(locstr, Lb_FILE_MODE_READ_ONLY);
    if (fh != INVALID_FILE)
    {
        LbFileRead(fh, &fmtver, sizeof(ulong));
        LbFileRead(fh, &mission_strings_len, sizeof(ushort));
        LbFileRead(fh, engine_mem_alloc_ptr + engine_mem_alloc_size - 64000, mission_strings_len);
        LbFileRead(fh, &next_mission, sizeof(ushort));
        LbFileRead(fh, mission_list, sizeof(struct Mission) * next_mission);
        LbFileRead(fh, &next_used_objective, sizeof(ushort));
        LbFileRead(fh, game_used_objectives, sizeof(struct Objective) * next_used_objective);
        if (fmtver > 30)
            LbFileRead(fh, engine_mem_alloc_ptr + engine_mem_alloc_size - 1320 - 33, 1320);
        LbFileClose(fh);
        display_mode = 0;
    }
    else
    {
        LOGERR("Missions file could not be opened");
    }

    { // Fill mission names from strings block
        char *s;
        char *s_end;

        s = engine_mem_alloc_ptr + engine_mem_alloc_size - 64000;
        s_end = s + mission_strings_len;
        for (i = 1; i < next_mission; i++) {
            if (s >= s_end) break;
            mission_list[i].TextName = s;
            s += strlen(s) + 1;
        }
    }

    if (!in_network_game)
        LbMemorySet(mission_status, 0, 120 * sizeof(struct MissionStatus));
    if (fmtver < 2)
    {
        for (i = 1; i < next_mission; i++)
          mission_list[i].ReLevelNo = 0;
    }
    for (i = 1; i < next_mission; i++)
        mission_list[i].Complete = 0;
#endif
}

void save_objective_chain_conf(TbFileHandle fh, ushort objectv_head, char *buf)
{
    ushort objectv;
    char *s;
    ubyte nparams;

    objectv = 0;
    while (objectv != objectv_head)
    {
        struct Objective *p_objectv;
        struct ObjectiveDef *p_odef;

        { // Go backwards through single-directional chain
            ushort nxobjectv;
            nxobjectv = objectv_head;
            while (game_used_objectives[nxobjectv].Next != objectv)
                nxobjectv = game_used_objectives[nxobjectv].Next;
            objectv = nxobjectv;
        }

        p_objectv = &game_used_objectives[objectv];
        p_odef = &objectv_defs[p_objectv->Type];
        s = buf;

        sprintf(s, "P%02d = %s( ", (int)p_objectv->Pri, p_odef->CmdName);
        s += strlen(s);
        nparams = 0;

        if ((p_odef->Flags & ObDF_ReqGroup) != 0) {
            if (nparams) { sprintf(s, ", "); s += strlen(s); }
            sprintf(s, "Group(%hd)", p_objectv->Thing);
            s += strlen(s);
            nparams++;
        } else if ((p_odef->Flags & ObDF_ReqCount) != 0) {
            if (nparams) { sprintf(s, ", "); s += strlen(s); }
            sprintf(s, "Count(%hd)", p_objectv->Thing);
            s += strlen(s);
            nparams++;
        } else if (((p_odef->Flags & ObDF_ReqThing) != 0) ||
          (p_objectv->Thing != 0) || (p_objectv->UniqueID != 0)) {
            if (nparams) { sprintf(s, ", "); s += strlen(s); }
            sprintf(s, "Thing(%hd,%hu)", p_objectv->Thing, p_objectv->UniqueID);
            s += strlen(s);
            nparams++;
        }
        // Unexpected to have unique id withiut thing
        if (((p_odef->Flags & ObDF_ReqThing) == 0) && (p_objectv->UniqueID != 0)) {
            if (nparams) { sprintf(s, ", "); s += strlen(s); }
            sprintf(s, "UniqueID(%hu)", p_objectv->UniqueID);
            s += strlen(s);
        }

        if (((p_odef->Flags & ObDF_ReqCoord) != 0) ||
          (p_objectv->X|p_objectv->Y|p_objectv->Z) != 0) {
            if (nparams) { sprintf(s, ", "); s += strlen(s); }
            sprintf(s, "Coord(%hd,%hd,%hd)", p_objectv->X, p_objectv->Y, p_objectv->Z);
            s += strlen(s);
            nparams++;
        }
        if (((p_odef->Flags & ObDF_ReqRadius) != 0) ||
          (p_objectv->Radius != 0)) {
            if (nparams) { sprintf(s, ", "); s += strlen(s); }
            sprintf(s, "Radius(%hd)", p_objectv->Radius);
            s += strlen(s);
            nparams++;
        }
        if ((p_odef->Flags & ObDF_ReqAmount) != 0) {
            if (nparams) { sprintf(s, ", "); s += strlen(s); }
            sprintf(s, "Amount(%d)", (int)p_objectv->Arg2);
            s += strlen(s);
        } else if ((p_odef->Flags & ObDF_ReqSecGrp) != 0) {
            if (nparams) { sprintf(s, ", "); s += strlen(s); }
            sprintf(s, "SecGroup(%d)", (int)p_objectv->Arg2);
            s += strlen(s);
        } else if ((p_odef->Flags & ObDF_ReqSecTng) != 0) {
            // TODO storing a ThingOffset in 8-bit value is risky; when possible, switch Arg2 with Thing
            if (nparams) { sprintf(s, ", "); s += strlen(s); }
            sprintf(s, "SecThing(%d)", (int)p_objectv->Arg2);
            s += strlen(s);
        } else if (p_objectv->Arg2 != 0) {
            if (nparams) { sprintf(s, ", "); s += strlen(s); }
            sprintf(s, "Arg2(%d)", (int)p_objectv->Arg2);
            s += strlen(s);
        }
        if (p_objectv->StringIndex != 0) {
            if (nparams) { sprintf(s, ", "); s += strlen(s); }
            sprintf(s, "StringIndex(%hu)", p_objectv->StringIndex);
            s += strlen(s);
        }
        if (p_objectv->Flags != 0) {
            if (nparams) { sprintf(s, ", "); s += strlen(s); }
            sprintf(s, "Flags(%hu)", p_objectv->Flags);
            s += strlen(s);
            nparams++;
        }
        if (p_objectv->ObjText != 0) {
            if (nparams) { sprintf(s, ", "); s += strlen(s); }
            sprintf(s, "TextId(%d)", (int)p_objectv->ObjText);
            s += strlen(s);
        }
        if ((p_objectv->field_1B[0]|p_objectv->field_1B[1]|p_objectv->field_1B[2]|p_objectv->field_1B[3]) != 0) {
            if (nparams) { sprintf(s, ", "); s += strlen(s); }
            sprintf(s, "field_1B(%d,%d,%d,%d)", (int)p_objectv->field_1B[0],
              (int)p_objectv->field_1B[1], (int)p_objectv->field_1B[2], (int)p_objectv->field_1B[3]);
            s += strlen(s);
        }
        if (p_objectv->field_1F != 0) {
            if (nparams) { sprintf(s, ", "); s += strlen(s); }
            sprintf(s, "field_1F(%d)", (int)p_objectv->field_1F);
            s += strlen(s);
        }

        sprintf(s, " )\n");
        s += strlen(s);

        LbFileWrite(fh, buf, s - buf);
    }
}

void save_mission_single_conf(TbFileHandle fh, struct Mission *p_missi, char *buf)
{
    if (p_missi->TextName == NULL) {
        sprintf(buf, "Name = MAP%03d - LEVEL%03d\n", (int)p_missi->MapNo, (int)p_missi->LevelNo);
    } else {
        sprintf(buf, "Name = %s\n", p_missi->TextName);
    }
    LbFileWrite(fh, buf, strlen(buf));
    if (p_missi->TextId != 0) {
        sprintf(buf, "TextId = %hu\n", p_missi->TextId);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if (p_missi->SpecialEffectID != 0) {
        sprintf(buf, "SpecialEffectID = %hu\n", p_missi->SpecialEffectID);
        LbFileWrite(fh, buf, strlen(buf));
    }
    {
        sprintf(buf, "SourceID = %hu\n", p_missi->SourceID);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if (p_missi->SuccessID != 0) {
        sprintf(buf, "SuccessID = %hu\n", p_missi->SuccessID);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if (p_missi->FailID != 0) {
        sprintf(buf, "FailID = %hu\n", p_missi->FailID);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if ((p_missi->SpecialTrigger[0]|p_missi->SpecialTrigger[1]|p_missi->SpecialTrigger[2]) != 0) {
        sprintf(buf, "SpecialTrigger = %d %d %d\n",
          (int)p_missi->SpecialTrigger[0], (int)p_missi->SpecialTrigger[1], (int)p_missi->SpecialTrigger[2]);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if ((p_missi->SuccessTrigger[0]|p_missi->SuccessTrigger[1]|p_missi->SuccessTrigger[2]) != 0) {
        sprintf(buf, "SuccessTrigger = %d %d %d\n",
          (int)p_missi->SuccessTrigger[0], (int)p_missi->SuccessTrigger[1], (int)p_missi->SuccessTrigger[2]);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if ((p_missi->FailTrigger[0]|p_missi->FailTrigger[1]|p_missi->FailTrigger[2]) != 0) {
        sprintf(buf, "FailTrigger = %d %d %d\n",
          (int)p_missi->FailTrigger[0], (int)p_missi->FailTrigger[1], (int)p_missi->FailTrigger[2]);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if (p_missi->BankTest != 0) {
        sprintf(buf, "BankTest = %d\n", (int)p_missi->BankTest);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if (p_missi->SpecialEffectFailID != 0) {
        sprintf(buf, "SpecialEffectFailID = %hu\n", p_missi->SpecialEffectFailID);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if (p_missi->SpecialEffectSuccessID != 0) {
        sprintf(buf, "SpecialEffectSuccessID = %hu\n", p_missi->SpecialEffectSuccessID);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if (p_missi->StringIndex != 0) {
        sprintf(buf, "StringIndex = %hu\n", p_missi->StringIndex);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if ((p_missi->StartMap[0]|p_missi->StartMap[1]|p_missi->StartMap[2]) != 0) {
        sprintf(buf, "StartMap = %d %d %d\n",
          (int)p_missi->StartMap[0], (int)p_missi->StartMap[1], (int)p_missi->StartMap[2]);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if ((p_missi->StartLevel[0]|p_missi->StartLevel[1]|p_missi->StartLevel[2]) != 0) {
        sprintf(buf, "StartLevel = %d %d %d\n",
          (int)p_missi->StartLevel[0], (int)p_missi->StartLevel[1], (int)p_missi->StartLevel[2]);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if ((p_missi->SuccessMap[0]|p_missi->SuccessMap[1]|p_missi->SuccessMap[2]) != 0) {
        sprintf(buf, "SuccessMap = %d %d %d\n",
          (int)p_missi->SuccessMap[0], (int)p_missi->SuccessMap[1], (int)p_missi->SuccessMap[2]);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if ((p_missi->SuccessLevel[0]|p_missi->SuccessLevel[1]|p_missi->SuccessLevel[2]) != 0) {
        sprintf(buf, "SuccessLevel = %d %d %d\n",
          (int)p_missi->SuccessLevel[0], (int)p_missi->SuccessLevel[1], (int)p_missi->SuccessLevel[2]);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if ((p_missi->FailMap[0]|p_missi->FailMap[1]|p_missi->FailMap[2]) != 0) {
        sprintf(buf, "FailMap = %d %d %d\n",
          (int)p_missi->FailMap[0], (int)p_missi->FailMap[1], (int)p_missi->FailMap[2]);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if ((p_missi->FailLevel[0]|p_missi->FailLevel[1]|p_missi->FailLevel[2]) != 0) {
        sprintf(buf, "FailLevel = %d %d %d\n",
          (int)p_missi->FailLevel[0], (int)p_missi->FailLevel[1], (int)p_missi->FailLevel[2]);
        LbFileWrite(fh, buf, strlen(buf));
    }
    {
        sprintf(buf, "MapNo = %d\n", (int)p_missi->MapNo);
        LbFileWrite(fh, buf, strlen(buf));
    }
    {
        sprintf(buf, "LevelNo = %d\n", (int)p_missi->LevelNo);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if (p_missi->BankTestFail != 0) {
        sprintf(buf, "BankTestFail = %d\n", (int)p_missi->BankTestFail);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if (p_missi->Complete != 0) {
        sprintf(buf, "Complete = %d\n", (int)p_missi->Complete);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if ((p_missi->MissionCond[0]|p_missi->MissionCond[1]|p_missi->MissionCond[2]|p_missi->MissionCond[3]|p_missi->MissionCond[4]) != 0) {
        sprintf(buf, "MissionCond = %d %d %d %d %d\n", (int)p_missi->MissionCond[0],
          (int)p_missi->MissionCond[1], (int)p_missi->MissionCond[2],
          (int)p_missi->MissionCond[3], (int)p_missi->MissionCond[4]);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if (p_missi->ReLevelNo != 0) {
        sprintf(buf, "ReLevelNo = %d\n", (int)p_missi->ReLevelNo);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if (p_missi->CashReward != 0) {
        sprintf(buf, "CashReward = %d\n", (int)p_missi->CashReward);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if (p_missi->PANStart != 0) {
        sprintf(buf, "PANStart = %d\n", (int)p_missi->PANStart);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if (p_missi->PANEnd != 0) {
        sprintf(buf, "PANEnd = %d\n", (int)p_missi->PANEnd);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if (p_missi->WaitToFade != 0) {
        sprintf(buf, "WaitToFade = %hu\n", p_missi->WaitToFade);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if (p_missi->PreProcess != 0) {
      sprintf(buf, "PreProcess = %hu\n", p_missi->PreProcess);
      LbFileWrite(fh, buf, strlen(buf));
    }
    if ((p_missi->field_48[0]|p_missi->field_48[1]|p_missi->field_48[2]) != 0) {
        sprintf(buf, "field_48 = %d %d %d\n", (int)p_missi->field_48[0],
          (int)p_missi->field_48[1], (int)p_missi->field_48[2]);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if (p_missi->field_4B != 0) {
      sprintf(buf, "field_4B = %d\n", (int)p_missi->field_4B);
      LbFileWrite(fh, buf, strlen(buf));
    }
}

void save_missions_conf(int num)
{
    TbFileHandle fh;
    char locbuf[120];
    int i;

    sprintf(locbuf, "%s/miss%03d.ini", "conf", num);
    fh = LbFileOpen(locbuf, Lb_FILE_MODE_NEW);
    if (fh == INVALID_FILE) {
        LOGERR("Missions config file could not be created");
        return;
    }

    {
        sprintf(locbuf, "[common]\n");
        LbFileWrite(fh, locbuf, strlen(locbuf));

        sprintf(locbuf, "MissionsCount = %hu\n", next_mission);
        LbFileWrite(fh, locbuf, strlen(locbuf));

        sprintf(locbuf, "\n");
        LbFileWrite(fh, locbuf, strlen(locbuf));
    }

    for (i = 0; i < next_mission; i++) {
        struct Mission *p_missi;

        p_missi = &mission_list[i];

        sprintf(locbuf, "[mission%d]\n", i);
        LbFileWrite(fh, locbuf, strlen(locbuf));

        save_mission_single_conf(fh, p_missi, locbuf);

        sprintf(locbuf, "\n");
        LbFileWrite(fh, locbuf, strlen(locbuf));

        if (p_missi->StartHead != 0) {
            sprintf(locbuf, "[misstart%d]\n", i);
            LbFileWrite(fh, locbuf, strlen(locbuf));

            save_objective_chain_conf(fh, p_missi->StartHead, locbuf);

            sprintf(locbuf, "\n");
            LbFileWrite(fh, locbuf, strlen(locbuf));
        }

        if (p_missi->SuccessHead != 0) {
            sprintf(locbuf, "[missuccess%d]\n", i);
            LbFileWrite(fh, locbuf, strlen(locbuf));

            save_objective_chain_conf(fh, p_missi->SuccessHead, locbuf);

            sprintf(locbuf, "\n");
            LbFileWrite(fh, locbuf, strlen(locbuf));
        }

        if (p_missi->FailHead != 0) {
            sprintf(locbuf, "[misfail%d]\n", i);
            LbFileWrite(fh, locbuf, strlen(locbuf));

            save_objective_chain_conf(fh, p_missi->FailHead, locbuf);

            sprintf(locbuf, "\n");
            LbFileWrite(fh, locbuf, strlen(locbuf));
        }
    }
    LbFileClose(fh);
}

/******************************************************************************/
