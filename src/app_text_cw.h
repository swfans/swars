/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file app_text_cw.h
 *     Header file for app_text_cw.c.
 * @par Purpose:
 *     Allows drawing text coloured in a wave-like transsition.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 15 Oct 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef APP_TEXT_CW_H_
#define APP_TEXT_CW_H_

#include <stdarg.h>
#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Draws a string in the current text window.
 *
 * @param posx Position of the text, X coord.
 * @param posy Position of the text, Y coord.
 * @param text The text to be drawn.
 * @return
 */
TbBool AppTextDrawColourWave(int posx, int posy, const char *text);

/**
 * Draws a string in the current text window in given scale.
 *
 * @param posx Position of the text, X coord.
 * @param posy Position of the text, Y coord.
 * @param units_per_px Scale in pixels; 16 is 100%.
 * @param text The text to be drawn.
 * @return
 */
TbBool AppTextDrawColourWaveResized(int posx, int posy, int units_per_px, const char *text);

TbBool AppTextDrawMissionStatus(int posx, int posy, const char *text);

#ifdef __cplusplus
};
#endif

#endif // APP_TEXT_CW_H_
/******************************************************************************/
