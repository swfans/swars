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

void save_missions_conf(int num)
{
    TbFileHandle fh;
    char locstr[120];
    int i;

    sprintf(locstr, "%s/miss%03d.ini", "conf", num);
    fh = LbFileOpen(locstr, Lb_FILE_MODE_NEW);
    if (fh == INVALID_FILE) {
        LOGERR("Missions config file could not be created");
        return;
    }

    //LbFileRead(fh, &mission_strings_len, sizeof(ushort));
    //LbFileRead(fh, engine_mem_alloc_ptr + engine_mem_alloc_size - 64000, mission_strings_len);

    for (i = 0; i < next_mission; i++) {
        struct Mission *missi;

        missi = &mission_list[i];
        sprintf(locstr, "[mission%d]\n", i);
        LbFileWrite(fh, locstr, strlen(locstr));

        if (missi->TextName == NULL) {
            sprintf(locstr, "Name = MAP%03d - LEVEL%03d\n", (int)missi->MapNo, (int)missi->LevelNo);
        } else {
            sprintf(locstr, "Name = %s\n", missi->TextName);
        }
        LbFileWrite(fh, locstr, strlen(locstr));
        if (missi->TextId != 0) {
            sprintf(locstr, "TextId = %hu\n", missi->TextId);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if (missi->StartHead != 0) {
            sprintf(locstr, "StartHead = %hu\n", missi->StartHead);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if (missi->SpecialEffectID != 0) {
            sprintf(locstr, "SpecialEffectID = %hu\n", missi->SpecialEffectID);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if (missi->SuccessHead != 0) {
            sprintf(locstr, "SuccessHead = %hu\n", missi->SuccessHead);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if (missi->FailHead != 0) {
            sprintf(locstr, "FailHead = %hu\n", missi->FailHead);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        {
            sprintf(locstr, "SourceID = %hu\n", missi->SourceID);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if (missi->SuccessID != 0) {
            sprintf(locstr, "SuccessID = %hu\n", missi->SuccessID);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if (missi->FailID != 0) {
            sprintf(locstr, "FailID = %hu\n", missi->FailID);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if ((missi->SpecialTrigger[0]|missi->SpecialTrigger[1]|missi->SpecialTrigger[2]) != 0) {
            sprintf(locstr, "SpecialTrigger = %d %d %d\n",
              (int)missi->SpecialTrigger[0], (int)missi->SpecialTrigger[1], (int)missi->SpecialTrigger[2]);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if ((missi->SuccessTrigger[0]|missi->SuccessTrigger[1]|missi->SuccessTrigger[2]) != 0) {
            sprintf(locstr, "SuccessTrigger = %d %d %d\n",
              (int)missi->SuccessTrigger[0], (int)missi->SuccessTrigger[1], (int)missi->SuccessTrigger[2]);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if ((missi->FailTrigger[0]|missi->FailTrigger[1]|missi->FailTrigger[2]) != 0) {
            sprintf(locstr, "FailTrigger = %d %d %d\n",
              (int)missi->FailTrigger[0], (int)missi->FailTrigger[1], (int)missi->FailTrigger[2]);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if (missi->BankTest != 0) {
            sprintf(locstr, "BankTest = %d\n", (int)missi->BankTest);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if (missi->SpecialEffectFailID != 0) {
            sprintf(locstr, "SpecialEffectFailID = %hu\n", missi->SpecialEffectFailID);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if (missi->SpecialEffectSuccessID != 0) {
            sprintf(locstr, "SpecialEffectSuccessID = %hu\n", missi->SpecialEffectSuccessID);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if (missi->StringIndex != 0) {
            sprintf(locstr, "StringIndex = %hu\n", missi->StringIndex);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if ((missi->StartMap[0]|missi->StartMap[1]|missi->StartMap[2]) != 0) {
            sprintf(locstr, "StartMap = %d %d %d\n",
              (int)missi->StartMap[0], (int)missi->StartMap[1], (int)missi->StartMap[2]);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if ((missi->StartLevel[0]|missi->StartLevel[1]|missi->StartLevel[2]) != 0) {
            sprintf(locstr, "StartLevel = %d %d %d\n",
              (int)missi->StartLevel[0], (int)missi->StartLevel[1], (int)missi->StartLevel[2]);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if ((missi->SuccessMap[0]|missi->SuccessMap[1]|missi->SuccessMap[2]) != 0) {
            sprintf(locstr, "SuccessMap = %d %d %d\n",
              (int)missi->SuccessMap[0], (int)missi->SuccessMap[1], (int)missi->SuccessMap[2]);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if ((missi->SuccessLevel[0]|missi->SuccessLevel[1]|missi->SuccessLevel[2]) != 0) {
            sprintf(locstr, "SuccessLevel = %d %d %d\n",
              (int)missi->SuccessLevel[0], (int)missi->SuccessLevel[1], (int)missi->SuccessLevel[2]);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if ((missi->FailMap[0]|missi->FailMap[1]|missi->FailMap[2]) != 0) {
            sprintf(locstr, "FailMap = %d %d %d\n",
              (int)missi->FailMap[0], (int)missi->FailMap[1], (int)missi->FailMap[2]);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if ((missi->FailLevel[0]|missi->FailLevel[1]|missi->FailLevel[2]) != 0) {
            sprintf(locstr, "FailLevel = %d %d %d\n",
              (int)missi->FailLevel[0], (int)missi->FailLevel[1], (int)missi->FailLevel[2]);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        {
            sprintf(locstr, "MapNo = %d\n", (int)missi->MapNo);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        {
            sprintf(locstr, "LevelNo = %d\n", (int)missi->LevelNo);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if (missi->BankTestFail != 0) {
            sprintf(locstr, "BankTestFail = %d\n", (int)missi->BankTestFail);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if (missi->Complete != 0) {
            sprintf(locstr, "Complete = %d\n", (int)missi->Complete);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if ((missi->MissionCond[0]|missi->MissionCond[1]|missi->MissionCond[2]|missi->MissionCond[3]|missi->MissionCond[4]) != 0) {
            sprintf(locstr, "MissionCond = %d %d %d %d %d\n", (int)missi->MissionCond[0],
              (int)missi->MissionCond[1], (int)missi->MissionCond[2],
              (int)missi->MissionCond[3], (int)missi->MissionCond[4]);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if (missi->ReLevelNo != 0) {
            sprintf(locstr, "ReLevelNo = %d\n", (int)missi->ReLevelNo);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if (missi->CashReward != 0) {
            sprintf(locstr, "CashReward = %d\n", (int)missi->CashReward);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if (missi->PANStart != 0) {
            sprintf(locstr, "PANStart = %d\n", (int)missi->PANStart);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if (missi->PANEnd != 0) {
            sprintf(locstr, "PANEnd = %d\n", (int)missi->PANEnd);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if (missi->WaitToFade != 0) {
            sprintf(locstr, "WaitToFade = %hu\n", missi->WaitToFade);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if (missi->PreProcess != 0) {
          sprintf(locstr, "PreProcess = %hu\n", missi->PreProcess);
          LbFileWrite(fh, locstr, strlen(locstr));
        }
        if ((missi->field_48[0]|missi->field_48[1]|missi->field_48[2]) != 0) {
            sprintf(locstr, "field_48 = %d %d %d\n", (int)missi->field_48[0],
              (int)missi->field_48[1], (int)missi->field_48[2]);
            LbFileWrite(fh, locstr, strlen(locstr));
        }
        if (missi->field_4B != 0) {
          sprintf(locstr, "field_4B = %d\n", (int)missi->field_4B);
          LbFileWrite(fh, locstr, strlen(locstr));
        }

        sprintf(locstr, "\n");
        LbFileWrite(fh, locstr, strlen(locstr));
    }
/*
    LbFileRead(fh, &next_used_objective, sizeof(ushort));
    LbFileRead(fh, game_used_objectives, sizeof(struct Objective) * next_used_objective);
    LbFileRead(fh, engine_mem_alloc_ptr + engine_mem_alloc_size - 1320 - 33, 1320);
*/
    LbFileClose(fh);
}

/******************************************************************************/
