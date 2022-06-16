/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfanywnd.h
 *     Header file for ganywnd.c.
 * @par Purpose:
 *     Unknown.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIBRARY_BFANYWND_H_
#define BFLIBRARY_BFANYWND_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

struct TbAnyWindow { // sizeof=20
    long x; // offset=0
    long y; // offset=4
    ulong width; // offset=8
    ulong height; // offset=12
    TbPixel *ptr; // offset=16
};

typedef struct TbAnyWindow TbAnyWindow;

#pragma pack()

extern struct TbAnyWindow lbTextJustifyWindow;
extern struct TbAnyWindow lbTextClipWindow;

/**
 * Stores the current graphics window coords into TbAnyWindow structure.
 * Intended to use with LbScreenLoadGraphicsWindow() when changing the window
 * temporary.
 */
TbResult LbScreenStoreGraphicsWindow(struct TbAnyWindow *wnd);

/**
 * Sets the current graphics window coords from those in TbAnyWindow structure.
 * Use it only with TbAnyWindow which was filled using function
 * LbScreenStoreGraphicsWindow(), because the values are not checked for sanity!
 * To set values from other sources, use LbScreenSetGraphicsWindow() instead.
 */
TbResult LbScreenLoadGraphicsWindow(struct TbAnyWindow *wnd);

TbResult LbTextSetJustifyWindow(long x, long y, ulong width);
TbResult LbTextSetClipWindow(long x, long y, ulong width, ulong height);
TbResult LbTextSetWindow(long x, long y, ulong width, ulong height);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFANYWND_H_
/******************************************************************************/
