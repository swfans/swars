/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file purpldrwlst.h
 *     Header file for purpldrwlst.c.
 * @par Purpose:
 *     Drawlists handling for the purple projector screens.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     22 Apr 2024 - 28 Sep 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef PURPLDRWLST_H
#define PURPLDRWLST_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

#pragma pack(1)

enum PurpleDrawType {
  PuDT_NONE = 0,
  PuDT_BOX = 1,
  PuDT_TEXT = 2,
  PuDT_UNK03 = 3,
  PuDT_COPYBOX = 4,
  PuDT_SPRITE = 5,
  PuDT_POTRIG = 6, /* Textured triangle from projector origin point to given line */
  PuDT_FLIC = 7,
  PuDT_NOISEBOX = 8,
  PuDT_LINE = 9,
  PuDT_HVLINE = 10,
  PuDT_TRIANGLE = 11,
  PuDT_HOTSPOT = 12,
};

struct SRect {
	short x;
	short y;
	short w;
	short h;
};

struct DITrig { // sizeof=48
	ulong *vlcbuf[2];
	long vlcid;
	ushort *imgbuf;
	struct SRect rect[2];
	long rectid; // offs=0x20
	struct SRect slice;
	long isdone; // offs=0x2C
};

struct DITriangle {
	short X1;
	short Y1;
	short X2;
	short Y2;
	short X3;
	short Y3;
	ubyte Colour;
};

struct DIBox {
	short X;
	short Y;
	short Width;
	short Height;
	ubyte Colour;
};

struct DILine {
	short X1;
	short Y1;
	short X2;
	short Y2;
	ubyte Colour;
};

struct DISprite { // sizeof=12
	short X;
	short Y;
	struct TbSprite *Sprite;
	ubyte Colour;
};

struct DIText { // sizeof=23
	short WindowX;
	short WindowY;
	short Width;
	short Height;
	short X;
	short Y; // offs=0x0A
	char *Text;
	struct TbSprite *Font;
	ushort Line;
	ubyte Colour; // offs=0x16
};

struct DIFlic {
	void (*Function)();
	void *Param;
	ubyte Colour;
};

struct DIHotspot {
	short X;
	short Y;
};

struct PurpleDrawItem { // sizeof=26
	union {
		//struct DITrig Trig; // unused? too large to be here
		struct DITriangle Triangle;
		struct DIBox Box;
		struct DILine Line;
		struct DISprite Sprite;
		struct DIText Text;
		struct DIFlic Flic;
		struct DIHotspot Hotspot;
	} U;
	ubyte Type;
	ushort Flags;
};

#pragma pack()
/******************************************************************************/

extern struct PurpleDrawItem *purple_draw_list;
extern ushort purple_draw_index;

extern struct ScreenPoint *hotspot_buffer;
#define hotspot_buffer_len 512

extern struct ScreenPoint proj_origin;

void draw_purple_screen(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
