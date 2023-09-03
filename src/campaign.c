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
#include "bfini.h"
#include "cybmod.h"
#include "weapon.h"
#include "lvobjctv.h"
#include "game_data.h"
#include "game.h"
#include "swlog.h"
/******************************************************************************/

enum MissionListConfigCmd {
    MissL_MissionsCount = 1,
    MissL_TextName,
    MissL_TextId,
    MissL_FirstTrigger,
    MissL_NetscanTextId,
    MissL_WeaponsTextIdShift,
    MissL_ResearchWeapons,
    MissL_StandardWeapons,
    MissL_ResearchMods,
    MissL_StandardMods,
    MissL_ProjectorFnMk,
    MissL_OutroFMV,
    MissL_OutroBkFn,
    MissL_Selectable,
    MissL_Type,
    MissL_SpecialEffectID,
    MissL_SourceID,
    MissL_SuccessID,
    MissL_FailID,
    MissL_SpecialTrigger,
    MissL_SuccessTrigger,
    MissL_FailTrigger,
    MissL_BankTest,
    MissL_SpecialEffectFailID,
    MissL_SpecialEffectSuccessID,
    MissL_StringIndex,
    MissL_StartMap,
    MissL_StartLevel,
    MissL_SuccessMap,
    MissL_SuccessLevel,
    MissL_FailMap,
    MissL_FailLevel,
    MissL_MapNo,
    MissL_LevelNo,
    MissL_BankTestFail,
    MissL_Complete,
    MissL_MissionCond,
    MissL_ReLevelNo,
    MissL_CashReward,
    MissL_PANStart,
    MissL_PANEnd,
    MissL_WaitToFade,
    MissL_ImmediateNextOnSuccess,
    MissL_RemainUntilSuccess,
    MissL_IsFinalMission,
    MissL_PreProcess,
};

const struct TbNamedEnum missions_conf_common_cmds[] = {
  {"MissionsCount",	MissL_MissionsCount},
  {"TextName",		MissL_TextName},
  {"TextId",		MissL_TextId},
  {"FirstTrigger",	MissL_FirstTrigger},
  {"NetscanTextId",	MissL_NetscanTextId},
  {"WeaponsTextIdShift",MissL_WeaponsTextIdShift},
  {"ResearchWeapons",MissL_ResearchWeapons},
  {"StandardWeapons",MissL_StandardWeapons},
  {"ResearchMods",	MissL_ResearchMods},
  {"StandardMods",	MissL_StandardMods},
  {"ProjectorFnMk",	MissL_ProjectorFnMk},
  {"OutroFMV",		MissL_OutroFMV},
  {"OutroBkFn",		MissL_OutroBkFn},
  {"Selectable",	MissL_Selectable},
  {"Type",			MissL_Type},
  {NULL,			0},
};

const struct TbNamedEnum missions_conf_mission_cmds[] = {
  {"Name",			MissL_TextName},
  {"TextId",		MissL_TextId},
  {"SpecialEffectID",	MissL_SpecialEffectID},
  {"SourceID",		MissL_SourceID},
  {"SuccessID",		MissL_SuccessID},
  {"FailID",		MissL_FailID},
  {"SpecialTrigger",	MissL_SpecialTrigger},
  {"SuccessTrigger",	MissL_SuccessTrigger},
  {"FailTrigger",	MissL_FailTrigger},
  {"BankTest",		MissL_BankTest},
  {"SpecialEffectFailID",	MissL_SpecialEffectFailID},
  {"SpecialEffectSuccessID",	MissL_SpecialEffectSuccessID},
  {"StringIndex",	MissL_StringIndex},
  {"StartMap",		MissL_StartMap},
  {"StartLevel",	MissL_StartLevel},
  {"SuccessMap",	MissL_SuccessMap},
  {"SuccessLevel",	MissL_SuccessLevel},
  {"FailMap",		MissL_FailMap},
  {"FailLevel",		MissL_FailLevel},
  {"MapNo",			MissL_MapNo},
  {"LevelNo"	,	MissL_LevelNo},
  {"BankTestFail",	MissL_BankTestFail},
  {"Complete",		MissL_Complete},
  {"MissionCond",	MissL_MissionCond},
  {"ReLevelNo",		MissL_ReLevelNo},
  {"CashReward",	MissL_CashReward},
  {"PANStart",		MissL_PANStart},
  {"PANEnd",		MissL_PANEnd},
  {"WaitToFade",	MissL_WaitToFade},
  {"ImmediateNextOnSuccess",MissL_ImmediateNextOnSuccess},
  {"RemainUntilSuccess",MissL_RemainUntilSuccess},
  {"IsFinalMission",MissL_IsFinalMission},
  {"PreProcess",	MissL_PreProcess},
  {NULL,			0},
};

const struct TbNamedEnum missions_conf_common_types[] = {
  {"SP",			1},
  {"MP",			2},
  {NULL,			0},
};

const struct TbNamedEnum missions_conf_any_bool[] = {
  {"True",			1},
  {"False",			2},
  {NULL,			0},
};

struct Campaign campaigns[CAMPAIGNS_MAX_COUNT];

