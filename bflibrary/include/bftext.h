/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bftext.h
 *     Header file for gtext.c.
 * @par Purpose:
 *     Allows drawing text using various positioning and font sprites.
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
#ifndef BFLIBRARY_BFTEXT_H_
#define BFLIBRARY_BFTEXT_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

#define TEXT_DRAW_MAX_LEN 4096

struct TbSprite;

#pragma pack()

extern int lbSpacesPerTab;
extern struct TbSprite *lbFontPtr;

int LbTextStringWidth();
int LbTextStringHeight();

TbResult LbTextDraw(long X, long Y, const char *Text);


/**
 * Computes width of one word in given string, starting at given pointer.
 * The word may end with NULL character, space, tab or line end / return carret.
 *
 * @param font The font to be used, in form of array of sprites.
 * @param text The text from which first word will be taken.
 * @return Length of the string, or 0 of either text or font is empty.
 */
long LbSprFontWordWidth(const struct TbSprite *font, const char *text);

/**
 * Computes width of one word in given string, starting at given pointer.
 * The word may end with NULL character, space, tab or line end / return carret.
 * The currently set font is used for the computations.
 *
 * @param text The text from which first word will be taken.
 * @return Length of the string, or 0 of either text or font is empty.
 */
long LbTextWordWidth(const char *text);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFTEXT_H_
/******************************************************************************/
