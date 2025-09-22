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

enum SysScreenType {
  SySc_NONE = 0x0,
  SySc_NETGAME,
  SySc_STORAGE,
  SySc_CONTROLS,
  SySc_AUDIO_OPTS,
  SySc_GFX_OPTS,
  SySc_TYPES_COUNT,
};

/** Special value whch does not have system screen,
 * but commands to logout to main menu.
 */
#define SySc_LOGOUT SySc_TYPES_COUNT

struct SynTime {
    ubyte Minute;
    ubyte Hour;
    ubyte Day;
    ubyte Month;
    ubyte Year;
};

struct ScreenTextBox;

#pragma pack()
/******************************************************************************/
extern struct ScreenTextBox unkn13_SYSTEM_button;

extern struct SynTime global_date;
extern struct SynTime research_curr_wep_date;
extern struct SynTime research_curr_mod_date;

short get_fe_max_detail_for_screen_res(short screen_width, short screen_height);
void reload_background(void);

void show_main_screen(void);
void init_main_screen_boxes(void);
void set_flag01_main_screen_boxes(void);

TbBool is_purple_alert_on_top(void);
void show_alert_box(void);
void init_alert_screen_boxes(void);
void reset_alert_screen_boxes_flags(void);
void set_flag01_alert_screen_boxes(void);
void alert_box_text_fmt(const char *fmt, ...);

ubyte ac_show_title_box(struct ScreenTextBox *box);

void init_system_menu_boxes(void);
void init_global_boxes(void);
TbResult load_small_font_for_current_purple_mode(void);
void init_menu_screen_colors_and_sprites(void);

/** Re-enables a button from system menu, moving the following buttons down.
 */
void sysmnu_button_enable(int btnno, int count);

/** Disables a button from system menu, moving the buttons below to fill its space.
 */
void sysmnu_button_disable(int btnno, int count);

void set_heading_box_text(const char *text);
void reset_heading_screen_boxes_flags(void);
TbBool is_heading_flag01(void);
ubyte draw_heading_box(void);
void set_flag02_heading_screen_boxes(void);
void reset_system_menu_boxes_flags(void);
void mark_system_menu_screen_boxes_redraw(void);

void show_mission_loading_screen(void);

void show_date_time(void);
void update_date_time(void);
TbBool input_date_time(void);

void show_sysmenu_screen(void);

TbBool is_purple_apps_selection_bar_visible(void);
void show_purple_apps_selection_bar(void);
TbBool input_purple_apps_selection_bar(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