/** Size of campaign strings within the engine buffer.
 */
ushort campaign_strings_len = 0;

/** Size of mission strings after campaign strings.
 */
extern ushort mission_strings_len; // = 0;
extern ushort display_mode;

void load_campaigns(void)
{
    ushort campgn;

    campaign_strings_len = 0;
    mission_strings_len = 0;

    for (campgn = 0; campgn < CAMPAIGNS_MAX_COUNT; campgn++) {
        if (!read_missions_conf_info(campgn))
            break;
    }
}

ushort selectable_campaigns_count(void)
{
    ushort matches;
    ushort campgn;

    matches = 0;
    for (campgn = 0; campgn < CAMPAIGNS_MAX_COUNT; campgn++) {
        struct Campaign *p_campgn;

        p_campgn = &campaigns[campgn];
        if ((p_campgn->Flags & CmpgF_IsSinglePlayer) != 0)
            if ((p_campgn->Flags & CmpgF_IsSelectable) != 0)
                matches++;
    }
    return matches;
}

ushort find_mission_state_slot(ushort missi)
{
    ushort i;

    if (missi < 1)
        return 0;
    for (i = 1; i < 50; i++) {
        if (mission_open[i] == missi)
            break;
    }
    if (i >= 50)
        i = 0;
    return i;
}

ushort find_empty_mission_state_slot(void)
{
    ushort i;

    for (i = 1; i < 50; i++) {
        if (mission_open[i] == 0)
            break;
    }
    if (i >= 50)
        i = 0;
    return i;
}

void remove_mission_state_slot(ushort mslot)
{
    ushort i;

    for (i = mslot; i < 50-1; i++)
    {
        mission_open[i] = mission_open[i + 1];
        mission_state[i] = mission_state[i + 1];
    }
}

void init_mission_states(void)
{
    struct Objective *p_objectv;
    ushort missi, mslot;
    ushort objectv;
    int i;

    mslot = find_mission_state_slot(ingame.CurrentMission);
    if (mslot > 0) {
        missi = mission_open[mslot];
        mission_state[mslot] = 0;
    } else {
        missi = ingame.CurrentMission;
        LOGWARN("Mission %d has no slot; initing anyway", (int)missi);
    }
    if (missi > next_mission) {
        LOGERR("Ignored request for unallocated mission %d", (int)missi);
        return;
    }

    mission_list[missi].BankTest = 0;

    objectv = mission_list[missi].SuccessHead;
    for (i = 0; i < 100; i++) {
        if (objectv == 0)
            break;
        p_objectv = &game_used_objectives[objectv];
        p_objectv->Status = 0;
        objectv = p_objectv->Next;
    }
    LOGSYNC("Prepared %d success objectives for mission %d", i, (int)missi);

    objectv = mission_list[missi].FailHead;
    for (i = 0; i < 100; i++) {
        if (objectv == 0)
            break;
        p_objectv = &game_used_objectives[objectv];
        p_objectv->Status = 0;
        objectv = p_objectv->Next;
    }
    LOGSYNC("Prepared %d fail objectives for mission %d", i, (int)missi);
}

void load_missions(int num)
{
    read_missions_conf_file(num);
    apply_missions_fixups();
}

void apply_missions_fixups(void)
{
    ushort missi;

    for (missi = 0; missi < next_mission; missi++)
    {
        struct Mission *p_missi;
        int n;

        p_missi = &mission_list[missi];

        for (n=0; n < 3; n++)
        {
            if (p_missi->SpecialTrigger[n] > next_mission) {
                LOGWARN("Reference to invalid mission %d within mission %d %s; cleared",
                    (int)p_missi->SpecialTrigger[n], (int)missi, "SpecialTrigger");
                p_missi->SpecialTrigger[n] = 0;
            }
        }
        for (n=0; n < 3; n++)
        {
            struct Mission *p_trg_missi;

            if (p_missi->SuccessTrigger[n] > next_mission) {
                LOGWARN("Reference to invalid mission %d within mission %d %s; cleared",
                    (int)p_missi->SuccessTrigger[n], (int)missi, "SuccessTrigger");
                p_missi->SuccessTrigger[n] = 0;
            }
            if (p_missi->SuccessTrigger[n] == 0)
                continue;
            p_trg_missi = &mission_list[p_missi->SuccessTrigger[n]];
            // Mark mission which are immediate after previous
            if (p_missi->Flags & MisF_ImmediateNextOnSuccess)
                p_trg_missi->Flags |= MisF_ImmediatePrevious;
             else
                p_trg_missi->Flags &= ~MisF_ImmediatePrevious;
        }
        for (n=0; n < 3; n++)
        {
            if (p_missi->FailTrigger[n] > next_mission) {
                LOGWARN("Reference to invalid mission %d within  mission %d%s; cleared",
                    (int)p_missi->FailTrigger[n], (int)missi, "FailTrigger");
                p_missi->FailTrigger[n] = 0;
            }
        }
    }
}

