/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file fedebrief.h
 *     Header file for fedebrief.c.
 * @par Purpose:
 *     Front-end desktop and menu system, mission debrief screen.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     22 Apr 2023 - 22 Oct 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef FEDEBRIEF_H
#define FEDEBRIEF_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct DebriefReport {
    short RefNo;
    short BriefNo;
    short Status;
    ulong Income;
    ulong Expenditure;
    ulong WeaponsResearched;
    ulong ModsResearched;
    ubyte ScientistsLost;
    ubyte SciLostReason;
};

#pragma pack()
/******************************************************************************/

void show_debrief_screen(void);

void init_debrief_screen_boxes(void);
void reset_debrief_screen_boxes_flags(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
