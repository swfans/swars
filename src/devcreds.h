/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file devcreds.h
 *     Header file for devcreds.c.
 * @par Purpose:
 *     Developer team credits list display.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     17 Sep 2023 - 17 Oct 2026
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef DEVCREDS_H
#define DEVCREDS_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct TbSprite;

#pragma pack()
/******************************************************************************/
extern u32 colwavapr_timer;
extern u32 colwavapr_reveal_tm;
extern TbBool colwav_enable;
/******************************************************************************/

int load_devteam_credits_text(ubyte *buf);
void draw_colwavapr_text_paragraph(int scr_x, int *p_scr_y,
  const struct TbSprite *p_font, const char *text);

void devteam_credits_show(void);

void init_outro(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
