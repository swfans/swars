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

struct BigBlip
{
  long X;
  long Z;
  char Colour;
  char Period;
  char Speed;
  char Counter;
};

struct Arc
{
  long X1;
  long Z1;
  long X2;
  long Z2;
  char ColourIsUnused;
  char Period;
  char SpeedIsUnused;
  char Counter;
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
  long Width[16];
  char field_68[724];
  struct BigBlip BigBlip_m1[1]; // data before BigBlip array
  struct BigBlip BigBlip[15];
  char field_3FC[9];
  char Arc__Period;
  char field_406;
  char field_407;
  struct Arc Arc1[3];
  char field_444[20];
  char Group[6];
  char GroupCol[6];
  ubyte GroupCount;
  short NearThing1;
};

#pragma pack()
/******************************************************************************/
extern long *SCANNER_width;

void SCANNER_init(void);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
