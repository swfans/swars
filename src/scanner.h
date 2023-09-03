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
  long X;
  long Z;
  ubyte Colour;
  ubyte Period;
  ubyte Speed;
  ubyte Counter;
};

struct Arc
{
  long X1;
  long Z1;
  long X2;
  long Z2;
  ubyte ColourIsUnused;
  ubyte Period;
  ubyte SpeedIsUnused;
  ubyte Counter;
};

struct Scanner
{
  long X1;
  long Y1;
  long X2;
  long Y2;
  long MX;
  long MZ;
  long Zoom;
  long Angle;
  long Contrast;
  long Brightness;
  long Width[200];
  struct BigBlip BigBlip[SCANNER_BIG_BLIP_COUNT];
  struct Arc Arc[SCANNER_ARC_COUNT];
  ubyte Group[SCANNER_GROUP_COUNT];
  ubyte GroupCol[SCANNER_GROUP_COUNT];
  ubyte GroupCount;
  short NearThing1;
};

#pragma pack()
/******************************************************************************/
extern long *SCANNER_width;

extern short waft_table2[32];
extern short waft_table[32];

void SCANNER_init(void);
void SCANNER_fill_in(void);
void SCANNER_set_colour(ubyte col);

void add_signal_to_scanner(struct Objective *p_objectv, ubyte flag);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
