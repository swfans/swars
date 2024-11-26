/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file game_sprani.h
 *     Header file for game_sprani.c.
 * @par Purpose:
 *     List of sprite animation frames used by the game.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     10 Feb 2024 - 22 Sep 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef GAME_SPRANI_H
#define GAME_SPRANI_H

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

#include "bftypes.h"
#include "game_bstype.h"

/******************************************************************************/
#pragma pack(1)

struct Frame {
    ushort FirstElement;
    ubyte SWidth;
    ubyte SHeight;
    ubyte FX;
    ubyte Flags;
    ushort Next;
};

struct Element { // sizeof=0x0A
    ushort ToSprite;
    short X;
    short Y;
    ushort Flags;
    ushort Next;
};

#pragma pack()
/******************************************************************************/
extern struct Element *melement_ani;
extern struct Element *mele_ani_end;
extern ushort *nstart_ani;
extern ushort *nstart_ani_end;
extern struct Frame *frame;
extern struct Frame *frame_end;

extern ushort shield_frm[4];


void shield_frames_init(void);
void shield_frames_cycle(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