void fix_mission_used_objectives(short missi)
{
    struct Mission *p_missi;
    struct Objective *p_objectv;
    ushort objectv;
    int i;

    p_missi = &mission_list[missi];

    objectv = p_missi->SuccessHead;
    for (i = 0; i < 100; i++) {
        if (objectv == 0)
            break;
        p_objectv = &game_used_objectives[objectv];
        fix_single_objective(p_objectv, objectv, "U");
        objectv = p_objectv->Next;
    }
    LOGSYNC("Fixed %d success objectives for mission %hd", i, missi);

    objectv = p_missi->FailHead;
    for (i = 0; i < 100; i++) {
        if (objectv == 0)
            break;
        p_objectv = &game_used_objectives[objectv];
        fix_single_objective(p_objectv, objectv, "U");
        objectv = p_objectv->Next;
    }
    LOGSYNC("Fixed %d fail objectives for mission %hd", i, missi);
}

void read_missions_bin_file(int num)
{
#if 0
    asm volatile ("call ASM_load_missions\n"
        : : "a" (num));
#else
    TbFileHandle fh;
    char locstr[52];
    ulong fmtver;
    char *p_str;
    int i;

    fmtver = 1;
    p_str = engine_mem_alloc_ptr + engine_mem_alloc_size - 64000 + campaign_strings_len;
    sprintf(locstr, "%s/all%03d.mis", game_dirs[DirPlace_Levels].directory, num);
    fh = LbFileOpen(locstr, Lb_FILE_MODE_READ_ONLY);
    if (fh != INVALID_FILE)
    {
        LbFileRead(fh, &fmtver, sizeof(ulong));
        LbFileRead(fh, &mission_strings_len, sizeof(ushort));
        LbFileRead(fh, p_str, mission_strings_len);
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
        char *p_strend;

        p_strend = p_str + mission_strings_len;
        for (i = 1; i < next_mission; i++) {
            if (p_str >= p_strend) break;
            mission_list[i].TextName = p_str;
            p_str += strlen(p_str) + 1;
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
    if ((p_missi->field_4A) != 0) {
        sprintf(buf, "field_4A = %d\n", (int)p_missi->field_4A);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if (p_missi->field_4B != 0) {
      sprintf(buf, "field_4B = %d\n", (int)p_missi->field_4B);
      LbFileWrite(fh, buf, strlen(buf));
    }
}

void save_missions_conf_file(int num)
{
    TbFileHandle fh;
    char locbuf[120];
    char conf_fname[80];
    int i;

    sprintf(conf_fname, "%s" FS_SEP_STR "miss%03d.ini", "conf", num);
    fh = LbFileOpen(conf_fname, Lb_FILE_MODE_NEW);
    if (fh == INVALID_FILE) {
        LOGERR("Could not create '%s' file.", conf_fname);
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

            save_objective_chain_conf(fh, p_missi->StartHead, locbuf, sizeof(locbuf));

            sprintf(locbuf, "\n");
            LbFileWrite(fh, locbuf, strlen(locbuf));
        }

        {
            sprintf(locbuf, "[missuccess%d]\n", i);
            LbFileWrite(fh, locbuf, strlen(locbuf));

            save_objective_chain_conf(fh, p_missi->SuccessHead, locbuf, sizeof(locbuf));

            sprintf(locbuf, "\n");
            LbFileWrite(fh, locbuf, strlen(locbuf));
        }

        if (p_missi->FailHead != 0) {
            sprintf(locbuf, "[misfail%d]\n", i);
            LbFileWrite(fh, locbuf, strlen(locbuf));

            save_objective_chain_conf(fh, p_missi->FailHead, locbuf, sizeof(locbuf));

            sprintf(locbuf, "\n");
            LbFileWrite(fh, locbuf, strlen(locbuf));
        }
    }
    LbFileClose(fh);
}

/** Reads common information block only, from missions file.
 */
TbBool read_missions_conf_info(int num)
{
    TbFileHandle conf_fh;
    TbBool done;
    unsigned int i;
    long k;
    char *conf_buf;
    struct TbIniParser parser;
    char conf_fname[80];
    int conf_len;
    struct Campaign *p_campgn;
    char *p_str;

    sprintf(conf_fname, "%s" FS_SEP_STR "miss%03d.ini", "conf", num);
    conf_fh = LbFileOpen(conf_fname, Lb_FILE_MODE_READ_ONLY);
    if (conf_fh != INVALID_FILE) {
        conf_len = LbFileLengthHandle(conf_fh);
        if (conf_len > 1024*1024)
            conf_len = 1024*1024;
        conf_buf = LbMemoryAlloc(conf_len+16);
        conf_len = LbFileRead(conf_fh, conf_buf, conf_len);
        LOGSYNC("Processing '%s' file, %d bytes", conf_fname, conf_len);
        LbFileClose(conf_fh);
    } else {
        LOGSYNC("Could not open '%s' file.", conf_fname);
        conf_buf = LbMemoryAlloc(16);
        conf_len = 0;
    }
    conf_buf[conf_len] = '\0';
    LbIniParseStart(&parser, conf_buf, conf_len);
#define CONFWRNLOG(format,args...) LOGWARN("%s(line %lu): " format, conf_fname, parser.line_num, ## args)
#define CONFDBGLOG(format,args...) LOGDBG("%s(line %lu): " format, conf_fname, parser.line_num, ## args)
    p_campgn = &campaigns[num];
    LbMemorySet(p_campgn, 0, sizeof(struct Campaign));

    p_str = engine_mem_alloc_ptr + engine_mem_alloc_size - 64000 + campaign_strings_len;
    // Parse the [common] section of loaded file
    done = false;
    if (LbIniFindSection(&parser, "common") != Lb_SUCCESS) {
        CONFDBGLOG("Could not find \"[%s]\" section, file skipped.", "common");
        LbIniParseEnd(&parser);
        LbMemoryFree(conf_buf);
        return false;
    }
#define COMMAND_TEXT(cmd_num) LbNamedEnumGetName(missions_conf_common_cmds,cmd_num)
    while (!done)
    {
        int cmd_num;

        // Finding command number in this line
        i = 0;
        cmd_num = LbIniRecognizeKey(&parser, missions_conf_common_cmds);
        // Now store the config item in correct place
        switch (cmd_num)
        {
        case MissL_MissionsCount:
            break;
        case MissL_TextName:
            i = LbIniValueGetStrWhole(&parser, p_str, 80);
            if (i <= 0) {
                CONFWRNLOG("Couldn't read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                break;
            }
            p_campgn->TextName = p_str;
            p_str += strlen(p_str) + 1;
            CONFDBGLOG("%s \"%s\"", COMMAND_TEXT(cmd_num), (int)p_campgn->TextName);
            break;
        case MissL_TextId:
            i = LbIniValueGetLongInt(&parser, &k);
            if (i <= 0) {
                CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                break;
            }
            p_campgn->TextId = k;
            CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_campgn->TextId);
            break;
        case MissL_FirstTrigger:
            i = LbIniValueGetLongInt(&parser, &k);
            if (i <= 0) {
                CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                break;
            }
            p_campgn->FirstTrigger = k;
            CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_campgn->FirstTrigger);
            break;
        case MissL_NetscanTextId:
            i = LbIniValueGetLongInt(&parser, &k);
            if (i <= 0) {
                CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                break;
            }
            p_campgn->NetscanTextId = k;
            CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_campgn->NetscanTextId);
            break;
        case MissL_WeaponsTextIdShift:
            i = LbIniValueGetLongInt(&parser, &k);
            if (i <= 0) {
                CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                break;
            }
            p_campgn->WeaponsTextIdShift = k;
            CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_campgn->WeaponsTextIdShift);
            break;
        case MissL_ResearchWeapons:
        case MissL_StandardWeapons:
        case MissL_ResearchMods:
        case MissL_StandardMods:
            // Not reading at this point - cannot guarantee weapons list and mods list is loaded
            break;
        case MissL_ProjectorFnMk:
            i = LbIniValueGetStrWhole(&parser, p_str, 80);
            if (i <= 0) {
                CONFWRNLOG("Couldn't read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                break;
            }
            p_campgn->ProjectorFnMk = p_str;
            p_str += strlen(p_str) + 1;
            CONFDBGLOG("%s \"%s\"", COMMAND_TEXT(cmd_num), (int)p_campgn->ProjectorFnMk);
            break;
        case MissL_OutroFMV:
            i = LbIniValueGetStrWhole(&parser, p_str, 80);
            if (i <= 0) {
                CONFWRNLOG("Couldn't read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                break;
            }
            p_campgn->OutroFMV = p_str;
            p_str += strlen(p_str) + 1;
            CONFDBGLOG("%s \"%s\"", COMMAND_TEXT(cmd_num), (int)p_campgn->OutroFMV);
            break;
        case MissL_OutroBkFn:
            i = LbIniValueGetStrWhole(&parser, p_str, 80);
            if (i <= 0) {
                CONFWRNLOG("Couldn't read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                break;
            }
            p_campgn->OutroBkFn = p_str;
            p_str += strlen(p_str) + 1;
            CONFDBGLOG("%s \"%s\"", COMMAND_TEXT(cmd_num), (int)p_campgn->OutroBkFn);
            break;
        case MissL_Selectable:
            i = LbIniValueGetNamedEnum(&parser, missions_conf_any_bool);
            if (i <= 0) {
                CONFWRNLOG("Could not recognize \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                break;
            }
            if (i == 1)
                p_campgn->Flags |= CmpgF_IsSelectable;
            else
                p_campgn->Flags &= ~CmpgF_IsSelectable;
            CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)(p_campgn->Flags & CmpgF_IsSelectable));
            break;
        case MissL_Type:
            i = LbIniValueGetNamedEnum(&parser, missions_conf_common_types);
            if (i <= 0) {
                CONFWRNLOG("Could not recognize \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                break;
            }
            if (i == 1)
                p_campgn->Flags |= CmpgF_IsSinglePlayer;
            else
                p_campgn->Flags &= ~CmpgF_IsSinglePlayer;
            CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)(p_campgn->Flags & CmpgF_IsSinglePlayer));
            break;
        case 0: // comment
            break;
        case -1: // end of buffer
        case -3: // end of section
            done = true;
            break;
        default:
            CONFWRNLOG("Unrecognized command.");
            break;
        }
        LbIniSkipToNextLine(&parser);
    }
#undef COMMAND_TEXT
    LbIniParseEnd(&parser);
    LbMemoryFree(conf_buf);
    campaign_strings_len = p_str - (char *)(engine_mem_alloc_ptr + engine_mem_alloc_size - 64000);
    return true;
}

