/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file mydraw.h
 *     Header file for mydraw.c.
 * @par Purpose:
 *     Modified drawing routines adjusted for the specific app.
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
#ifndef MYDRAW_H
#define MYDRAW_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

#pragma pack(1)


#pragma pack()
/******************************************************************************/

ubyte my_char_to_upper(ubyte c);
int font_word_length(const char *text);
ushort my_draw_text(short x, short y, const char *text, ushort startline);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
