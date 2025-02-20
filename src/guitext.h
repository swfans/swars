/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file guitext.h
 *     Header file for guitext.c.
 * @par Purpose:
 *     GUI text for menu and in-game display.
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
#ifndef GUITEXT_H
#define GUITEXT_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#define STRINGS_MAX 652

#pragma pack(1)


#pragma pack()
/******************************************************************************/
extern char *gui_strings[STRINGS_MAX];
extern char *gui_strings_data;
extern char *gui_strings_data_end;
extern ulong text_buf_pos;

void snprint_dh_time_duration(char *out, ulong outlen, long ndays, short nhours);

/** Converts any kind of text pointer to a global text pointer.
 *
 * Global text pointers are pointers within scratch buffers, valid for
 * whole drawing the current frame. Such pointers can be used in drawlists.
 */
const char *loctext_to_gtext(const char *ltext);

void read_strings_file(void);
TbBool create_strings_list(char **strings, char *strings_data, char *strings_data_end);

TbBool flashy_draw_text(int x, int y, const char *text, ubyte speed,
  int top_line, short *textpos, int cyan_flag);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
