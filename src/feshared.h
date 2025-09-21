/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file feshared.h
 *     Header file for feshared.c.
 * @par Purpose:
 *     Front-end desktop and menu system, items shared between screens.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     22 Apr 2024 - 03 Sep 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef FESHARED_H
#define FESHARED_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct ScreenBox;

#pragma pack()
/******************************************************************************/
extern struct ScreenTextBox system_screen_shared_header_box;
extern struct ScreenBox system_screen_shared_content_box;

TbBool is_sys_scr_shared_header_flag01(void);
void mark_sys_scr_shared_header_box_redraw(void);
void update_sys_scr_shared_header(ubyte sysscrn_no);
ubyte show_sys_scr_shared_header(void);

void init_sys_scr_shared_content_box(void);
void mark_sys_scr_shared_content_box_redraw(void);

void reset_sys_scr_shared_boxes_flags(void);
void set_flag02_sys_scr_shared_boxes(void);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
