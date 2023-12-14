/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file femain.h
 *     Header file for femain.c.
 * @par Purpose:
 *     Front-end desktop and menu system, login screen.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     27 Sep 2023 - 11 Dec 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef FEMAIN_H
#define FEMAIN_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct ScreenTextBox;

#pragma pack()
/******************************************************************************/

void show_main_screen(void);
void init_main_screen_boxes(void);
void set_flag01_main_screen_boxes(void);

void show_alert_box(void);
void init_alert_screen_boxes(void);
void reset_alert_screen_boxes_flags(void);
void set_flag01_alert_screen_boxes(void);

ubyte ac_show_title_box(struct ScreenTextBox *box);

void init_system_menu_boxes(void);

/** Re-enables a button from system menu, moving the following buttons down.
 */
void sysmnu_button_enable(int btnno, int count);

/** Disables a button from system menu, moving the buttons below to fill its space.
 */
void sysmnu_button_disable(int btnno, int count);

void set_heading_box_text(const char *text);
void reset_heading_screen_boxes_flags(void);
TbBool is_heading_flag01(void);
int draw_heading_box(void);
void set_flag02_heading_screen_boxes(void);
void reset_system_menu_boxes_flags(void);
void clear_someflags_system_menu_screen_boxes(void);

void show_mission_loading_screen(void);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
