/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file scanner.h
 *     Header file for scanner.c.
 * @par Purpose:
 *     Ingame scanner (minimap/radar) support.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 May 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef SCANNER_H
#define SCANNER_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

#define SCANNER_BIG_BLIP_COUNT 16
#define SCANNER_GROUP_COUNT 6
#define SCANNER_ARC_COUNT 4

struct Objective;

struct BigBlip
{
  i32 X;
  i32 Z;
  u8 Colour;
  u8 Period;
  u8 Speed;
  u8 Counter;
};

struct Arc
{
  i32 X1;
  i32 Z1;
  i32 X2;
  i32 Z2;
  u8 ColourIsUnused;
  u8 Period;
  u8 SpeedIsUnused;
  u8 Counter;
};

struct Scanner
{
  i32 X1;
  i32 Y1;
  i32 X2;
  i32 Y2;
  i32 MX;
  i32 MZ;
  i32 Zoom;
  i32 Angle;
  i32 Contrast;
  i32 Brightness;
  i32 Width[200];
  struct BigBlip BigBlip[SCANNER_BIG_BLIP_COUNT];
  struct Arc Arc[SCANNER_ARC_COUNT];
  u8 Group[SCANNER_GROUP_COUNT];
  u8 GroupCol[SCANNER_GROUP_COUNT];
  u8 GroupCount;
  short NearThing1;
};

#pragma pack()
/******************************************************************************/
extern i32 *SCANNER_width;

extern short waft_table2[32];
extern short waft_table[32];

void SCANNER_init(void);
void SCANNER_fill_in(void);
void SCANNER_set_colour(u8 col);

void add_blippoint_to_scanner(int x, int z, u8 colour);

void clear_all_scanner_signals(void);
void add_signal_to_scanner(struct Objective *p_objectv, u8 flag);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