/** Reads missions file, with information on all missions included.
 */
void read_missions_conf_file(int num)
{
    TbFileHandle conf_fh;
    TbBool done;
    int i, n;
    long k;
    char *conf_buf;
    struct TbIniParser parser;
    struct Campaign *p_campgn;
    char locbuf[120];
    char conf_fname[80];
    int conf_len;
    int missi;
    char *p_str;

    sprintf(conf_fname, "%s" FS_SEP_STR "miss%03d.ini", "conf", num);
    conf_fh = LbFileOpen(conf_fname, Lb_FILE_MODE_READ_ONLY);
    if (conf_fh != INVALID_FILE) {
        conf_len = LbFileLengthHandle(conf_fh);
        if (conf_len > 1024*1024)
            conf_len = 1024*1024;
        conf_buf = LbMemoryAlloc(conf_len+16);
        conf_len = LbFileRead(conf_fh, conf_buf, conf_len);
        LOGSYNC("Processing '%s' file, %d bytes", conf_fname, conf_len);
        LbFileClose(conf_fh);
    } else {
        LOGERR("Could not open '%s' file, mission list empty.", conf_fname);
        conf_buf = LbMemoryAlloc(16);
        conf_len = 0;
    }
    conf_buf[conf_len] = '\0';
    LbIniParseStart(&parser, conf_buf, conf_len);
#define CONFWRNLOG(format,args...) LOGWARN("%s(line %lu): " format, conf_fname, parser.line_num, ## args)
#define CONFDBGLOG(format,args...) LOGDBG("%s(line %lu): " format, conf_fname, parser.line_num, ## args)
    p_campgn = &campaigns[num];
    next_mission = 0;
    next_used_objective = 0;
    // Add empty objective 0
    add_used_objective(0, 0);

    p_str = engine_mem_alloc_ptr + engine_mem_alloc_size - 64000 + campaign_strings_len;
    // Parse the [common] section of loaded file
    done = false;
    if (LbIniFindSection(&parser, "common") != Lb_SUCCESS) {
        CONFWRNLOG("Could not find \"[%s]\" section, file skipped.", "common");
        LbIniParseEnd(&parser);
        LbMemoryFree(conf_buf);
        return;
    }
#define COMMAND_TEXT(cmd_num) LbNamedEnumGetName(missions_conf_common_cmds,cmd_num)
    while (!done)
    {
        int cmd_num;

        // Finding command number in this line
        i = 0;
        cmd_num = LbIniRecognizeKey(&parser, missions_conf_common_cmds);
        // Now store the config item in correct place
        switch (cmd_num)
        {
        case MissL_MissionsCount:
            i = LbIniValueGetLongInt(&parser, &k);
            if (i <= 0) {
                CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                break;
            }
            next_mission = k;
            CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)next_mission);
            break;
        case MissL_ResearchWeapons:
            n = 0;
            while (1)
            {
                i = LbIniValueGetNamedEnum(&parser, weapon_names);
                if (i <= 0) {
                    break;
                }
                n |= (1 << (i-1));
            }
            p_campgn->ResearchWeapons = n;
            CONFDBGLOG("%s 0x%lx", COMMAND_TEXT(cmd_num), p_campgn->ResearchWeapons);
            break;
        case MissL_StandardWeapons:
            n = 0;
            while (1)
            {
                i = LbIniValueGetNamedEnum(&parser, weapon_names);
                if (i <= 0) {
                    break;
                }
                n |= (1 << (i-1));
            }
            p_campgn->StandardWeapons = n;
            CONFDBGLOG("%s 0x%lx", COMMAND_TEXT(cmd_num), p_campgn->StandardWeapons);
            break;
        case MissL_ResearchMods:
            n = 0;
            while (1)
            {
                i = LbIniValueGetNamedEnum(&parser, mod_names);
                if (i <= 0) {
                    break;
                }
                n |= (1 << (i-1));
            }
            p_campgn->ResearchMods = n;
            CONFDBGLOG("%s 0x%lx", COMMAND_TEXT(cmd_num), p_campgn->ResearchMods);
            break;
        case MissL_StandardMods:
            n = 0;
            while (1)
            {
                i = LbIniValueGetNamedEnum(&parser, mod_names);
                if (i <= 0) {
                    break;
                }
                n |= (1 << (i-1));
            }
            p_campgn->StandardMods = n;
            CONFDBGLOG("%s 0x%lx", COMMAND_TEXT(cmd_num), p_campgn->StandardMods);
            break;
        case MissL_TextName:
        case MissL_TextId:
        case MissL_FirstTrigger:
        case MissL_NetscanTextId:
        case MissL_WeaponsTextIdShift:
        case MissL_ProjectorFnMk:
        case MissL_OutroFMV:
        case MissL_OutroBkFn:
        case MissL_Selectable:
        case MissL_Type:
            // These properties are handled when reading campaign info
            break;
        case 0: // comment
            break;
        case -1: // end of buffer
        case -3: // end of section
            done = true;
            break;
        default:
            CONFWRNLOG("Unrecognized command.");
            break;
        }
        LbIniSkipToNextLine(&parser);
    }
#undef COMMAND_TEXT
    for (missi = 0; missi < next_mission; missi++)
    {
        char sect_name[16];
        struct Mission *p_missi;

        p_missi = &mission_list[missi];
        LbMemorySet(p_missi, 0, sizeof(struct Mission));

        // Parse the [missionN] sections of loaded file
        done = false;
        sprintf(sect_name, "mission%d", missi);
        if (LbIniFindSection(&parser, sect_name) != Lb_SUCCESS) {
            CONFWRNLOG("Could not find \"[%s]\" section.", sect_name);
            continue;
        }
#define COMMAND_TEXT(cmd_num) LbNamedEnumGetName(missions_conf_mission_cmds,cmd_num)
        while (!done)
        {
            int cmd_num;

            // Finding command number in this line
            i = 0;
            cmd_num = LbIniRecognizeKey(&parser, missions_conf_mission_cmds);
            // Now store the config item in correct place
            switch (cmd_num)
            {
            case MissL_TextName:
                i = LbIniValueGetStrWhole(&parser, p_str, 80);
                if (i <= 0) {
                    CONFWRNLOG("Couldn't read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_missi->TextName = p_str;
                p_str += strlen(p_str) + 1;
                CONFDBGLOG("%s \"%s\"", COMMAND_TEXT(cmd_num), (int)p_missi->TextName);
                break;
            case MissL_TextId:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_missi->TextId = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_missi->TextId);
                break;
            case MissL_SpecialEffectID:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_missi->SpecialEffectID = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_missi->SpecialEffectID);
                break;
            case MissL_SourceID:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_missi->SourceID = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_missi->SourceID);
                break;
            case MissL_SuccessID:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_missi->SuccessID = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_missi->SuccessID);
                break;
            case MissL_FailID:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_missi->FailID = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_missi->FailID);
                break;
            case MissL_SpecialTrigger:
                for (n=0; n < 3; n++)
                {
                    i = LbIniValueGetLongInt(&parser, &k);
                    if (i <= 0) {
                        CONFWRNLOG("Could not read \"%s\" command parameter %u.", COMMAND_TEXT(cmd_num), n);
                        break;
                    }
                    p_missi->SpecialTrigger[n] = k;
                }
                CONFDBGLOG("%s %d %d %d", COMMAND_TEXT(cmd_num), (int)p_missi->SpecialTrigger[0],
                  (int)p_missi->SpecialTrigger[1], (int)p_missi->SpecialTrigger[2]);
                break;
            case MissL_SuccessTrigger:
                for (n=0; n < 3; n++)
                {
                    i = LbIniValueGetLongInt(&parser, &k);
                    if (i <= 0) {
                        CONFWRNLOG("Could not read \"%s\" command parameter %u.", COMMAND_TEXT(cmd_num), n);
                        break;
                    }
                    p_missi->SuccessTrigger[n] = k;
                }
                CONFDBGLOG("%s %d %d %d", COMMAND_TEXT(cmd_num), (int)p_missi->SuccessTrigger[0],
                  (int)p_missi->SuccessTrigger[1], (int)p_missi->SuccessTrigger[2]);
                break;
            case MissL_FailTrigger:
                for (n=0; n < 3; n++)
                {
                    i = LbIniValueGetLongInt(&parser, &k);
                    if (i <= 0) {
                        CONFWRNLOG("Could not read \"%s\" command parameter %u.", COMMAND_TEXT(cmd_num), n);
                        break;
                    }
                    p_missi->FailTrigger[n] = k;
                }
                CONFDBGLOG("%s %d %d %d", COMMAND_TEXT(cmd_num), (int)p_missi->FailTrigger[0],
                  (int)p_missi->FailTrigger[1], (int)p_missi->FailTrigger[2]);
                break;
            case MissL_BankTest:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_missi->BankTest = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_missi->BankTest);
                break;
            case MissL_SpecialEffectFailID:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_missi->SpecialEffectFailID = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_missi->SpecialEffectFailID);
                break;
            case MissL_SpecialEffectSuccessID:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_missi->SpecialEffectSuccessID = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_missi->SpecialEffectSuccessID);
                break;
            case MissL_StringIndex:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_missi->StringIndex = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_missi->StringIndex);
                break;
            case MissL_StartMap:
                for (n=0; n < 3; n++)
                {
                    i = LbIniValueGetLongInt(&parser, &k);
                    if (i <= 0) {
                        CONFWRNLOG("Could not read \"%s\" command parameter %u.", COMMAND_TEXT(cmd_num), n);
                        break;
                    }
                    p_missi->StartMap[n] = k;
                }
                CONFDBGLOG("%s %d %d %d", COMMAND_TEXT(cmd_num), (int)p_missi->StartMap[0],
                  (int)p_missi->StartMap[1], (int)p_missi->StartMap[2]);
                break;
            case MissL_StartLevel:
                for (n=0; n < 3; n++)
                {
                    i = LbIniValueGetLongInt(&parser, &k);
                    if (i <= 0) {
                        CONFWRNLOG("Could not read \"%s\" command parameter %u.", COMMAND_TEXT(cmd_num), n);
                        break;
                    }
                    p_missi->StartLevel[n] = k;
                }
                CONFDBGLOG("%s %d %d %d", COMMAND_TEXT(cmd_num), (int)p_missi->StartLevel[0],
                  (int)p_missi->StartLevel[1], (int)p_missi->StartLevel[2]);
                break;
            case MissL_SuccessMap:
                for (n=0; n < 3; n++)
                {
                    i = LbIniValueGetLongInt(&parser, &k);
                    if (i <= 0) {
                        CONFWRNLOG("Could not read \"%s\" command parameter %u.", COMMAND_TEXT(cmd_num), n);
                        break;
                    }
                    p_missi->SuccessMap[n] = k;
                }
                CONFDBGLOG("%s %d %d %d", COMMAND_TEXT(cmd_num), (int)p_missi->SuccessMap[0],
                  (int)p_missi->SuccessMap[1], (int)p_missi->SuccessMap[2]);
                break;
            case MissL_SuccessLevel:
                for (n=0; n < 3; n++)
                {
                    i = LbIniValueGetLongInt(&parser, &k);
                    if (i <= 0) {
                        CONFWRNLOG("Could not read \"%s\" command parameter %u.", COMMAND_TEXT(cmd_num), n);
                        break;
                    }
                    p_missi->SuccessLevel[n] = k;
                }
                CONFDBGLOG("%s %d %d %d", COMMAND_TEXT(cmd_num), (int)p_missi->SuccessLevel[0],
                  (int)p_missi->SuccessLevel[1], (int)p_missi->SuccessLevel[2]);
                break;
            case MissL_FailMap:
                for (n=0; n < 3; n++)
                {
                    i = LbIniValueGetLongInt(&parser, &k);
                    if (i <= 0) {
                        CONFWRNLOG("Could not read \"%s\" command parameter %u.", COMMAND_TEXT(cmd_num), n);
                        break;
                    }
                    p_missi->FailMap[n] = k;
                }
                CONFDBGLOG("%s %d %d %d", COMMAND_TEXT(cmd_num), (int)p_missi->FailMap[0],
                  (int)p_missi->FailMap[1], (int)p_missi->FailMap[2]);
                break;
            case MissL_FailLevel:
                for (n=0; n < 3; n++)
                {
                    i = LbIniValueGetLongInt(&parser, &k);
                    if (i <= 0) {
                        CONFWRNLOG("Could not read \"%s\" command parameter %u.", COMMAND_TEXT(cmd_num), n);
                        break;
                    }
                    p_missi->FailLevel[n] = k;
                }
                CONFDBGLOG("%s %d %d %d", COMMAND_TEXT(cmd_num), (int)p_missi->FailLevel[0],
                  (int)p_missi->FailLevel[1], (int)p_missi->FailLevel[2]);
                break;
            case MissL_MapNo:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_missi->MapNo = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_missi->MapNo);
                break;
            case MissL_LevelNo:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_missi->LevelNo = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_missi->LevelNo);
                break;
            case MissL_BankTestFail:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_missi->BankTestFail = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_missi->BankTestFail);
                break;
            case MissL_Complete:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_missi->Complete = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_missi->Complete);
                break;
            case MissL_MissionCond:
                for (n=0; n < 5; n++)
                {
                    i = LbIniValueGetLongInt(&parser, &k);
                    if (i <= 0) {
                        CONFWRNLOG("Could not read \"%s\" command parameter %u.", COMMAND_TEXT(cmd_num), n);
                        break;
                    }
                    p_missi->MissionCond[n] = k;
                }
                CONFDBGLOG("%s %d %d %d %d %d", COMMAND_TEXT(cmd_num), (int)p_missi->MissionCond[0],
                  (int)p_missi->MissionCond[1], (int)p_missi->MissionCond[2],
                  (int)p_missi->MissionCond[3], (int)p_missi->MissionCond[4]);
                break;
            case MissL_ReLevelNo:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_missi->ReLevelNo = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_missi->ReLevelNo);
                break;
            case MissL_CashReward:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_missi->CashReward = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_missi->CashReward);
                break;
            case MissL_PANStart:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_missi->PANStart = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_missi->PANStart);
                break;
            case MissL_PANEnd:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_missi->PANEnd = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_missi->PANEnd);
                break;
            case MissL_WaitToFade:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_missi->WaitToFade = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_missi->WaitToFade);
                break;

            case MissL_ImmediateNextOnSuccess:
                i = LbIniValueGetNamedEnum(&parser, missions_conf_any_bool);
                if (i <= 0) {
                    CONFWRNLOG("Could not recognize \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                if (i == 1)
                    p_missi->Flags |= MisF_ImmediateNextOnSuccess;
                else
                    p_missi->Flags &= ~MisF_ImmediateNextOnSuccess;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)(p_missi->Flags & MisF_ImmediateNextOnSuccess));
                break;
            case MissL_RemainUntilSuccess:
                i = LbIniValueGetNamedEnum(&parser, missions_conf_any_bool);
                if (i <= 0) {
                    CONFWRNLOG("Could not recognize \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                if (i == 1)
                    p_missi->Flags |= MisF_RemainUntilSuccess;
                else
                    p_missi->Flags &= ~MisF_RemainUntilSuccess;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)(p_missi->Flags & MisF_RemainUntilSuccess));
                break;
            case MissL_IsFinalMission:
                i = LbIniValueGetNamedEnum(&parser, missions_conf_any_bool);
                if (i <= 0) {
                    CONFWRNLOG("Could not recognize \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                if (i == 1)
                    p_missi->Flags |= MisF_IsFinalMission;
                else
                    p_missi->Flags &= ~MisF_IsFinalMission;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)(p_missi->Flags & MisF_IsFinalMission));
                break;
            case MissL_PreProcess:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_missi->PreProcess = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_missi->PreProcess);
                break;
            case 0: // comment
                break;
            case -1: // end of buffer
            case -3: // end of section
                done = true;
                break;
            default:
                CONFWRNLOG("Unrecognized command.");
                break;
            }
            LbIniSkipToNextLine(&parser);
        }
#undef COMMAND_TEXT

        // Parse the [missuccessN] sections of loaded file
        done = false;
        sprintf(sect_name, "missuccess%d", missi);
        if (LbIniFindSection(&parser, sect_name) != Lb_SUCCESS) {
            CONFWRNLOG("Could not find \"[%s]\" section.", sect_name);
        } else
        while (!done)
        {
            long pri;
            struct Objective *p_objectv;

            // Get the key, as it holds priority
            k = LbIniGetKey(&parser, locbuf, sizeof(locbuf));
            // Now store the config item in correct place
            switch (k)
            {
            case 0: // comment
                break;
            case -1: // end of buffer
            case -3: // end of section
                done = true;
                break;
            default:
                if ((locbuf[0] != 'P') || !isdigit(locbuf[1])) {
                    CONFWRNLOG("Unrecognized key in \"%s\" section.", sect_name);
                    break;
                }
                // Priority is in key name
                pri = atol(&locbuf[1]);
                // Now get the objective command from value
                k = LbIniValueGetStrWhole(&parser, locbuf, sizeof(locbuf));
                if (k <= 0) {
                    CONFWRNLOG("Could not read the latter of key-value pair in \"%s\" section.", sect_name);
                    break;
                }
                k = parse_next_used_objective(locbuf, sizeof(locbuf), pri, p_missi->MapNo, p_missi->LevelNo);
                if (k <= 0) {
                    CONFWRNLOG("Could parse objective command in \"%s\" section.", sect_name);
                    break;
                }
                p_objectv = &game_used_objectives[k];
                p_objectv->Next = p_missi->SuccessHead;
                p_missi->SuccessHead = k;
            }

            LbIniSkipToNextLine(&parser);
        }

        // Parse the [misfailN] sections of loaded file
        done = false;
        sprintf(sect_name, "misfail%d", missi);
        if (LbIniFindSection(&parser, sect_name) != Lb_SUCCESS) {
            CONFDBGLOG("Could not find \"[%s]\" section.", sect_name);
        } else
        while (!done)
        {
            long pri;
            struct Objective *p_objectv;

            // Get the key, as it holds priority
            k = LbIniGetKey(&parser, locbuf, sizeof(locbuf));
            // Now store the config item in correct place
            switch (k)
            {
            case 0: // comment
                break;
            case -1: // end of buffer
            case -3: // end of section
                done = true;
                break;
            default:
                if ((locbuf[0] != 'P') || !isdigit(locbuf[1])) {
                    CONFWRNLOG("Unrecognized key in \"%s\" section.", sect_name);
                    break;
                }
                // Priority is in key name
                pri = atol(&locbuf[1]);
                // Now get the objective command from value
                k = LbIniValueGetStrWhole(&parser, locbuf, sizeof(locbuf));
                if (k <= 0) {
                    CONFWRNLOG("Could not read the latter of key-value pair in \"%s\" section.", sect_name);
                    break;
                }
                k = parse_next_used_objective(locbuf, sizeof(locbuf), pri, p_missi->MapNo, p_missi->LevelNo);
                if (k <= 0) {
                    CONFWRNLOG("Could parse objective command in \"%s\" section.", sect_name);
                    break;
                }
                p_objectv = &game_used_objectives[k];
                p_objectv->Next = p_missi->FailHead;
                p_missi->FailHead = k;
            }

            LbIniSkipToNextLine(&parser);
        }
    }
#undef CONFDBGLOG
#undef CONFWRNLOG
    LbIniParseEnd(&parser);
    LbMemoryFree(conf_buf);
    mission_strings_len = p_str - (char *)(engine_mem_alloc_ptr + engine_mem_alloc_size - 64000 + campaign_strings_len);
}

/******************************************************************************/
