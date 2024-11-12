/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file femain.c
 *     Front-end desktop and menu system, login screen.
 * @par Purpose:
 *     Implement functions for login screen in front-end desktop.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     22 Apr 2023 - 22 Oct 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "femain.h"

#include "bftext.h"
#include "bfkeybd.h"
#include "bfsprite.h"
#include "bftime.h"
#include "bfmemut.h"
#include "bfmouse.h"
#include "bfutility.h"
#include "bflib_joyst.h"
#include "campaign.h"
#include "display.h"
#include "femail.h"
#include "feresearch.h"
#include "guiboxes.h"
#include "guitext.h"
#include "game_speed.h"
#include "game_sprts.h"
#include "game.h"
#include "keyboard.h"
#include "network.h"
#include "player.h"
#include "purpldrw.h"
#include "purpldrwlst.h"
#include "research.h"
#include "sound.h"
#include "wrcities.h"
#include "swlog.h"
/******************************************************************************/
#define SYSMNU_BUTTONS_COUNT 6

#define PURPLE_APPS_EMAIL_ICONS_LIMIT 10

extern struct ScreenTextBox heading_box;
extern struct ScreenButton sysmnu_buttons[SYSMNU_BUTTONS_COUNT];
extern char options_title_text[];

extern struct ScreenButton main_quit_button;
extern struct ScreenButton main_login_button;
extern struct ScreenButton main_map_editor_button;
extern struct ScreenButton main_load_button;

extern struct ScreenBox alert_box;
extern struct ScreenButton alert_OK_button;

extern struct ScreenTextBox loading_INITIATING_box;

extern struct TbSprite *sprites_Icons0_0;

extern ubyte byte_155124[];
extern ubyte byte_15512C[];

extern ubyte research_curr_wep_daily_done;
extern ubyte research_curr_mod_daily_done;
extern ubyte byte_1C497D;
extern ubyte byte_1C497E;
extern ubyte month_days[12];

extern ubyte byte_1C497F;
extern ubyte byte_1C4980;
extern ubyte byte_1C4984[];
extern short word_1C498A;
extern ubyte byte_1C498C;

extern short word_1C6F3E;
extern short word_1C6F40;
extern ubyte mo_from_agent;

struct ScreenBoxBase global_top_bar_box = {4, 4, 632, 15};
struct ScreenBoxBase global_apps_bar_box = {3, 432, 634, 48};

ubyte ac_main_do_my_quit(ubyte click);
ubyte ac_main_do_login_1(ubyte click);
ubyte ac_goto_savegame(ubyte click);
ubyte ac_main_do_map_editor(ubyte click);
ubyte ac_alert_OK(ubyte click);
ubyte ac_do_sysmnu_button(ubyte click);

long time_difference(struct SynTime *tm1, struct SynTime *tm2)
{
#if 0
    asm volatile ("call ASM_time_difference\n"
        : : "a" (tm1), "d" (tm2));
    return;
#endif
    return 60 * (tm1->Hour - (long)tm2->Hour) + tm1->Minute - (long)tm2->Minute;
}

/** Increment timestamp stored in given syntime by one day.
 */
void syntime_inc_day(struct SynTime *tm)
{
    tm->Day++;
    if (tm->Day > month_days[tm->Month-1])
    {
        tm->Month++;
        tm->Day = 1;
        if (tm->Month > 12) {
            tm->Year++;
            tm->Month = 1;
            tm->Year %= 100;
        }
    }
}

ubyte main_do_my_quit(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_main_do_my_quit\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte main_do_map_editor(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_main_do_map_editor\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte main_do_login_1(ubyte click)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_main_do_login_1\n"
        : "=r" (ret) : "a" (click));
    return ret;
#endif
    screentype = SCRT_LOGIN;
    edit_flag = 1;
    reload_background_flag = 1;
    return 1;
}

void show_main_screen(void)
{
    if ((game_projector_speed && (main_quit_button.Flags & GBxFlg_Unkn0001)) ||
      (is_key_pressed(KC_SPACE, KMod_DONTCARE) && !edit_flag))
    {
        clear_key_pressed(KC_SPACE);
        main_quit_button.Flags |= GBxFlg_Unkn0002;
        main_load_button.Flags |= GBxFlg_Unkn0002;
        main_login_button.Flags |= GBxFlg_Unkn0002;
        main_map_editor_button.Flags |= GBxFlg_Unkn0002;
    }
    //main_quit_button.DrawFn(&main_quit_button); -- incompatible calling convention
    asm volatile ("call *%1\n"
        : : "a" (&main_quit_button), "g" (main_quit_button.DrawFn));
    //main_load_button.DrawFn(&main_load_button); -- incompatible calling convention
    asm volatile ("call *%1\n"
        : : "a" (&main_load_button), "g" (main_load_button.DrawFn));
    //main_login_button.DrawFn(&main_login_button); -- incompatible calling convention
    asm volatile ("call *%1\n"
        : : "a" (&main_login_button), "g" (main_login_button.DrawFn));
}

void init_main_screen_boxes(void)
{
    short scr_w;

    scr_w = lbDisplay.GraphicsWindowWidth;

    init_screen_button(&main_map_editor_button, 260, 387,
      gui_strings[443], 6, med2_font, 1, 0);
    init_screen_button(&main_load_button, 260, 358,
      gui_strings[496], 6, med2_font, 1, 0);
    init_screen_button(&main_quit_button, 260, 329,
      gui_strings[445], 6, med2_font, 1, 0);
    init_screen_button(&main_login_button, 260, 300,
      gui_strings[444], 6, med2_font, 1, 0);

    main_map_editor_button.X = (scr_w - main_map_editor_button.Width) / 2 - 1;
    main_login_button.X = (scr_w - main_login_button.Width) / 2 - 1;
    main_quit_button.X = (scr_w - main_quit_button.Width) / 2 - 1;
    main_load_button.X = (scr_w - main_load_button.Width) / 2 - 1;

    main_map_editor_button.Border = 3;
    main_login_button.Border = 3;
    main_quit_button.Border = 3;
    main_load_button.Border = 3;

    main_map_editor_button.CallBackFn = ac_main_do_map_editor;
    main_login_button.CallBackFn = ac_main_do_login_1;
    main_quit_button.CallBackFn = ac_main_do_my_quit;
    main_load_button.CallBackFn = ac_goto_savegame;

    main_login_button.AccelKey = 28;
    main_quit_button.AccelKey = 1;
}

void set_flag01_main_screen_boxes(void)
{
    main_quit_button.Flags |= GBxFlg_Unkn0001;
    main_login_button.Flags |= GBxFlg_Unkn0001;
    main_load_button.Flags |= GBxFlg_Unkn0001;
    main_map_editor_button.Flags |= GBxFlg_Unkn0001;
}

ubyte alert_OK(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_alert_OK\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

void show_alert_box(void)
{
    asm volatile ("call ASM_show_alert_box\n"
        :  :  : "eax" );
}

void init_alert_screen_boxes(void)
{
    short scr_w;
    int w;

    scr_w = lbDisplay.GraphicsWindowWidth;

    init_screen_text_box(&loading_INITIATING_box, 210u, 230u, 220u, 20,
      6, med_font, 1);
    loading_INITIATING_box.Text = gui_strings[376];

    lbFontPtr = med_font;
    loading_INITIATING_box.Height = font_height('A') + 8;
    w = my_string_width(gui_strings[376]);
    loading_INITIATING_box.Width = w + 9;
    loading_INITIATING_box.X = (scr_w - (w + 9)) / 2 - 1;
    loading_INITIATING_box.Y = 219 - (loading_INITIATING_box.Height >> 1);

    init_screen_box(&alert_box, 219u, 189u, 200u, 100, 6);
    init_screen_button(&alert_OK_button, 10u, 269u,
      gui_strings[458], 6, med2_font, 1, 0);
    alert_OK_button.CallBackFn = ac_alert_OK;

    alert_box.X = (scr_w - alert_box.Width) / 2 - 1;
    alert_OK_button.X = (scr_w - alert_OK_button.Width) / 2 - 1;
    alert_OK_button.AccelKey = 28;
}

void reset_alert_screen_boxes_flags(void)
{
    alert_box.Flags = GBxFlg_Unkn0001;
}

void set_flag01_alert_screen_boxes(void)
{
    alert_OK_button.Flags |= GBxFlg_Unkn0001;
}

ubyte show_title_box(struct ScreenTextBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_title_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

void show_sysmenu_screen(void)
{
    asm volatile ("call ASM_show_sysmenu_screen\n"
        :  :  : "eax" );
}

ubyte do_sysmnu_button(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_sysmnu_button\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

void init_system_menu_boxes(void)
{
    short scr_w, start_x;
    int i, val;
    short x, y;

    scr_w = lbDisplay.GraphicsWindowWidth;

    x = 7;
    y = 25;
    init_screen_text_box(&heading_box, x, y, 640 - 2*7, 38, 6, big_font, 1);
    heading_box.DrawTextFn = ac_show_title_box;
    heading_box.Text = options_title_text;

    start_x = (scr_w - heading_box.Width) / 2;
    heading_box.X = start_x;

    init_screen_text_box(&unkn13_SYSTEM_button, x, y, 197u, 38, 6,
      big_font, 1);
    unkn13_SYSTEM_button.Text = gui_strings[366];
    unkn13_SYSTEM_button.DrawTextFn = ac_show_title_box;

    val = 0;
    y += unkn13_SYSTEM_button.Height + 9;
    for (i = 0; i < SYSMNU_BUTTONS_COUNT; i++)
    {
        init_screen_button(&sysmnu_buttons[i], x, y,
          gui_strings[378 + val], 6, med2_font, 1, 0);
        sysmnu_buttons[i].Width = unkn13_SYSTEM_button.Width;
        sysmnu_buttons[i].Height = 21;
        sysmnu_buttons[i].CallBackFn = ac_do_sysmnu_button;
        sysmnu_buttons[i].Flags |= GBxFlg_Unkn0010;
        sysmnu_buttons[i].Border = 3;
        val++;
        y += 30;
    }

    unkn13_SYSTEM_button.X = start_x;
    for (i = 0; i < SYSMNU_BUTTONS_COUNT; i++)
    {
        sysmnu_buttons[i].X = start_x;
    }
}

void sysmnu_button_enable(int btnno, int count)
{
    int i;
    short y;

    y = sysmnu_buttons[btnno].Y;
    for (i = btnno; i < btnno+count; i++) {
        sysmnu_buttons[i].Y = y;
        y += 30;
    }
    for (i = btnno+count; i < SYSMNU_BUTTONS_COUNT; i++) {
        sysmnu_buttons[i].Y += 30 * count;
    }
}

void sysmnu_button_disable(int btnno, int count)
{
    int i;
    for (i = btnno+count; i < SYSMNU_BUTTONS_COUNT; i++) {
        sysmnu_buttons[i].Y -= 30 * count;
    }
}

void set_heading_box_text(const char *text)
{
    heading_box.Text = text;
}

void reset_heading_screen_boxes_flags(void)
{
    heading_box.Flags = GBxFlg_Unkn0001;
}

TbBool is_heading_flag01(void)
{
    return (heading_box.Flags & GBxFlg_Unkn0001) != 0;
}

void set_flag02_heading_screen_boxes(void)
{
    heading_box.Flags |= GBxFlg_Unkn0002;
}

ubyte draw_heading_box(void)
{
    ubyte drawn = true;
    //drawn = heading_box.DrawFn(&heading_box); -- incompatible calling convention
    asm volatile ("call *%2\n"
        : "=r" (drawn) : "a" (&heading_box), "g" (heading_box.DrawFn));
    return drawn;
}

void global_date_tick(void)
{
    struct TbTime curr_time;
    TbBool notable;

    LbTime(&curr_time);
    global_date.Minute = curr_time.Minute;
    global_date.Hour = curr_time.Hour;
    if (curr_time.Minute || curr_time.Hour)
    {
        if (byte_1C497D) {
            byte_1C497D = 0;
        }
    }
    else
    {
        if (!byte_1C497D) {
            byte_1C497D = 1;
            syntime_inc_day(&global_date);
        }
    }

    notable = false;

    if (research_progress_rtc_minutes > 0)
    {
        if (time_difference(&global_date, &research_curr_mod_date) >= research_progress_rtc_minutes)
        {
            if (research_curr_mod_daily_done) {
                research_curr_mod_daily_done = 0;
            }
            LbMemoryCopy(&research_curr_mod_date, &global_date, sizeof(struct SynTime));
        }
        else
        {
            if (!research_curr_mod_daily_done) {
                notable |= research_cybmod_daily_progress();
                research_curr_mod_daily_done = 1;
            }
        }
        if (time_difference(&global_date, &research_curr_wep_date) >= research_progress_rtc_minutes)
        {
            if (research_curr_wep_daily_done) {
                research_curr_wep_daily_done = 0;
            }
            LbMemoryCopy(&research_curr_wep_date, &global_date, sizeof(struct SynTime));
        }
        else
        {
            if (!research_curr_wep_daily_done) {
                notable |= research_weapon_daily_progress();
                research_curr_wep_daily_done = 1;
            }
        }
    }

    if (notable) {
        //TODO something notable happened in regard to research
        // display message? show report?
    }
}

void global_date_inputs(void)
{
    if ((ingame.UserFlags & UsrF_Cheats) != 0)
    {
        if (lbKeyOn[KC_PERIOD]) {
            lbKeyOn[KC_PERIOD] = 0;
            ingame.Credits += 10000;
        }
    }
}

static void global_date_box_draw(void)
{
    char *text;
    char locstr[50];
    short cx, cy;

    cx = global_top_bar_box.X + 63;
    cy = global_top_bar_box.Y;

    lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
    draw_box_purple_list(cx + 0, cy + 0, 81, global_top_bar_box.Height, 56);
    lbDisplay.DrawFlags = Lb_SPRITE_OUTLINE;
    draw_box_purple_list(cx + 1, cy + 1, 79, global_top_bar_box.Height - 2, 247);
    lbDisplay.DrawFlags = 0;

    // Draw current date
    sprintf(locstr, "%02d:%02d:%02d", (int)global_date.Day,
      (int)global_date.Month, (int)global_date.Year);

    lbFontPtr = small_med_font;
    my_set_text_window(cx + 1, cy + 1, 79, global_top_bar_box.Height - 2);
    text = (char *)back_buffer + text_buf_pos;
    strcpy(text, locstr);
    draw_text_purple_list2(3, 3, text, 0);

    lbFontPtr = small_font;
    text_buf_pos += strlen(locstr) + 1;
    draw_text_purple_list2(66, 5, misc_text[3], 0);

}

static void global_time_box_draw(void)
{
    char *text;
    const char *subtext;
    char locstr[50];
    short cx, cy;

    cx = global_top_bar_box.X + 4;
    cy = global_top_bar_box.Y;

    lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
    draw_box_purple_list(cx + 0, cy + 0, 59, global_top_bar_box.Height, 56);
    lbDisplay.DrawFlags = Lb_SPRITE_OUTLINE;
    draw_box_purple_list(cx + 1, cy + 1, 57, global_top_bar_box.Height - 2, 247);
    lbDisplay.DrawFlags = 0;

    // Draw current time
    if (global_date.Hour == 0)
        sprintf(locstr, "%02d:%02d", 12, (int)global_date.Minute);
    else if (global_date.Hour > 12)
        sprintf(locstr, "%02d:%02d", (int)global_date.Hour - 12, (int)global_date.Minute);
    else
        sprintf(locstr, "%02d:%02d", (int)global_date.Hour, (int)global_date.Minute);

    lbFontPtr = small_med_font;
    my_set_text_window(cx + 1, cy + 1, 57, global_top_bar_box.Height - 2);
    text = (char *)back_buffer + text_buf_pos;
    strcpy(text, locstr);
    draw_text_purple_list2(3, 3, text, 0);
    text_buf_pos += strlen(locstr) + 1;

    if (global_date.Hour >= 12)
          subtext = "PM";
    else
          subtext = "AM";
    sprintf(locstr, "%s", subtext);

    lbFontPtr = small_font;
    text = (char *)back_buffer + text_buf_pos;
    strcpy(text, locstr);
    text_buf_pos += strlen(locstr) + 1;
    draw_text_purple_list2(43, 5, text, 0);
}

static void global_credits_box_draw(void)
{
    char *text;
    uint n;
    uint usedlen;
    char locstr[50];
    short tx;
    short cx, cy;

    cx = global_top_bar_box.X + 511;
    cy = global_top_bar_box.Y;

    lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
    draw_box_purple_list(cx + 0, cy + 0, 121, global_top_bar_box.Height, 56);
    lbDisplay.DrawFlags = Lb_SPRITE_OUTLINE;
    draw_box_purple_list(cx + 1, cy + 1, 119, global_top_bar_box.Height - 2, 247);
    lbDisplay.DrawFlags = 0;

    // Draw credits amount
    lbFontPtr = small_med_font;
    my_set_text_window(cx + 1, cy + 1, 119, global_top_bar_box.Height - 2);
    tx = 3;

    sprintf(locstr, "%ld", ingame.Credits);

    // Leading zeros are half transparent
    usedlen = strlen(locstr) + 1;
    text = (char *)back_buffer + text_buf_pos;
    for (n = 0; n < 12 - (usedlen - 1); n++) {
        text[n] = '0';
    }
    text[n] = '\0';
    text_buf_pos += n + 1;

    lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
    draw_text_purple_list2(tx, 3, text, 0);
    lbDisplay.DrawFlags = 0;
    tx += LbTextStringWidth(text);

    // Now the actual credits amount
    text = (char *)back_buffer + text_buf_pos;
    strcpy(text, locstr);
    draw_text_purple_list2(tx, 3, text, 0);
    lbFontPtr = small_font;
    text_buf_pos += strlen(locstr) + 1;
    draw_text_purple_list2(111, 5, misc_text[1], 0);
}

static void global_citydrop_box_draw(void)
{
    char *text;
    uint n;
    const char *subtext;
    char locstr[50];
    short cx, cy;

    cx = global_top_bar_box.X + 148;
    cy = global_top_bar_box.Y;

    lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
    draw_box_purple_list(cx + 0, cy + 0, 200, global_top_bar_box.Height, 56);
    lbDisplay.DrawFlags = Lb_SPRITE_OUTLINE;
    draw_box_purple_list(cx + 1, cy + 1, 198, global_top_bar_box.Height - 2, 247);
    lbDisplay.DrawFlags = 0;

    lbFontPtr = small_med_font;
    my_set_text_window(cx + 1, cy + 1, 198, global_top_bar_box.Height - 2);

    if (login_control__City == -1) {
        subtext = "";
    } else {
        unkn_city_no = login_control__City;
        n = cities[unkn_city_no].TextIndex[0];
        subtext = (char *)&memload[n];
    }
    sprintf(locstr, "%s: %s", gui_strings[446], subtext);
    text = (char *)back_buffer + text_buf_pos;
    strcpy(text, locstr);
    text_buf_pos += strlen(locstr) + 1;
    draw_text_purple_list2(3, 3, text, 0);
}

static void global_techlevel_box_draw(void)
{
    char *text;
    char locstr[50];
    short cx, cy;

    cx = global_top_bar_box.X + 352;
    cy = global_top_bar_box.Y;

    lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
    draw_box_purple_list(cx + 0, cy + 0, 156, global_top_bar_box.Height, 56);
    lbDisplay.DrawFlags = Lb_SPRITE_OUTLINE;
    draw_box_purple_list(cx + 1, cy + 1, 154, global_top_bar_box.Height - 2, 247);
    lbDisplay.DrawFlags = 0;

    lbFontPtr = small_med_font;
    my_set_text_window(cx + 1, cy + 1, 154, global_top_bar_box.Height - 2);

    sprintf(locstr, "%s: %d", gui_strings[447], login_control__TechLevel);
    text = (char *)back_buffer + text_buf_pos;
    strcpy(text, locstr);
    text_buf_pos += strlen(locstr) + 1;
    draw_text_purple_list2(3, 3, text, 0);
}

void show_date_time(void)
{
#if 0
    asm volatile ("call ASM_show_date_time\n"
        :  :  : "eax" );
    return;
#endif
    global_date_box_draw();
    global_time_box_draw();

    if (login_control__State == 5)
    {
        global_citydrop_box_draw();
        global_techlevel_box_draw();
    }

    global_credits_box_draw();

    global_date_tick();
    global_date_inputs();

}

void reset_system_menu_boxes_flags(void)
{
    int i;

    unkn13_SYSTEM_button.Flags = GBxFlg_Unkn0001;

    for (i = 0; i < SYSMNU_BUTTONS_COUNT; i++) {
        sysmnu_buttons[i].Flags = GBxFlg_Unkn0010 | GBxFlg_Unkn0001;
    }
}

void clear_someflags_system_menu_screen_boxes(void)
{
    unkn13_SYSTEM_button.Flags &= ~(GBxFlg_BkgndDrawn|GBxFlg_TextRight|GBxFlg_Unkn0004);
}

void draw_app_icon_hilight(short x, short y, ubyte iconid, ubyte aframe)
{
    struct TbSprite *spr;

    lbDisplay.DrawFlags |= 0x8000;
    spr = &sprites_Icons0_0[aframe + byte_155124[iconid] + byte_15512C[iconid]];
    draw_sprite_purple_list(x, y, spr);
    lbDisplay.DrawFlags = 0;
    spr = &sprites_Icons0_0[aframe + byte_155124[iconid]];
    draw_sprite_purple_list(x, y, spr);
    lbDisplay.DrawFlags = 0;
}

void draw_app_icon_normal(short x, short y, ubyte iconid, ubyte aframe)
{
    struct TbSprite *spr;

    lbDisplay.DrawFlags |= 0x8000;
    spr = &sprites_Icons0_0[aframe + byte_155124[iconid] + byte_15512C[iconid]];
    draw_sprite_purple_list(x, y, spr);
    lbDisplay.DrawFlags = 0;
}

void draw_unread_email_icon(short x, short y, ubyte aframe)
{
    struct TbSprite *spr;

    lbDisplay.DrawFlags |= 0x8000;
    switch (aframe)
    {
    case 1:
        spr = &sprites_Icons0_0[79];
        draw_sprite_purple_list(x, y, spr);
        break;
    case 2:
        play_sample_using_heap(0, 112, 127, 64, 100, 0, 1);
        // fall through
    case 3:
    case 4:
    case 5:
        spr = &sprites_Icons0_0[77 + aframe];
        draw_sprite_purple_list(x, y, spr);
        lbDisplay.DrawFlags = 0;
        spr = &sprites_Icons0_0[96 + aframe];
        draw_sprite_purple_list(x, y, spr);
        break;
    case 6:
        spr = &sprites_Icons0_0[82];
        draw_sprite_purple_list(x, y, spr);
        lbDisplay.DrawFlags = 0;
        break;
    default:
        break;
    }
    lbDisplay.DrawFlags = 0;
}

TbBool is_purple_alert_on_top(void)
{
    return (screentype == SCRT_ALERTBOX);
}

TbBool is_purple_apps_selection_bar_visible(void)
{
    return (screentype != SCRT_MAINMENU) && (screentype != SCRT_LOGIN) && !restore_savegame;
}

void draw_purple_app_utility_icon(short cx, short cy, short iconid)
{
    struct TbSprite *spr;

    spr = &sprites_Icons0_0[byte_155124[iconid]];
    if (mouse_move_over_rect(cx, cx + 1 + spr->SWidth, cy, cy + 1 + spr->SHeight))
    {
        if ((byte_1C497E & (1 << iconid)) == 0) {
            byte_1C497E |= (1 << iconid);
            play_sample_using_heap(0, 123, 127, 64, 100, 0, 1);
        }
        lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
        // If clicked, draw the icon without transparency
        if (lbDisplay.MLeftButton || (joy.Buttons[0] && !net_unkn_pos_02))
        {
            lbDisplay.DrawFlags = 0;
        }
        if (lbDisplay.MRightButton || (joy.Buttons[0] && !net_unkn_pos_02))
        {
            lbDisplay.DrawFlags = 0;
        }
        draw_app_icon_hilight(cx, cy, iconid, byte_1C4984[iconid]);
        byte_1C4984[iconid]++;
        if (byte_1C4984[iconid] == byte_15512C[iconid])
            byte_1C4984[iconid] = 0;
    }
    else
    {
        byte_1C497E &= ~(1 << iconid);
        lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
        draw_app_icon_normal(cx, cy, iconid, byte_1C4984[iconid]);
        if (byte_1C4984[iconid])
        {
            byte_1C4984[iconid]++;
            if (byte_1C4984[iconid] == byte_15512C[iconid])
                byte_1C4984[iconid] = 0;
        }
        if (word_1C498A == 2 * (iconid + 1)
            || word_1C498A == 2 * (iconid + 1) + 1)
            word_1C498A = 0;
    }
    lbDisplay.DrawFlags = 0;
}

TbBool get_purple_app_utility_icon_inputs(short cx, short cy, short iconid)
{
    struct TbSprite *spr;

    spr = &sprites_Icons0_0[byte_155124[iconid]];
    if (mouse_move_over_rect(cx, cx + 1 + spr->SWidth, cy, cy + 1 + spr->SHeight))
    {
        if (lbDisplay.MLeftButton || (joy.Buttons[0] && !net_unkn_pos_02))
        {
            lbDisplay.LeftButton = 0;
            word_1C498A = 2 * (iconid + 1);
        }
        else if (word_1C498A == 2 * (iconid + 1) + 0)
        {
            if (mo_weapon != -1 && mo_weapon == research.CurrentWeapon)
            {
                player_cryo_remove_weapon_one(mo_from_agent, mo_weapon + 1);
                research_unkn_func_003();
                mo_weapon = -1;
            }
            else
            {
                change_screen = iconid + 1;
                play_sample_using_heap(0, 111, 127, 64, 100, 0, 2);
            }
            word_1C498A = 0;
        }

        if (lbDisplay.MRightButton || (joy.Buttons[0] && !net_unkn_pos_02))
        {
            lbDisplay.RightButton = 0;
            word_1C498A = 2 * (iconid + 1) + 1;
        }
        else if (word_1C498A == 2 * (iconid + 1) + 1)
        {
            change_screen = iconid + 1;
            play_sample_using_heap(0, 111, 127, 64, 100, 0, 2);

            word_1C498A = 0;
        }
    }
    // If not mouse over, make sure to clear the state
    else if (word_1C498A == 2 * (iconid + 1) + 0)
        word_1C498A = 0;
    else if (word_1C498A == 2 * (iconid + 1) + 1)
        word_1C498A = 0;

    return false;
}

void draw_purple_app_unread_email_icon(short cx, short cy)
{
    struct TbSprite *spr;

    spr = &sprites_Icons0_0[79];
    if ((lbKeyOn[KC_RETURN]
        && ((game_system_screen != SCRT_WORLDMAP && game_system_screen != SCRT_MISSION)
            || screentype != SCRT_SYSMENU) && !edit_flag)
        || mouse_move_over_rect(cx, cx + 1 + spr->SWidth, cy, cy + 1 + spr->SHeight))
    {
        if (!byte_1C4980 && !lbKeyOn[KC_RETURN])
        {
            byte_1C4980 = 1;
            play_sample_using_heap(0, 123, 127, 64, 100, 0, 1);
        }
        lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
        // If clicked, draw the icon without transparency
        if (lbDisplay.MLeftButton || (joy.Buttons[0] && !net_unkn_pos_02))
        {
            lbDisplay.DrawFlags = 0;
        }
        draw_unread_email_icon(cx, cy, byte_1C498C);
        if (gameturn & 1)
        {
            if (++byte_1C498C > 5)
                byte_1C498C = 2;
        }
    }
    else
    {
        byte_1C4980 = 0;
        lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
        draw_unread_email_icon(cx, cy, byte_1C498C);
        if (gameturn & 1)
        {
            if (++byte_1C498C > 6)
                byte_1C498C = 0;
        }
    }
    lbDisplay.DrawFlags = 0;
}

TbBool get_purple_app_unread_email_icon_inputs(short cx, short cy)
{
    struct TbSprite *spr;
    const char *subtext;

    spr = &sprites_Icons0_0[79];
    if ((lbKeyOn[KC_RETURN]
        && ((game_system_screen != SCRT_WORLDMAP && game_system_screen != SCRT_MISSION)
            || screentype != SCRT_SYSMENU) && !edit_flag)
        || mouse_move_over_rect(cx, cx + 1 + spr->SWidth, cy, cy + 1 + spr->SHeight))
    {
        if (lbDisplay.MLeftButton || (joy.Buttons[0] && !net_unkn_pos_02))
        {
            lbDisplay.LeftButton = 0;
            word_1C498A = 50;
        }
        else
        {
            if (word_1C498A == 50 || lbKeyOn[KC_RETURN])
            {
                word_1C498A = 0;
                lbKeyOn[KC_RETURN] = 0;
                if (!is_purple_alert_on_top())
                {
                    if (activate_queued_mail() == 1)
                    {
                        word_1C6F40 = next_brief - 5;
                        if (word_1C6F40 < 0)
                            word_1C6F40 = 0;
                        open_brief = next_brief;
                        change_screen = ChSCRT_MISSION;
                        subtext = gui_strings[372];
                    }
                    else
                    {
                        word_1C6F3E = next_email - 4;
                        if (word_1C6F3E < 0)
                            word_1C6F3E = 0;
                        change_screen = ChSCRT_MISSION;
                        subtext = gui_strings[373];
                        open_brief = -next_email;
                    }
                    set_heading_box_text(subtext);
                    play_sample_using_heap(0, 111, 127, 64, 100, 0, 2);
                    if (new_mail)
                    {
                        play_sample_using_heap(0,
                          119 + (LbRandomAnyShort() % 3), 127, 64, 100, 0, 3);
                    }
                    else
                    {
                        byte_1C4980 = new_mail;
                    }
                }
            }
        }
    }
    // If not mouse over, make sure to clear the state
    else if (word_1C498A == 50)
        word_1C498A = 0;

    return false;
}

void draw_purple_app_email_icon(short cx, short cy, short bri)
{
    struct TbSprite *spr;
    char locstr[52];
    char *text;
    short iconid;
    short tx;

    iconid = bri - word_1C6F40;
    lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
    spr = &sprites_Icons0_0[102];
    if (mouse_move_over_rect(cx, cx + spr->SWidth + 1, cy,
        cy + 1 + spr->SHeight))
    {
        if ((byte_1C497F & (1 << iconid)) == 0)
        {
            byte_1C497F |= (1 << iconid);
            play_sample_using_heap(0, 123, 127, 64, 100, 0, 1u);
        }
        // If clicked, draw the icon without transparency
        if (lbDisplay.MLeftButton || (joy.Buttons[0] && !net_unkn_pos_02))
        {
            lbDisplay.DrawFlags = 0;
        }
        lbDisplay.DrawFlags |= 0x8000;
        draw_sprite_purple_list(cx, cy, spr);
        lbDisplay.DrawFlags = 0;
    }
    else
    {
        byte_1C497F &= ~(1 << iconid);
        lbDisplay.DrawFlags |= 0x8000;
        draw_sprite_purple_list(cx, cy, spr);
        lbDisplay.DrawFlags &= ~0x8000;
    }
    lbFontPtr = small2_font;
    lbDisplay.DrawColour = 87;
    if (mission_remain_until_success(brief_store[bri].Mission))
        lbDisplay.DrawFlags |= 0x0040;
    my_set_text_window(cx, cy, spr->SWidth + 2, spr->SHeight);
    draw_text_purple_list2(8, 3, misc_text[4], 0);

    lbFontPtr = med2_font;
    sprintf(locstr, "%d", brief_store[bri].RefNum);
    tx = (35 - LbTextStringWidth(locstr)) >> 1;
    text = (char*) back_buffer + text_buf_pos;
    strcpy(text, locstr);
    text_buf_pos += strlen(locstr) + 1;
    draw_text_purple_list2(tx, 10, text, 0);

    lbFontPtr = small2_font;
    sprintf(locstr, "%02d/%02d", (int) brief_store[bri].RecvDay,
        (int) brief_store[bri].RecvMonth);
    tx = (35 - LbTextStringWidth(locstr)) >> 1;
    text = (char*) back_buffer + text_buf_pos;
    strcpy(text, locstr);
    draw_text_purple_list2(tx, 23, text, 0);
    text_buf_pos += strlen(locstr) + 1;
    sprintf(locstr, "%02dNC", (int) brief_store[bri].RecvYear);

    tx = (35 - LbTextStringWidth(locstr)) >> 1;
    text = (char*) back_buffer + text_buf_pos;
    strcpy(text, locstr);
    text_buf_pos += strlen(locstr) + 1;
    draw_text_purple_list2(tx, 30, text, 0);
    draw_text_purple_list2(4, 37, gui_strings[375], 0);
    lbDisplay.DrawFlags = 0;
}

TbBool get_purple_app_email_icon_inputs(short cx, short cy, short bri)
{
    struct TbSprite *spr;

    spr = &sprites_Icons0_0[102];
    if (mouse_move_over_rect(cx, cx + spr->SWidth + 1, cy,
        cy + 1 + spr->SHeight))
    {
        if (lbDisplay.MLeftButton || (joy.Buttons[0] && !net_unkn_pos_02))
        {
            lbDisplay.LeftButton = 0;
            word_1C498A = 2 * (bri + 1) + 100;
        }
        else if (word_1C498A == 2 * (bri + 1) + 100)
        {
            change_screen = ChSCRT_MISSION;
            set_heading_box_text(gui_strings[372]);
            open_brief = bri + 1;
            play_sample_using_heap(0, 111, 127, 64, 100, 0, 2);

            word_1C498A = 0;
        }
    }
    // If not mouse over, make sure to clear the state
    else if (word_1C498A == 2 * (bri + 1) + 100)
        word_1C498A = 0;

    return false;
}

TbBool is_purple_apps_utility_space_reserved(short iconid)
{
    return  (iconid != ApBar_PANET);
}

TbBool is_purple_apps_utility_visible(short iconid)
{
    // Show research icon only if the player has research facility
    if ((iconid == ApBar_RESEARCH) && (research.NumBases == 0))
        return false;

    if (login_control__State == 5)
    {
        TbBool visible;

        if (is_unkn_current_player()) {
            visible = (iconid != ApBar_PANET && iconid != ApBar_RESEARCH);
        } else {
            visible = (iconid != ApBar_PANET &&
              iconid != ApBar_WORLDMAP &&
              iconid != ApBar_RESEARCH);
        }
        if ((unkn_flags_08 & 0x02) == 0 || (unkn_flags_08 & 0x01) == 0)
            visible = (iconid == ApBar_SYSTEM);
        return visible;
    }

    // Completely hide Public Access Network icon
    return (iconid != ApBar_PANET);
}

/** Show a collection of icons at bottom of the screen.
 */
void show_purple_apps_selection_bar(void)
{
#if 0
    asm volatile ("call ASM_show_purple_apps_selection_bar\n"
        :  :  : "eax" );
    return;
#endif
    ushort bri;
    short iconid;
    short cx, cy;

    // Show utility icons in bottom left
    cx = global_apps_bar_box.X;
    cy = global_apps_bar_box.Y;

    for (iconid = 0; iconid < 6; iconid++)
    {
        if (is_purple_apps_utility_visible(iconid))
        {
            draw_purple_app_utility_icon(cx, cy, iconid);
        }
        if (is_purple_apps_utility_space_reserved(iconid))
        {
            struct TbSprite *spr;
            spr = &sprites_Icons0_0[byte_155124[iconid]];
            cx += spr->SWidth + 3;
        }
    }

    // Show unread mail notification icon
    if (new_mail
        && (game_system_screen != SCRT_MISSION || screentype != SCRT_SYSMENU))
    {
        draw_purple_app_unread_email_icon(cx, cy);
    }

    // Show email icons in bottom right
    {
        struct TbSprite *spr;
        spr = &sprites_Icons0_0[102];
        cx = global_apps_bar_box.X + global_apps_bar_box.Width - spr->SWidth;
    }

    for (bri = word_1C6F40; bri < next_brief; bri++)
    {
        if (bri >= word_1C6F40 + PURPLE_APPS_EMAIL_ICONS_LIMIT)
            break;

        draw_purple_app_email_icon(cx, cy, bri);

        {
            struct TbSprite *spr;
            spr = &sprites_Icons0_0[102];
            cx -= spr->SWidth + 3;
        }
    }
}

TbBool get_purple_apps_selection_bar_inputs(void)
{
    short iconid;
    ushort bri;
    short cx, cy;

    // Get inputs from utility icons in bottom left
    cx = global_apps_bar_box.X;
    cy = global_apps_bar_box.Y;

    for (iconid = 0; iconid < 6; iconid++)
    {
        if (is_purple_apps_utility_visible(iconid))
        {
            if (!is_purple_alert_on_top())
                get_purple_app_utility_icon_inputs(cx, cy, iconid);
        }
        if (is_purple_apps_utility_space_reserved(iconid))
        {
            struct TbSprite *spr;
            spr = &sprites_Icons0_0[byte_155124[iconid]];
            cx += spr->SWidth + 3;
        }
    }

    // Get inputs from unread mail notification icon
    if (new_mail
        && (game_system_screen != SCRT_MISSION || screentype != SCRT_SYSMENU))
    {
        get_purple_app_unread_email_icon_inputs(cx, cy);
    }

    // Get inputs from email icons in bottom right
    {
        struct TbSprite *spr;
        spr = &sprites_Icons0_0[102];
        cx = global_apps_bar_box.X + global_apps_bar_box.Width - spr->SWidth;
    }

    for (bri = word_1C6F40; bri < next_brief; bri++)
    {
        if (bri >= word_1C6F40 + PURPLE_APPS_EMAIL_ICONS_LIMIT)
            break;

        get_purple_app_email_icon_inputs(cx, cy, bri);

        {
            struct TbSprite *spr;
            spr = &sprites_Icons0_0[102];
            cx -= spr->SWidth + 3;
        }
    }

    if (!net_unkn_pos_01b)
    {
        if (lbKeyOn[KC_F1])
        {
            lbKeyOn[KC_F1] = 0;
            change_screen = ChSCRT_SYSMENU;
        }
        if (lbKeyOn[KC_F2])
        {
            lbKeyOn[KC_F2] = 0;
            change_screen = ChSCRT_WORLDMAP;
        }
        if (lbKeyOn[KC_F3])
        {
            lbKeyOn[KC_F3] = 0;
            change_screen = ChSCRT_CRYO;
        }
        if (lbKeyOn[KC_F4])
        {
            lbKeyOn[KC_F4] = 0;
            change_screen = ChSCRT_EQUIP;
        }
        if (lbKeyOn[KC_F5])
        {
            lbKeyOn[KC_F5] = 0;
            if (research.NumBases > 0)
                change_screen = ChSCRT_RESEARCH;
        }
        if (lbKeyOn[KC_F6])
        {
            lbKeyOn[KC_F6] = 0;
            if (open_brief != 0)
                change_screen = ChSCRT_MISSION;
        }
    }

    return false;
}

void init_global_boxes(void)
{
    short scr_w, start_x;

    scr_w = lbDisplay.GraphicsWindowWidth;
    start_x = (scr_w - global_top_bar_box.Width) / 2;
    global_top_bar_box.X = start_x;
    global_top_bar_box.Y = 4;
    start_x = (scr_w - global_apps_bar_box.Width) / 2;
    global_apps_bar_box.X = start_x;
    global_apps_bar_box.Y = 432;
}

void show_mission_loading_screen(void)
{
    LbMouseChangeSprite(0);
    reload_background();
    play_sample_using_heap(0, 118, 127, 64, 100, 0, 3);

    ulong finished = 0; // Amount of frames after the drawing animation finished
    do
    {
        memcpy(lbDisplay.WScreen, back_buffer, lbDisplay.GraphicsScreenWidth * lbDisplay.GraphicsScreenHeight);
        text_buf_pos = lbDisplay.GraphicsScreenWidth * lbDisplay.GraphicsScreenHeight;
        if ((0 != game_projector_speed && (loading_INITIATING_box.Flags & GBxFlg_Unkn0001))
          || (is_key_pressed(KC_SPACE, KMod_DONTCARE) && !edit_flag)) {
            clear_key_pressed(KC_SPACE);
            loading_INITIATING_box.Flags |= GBxFlg_Unkn0002;
        }
        //loading_INITIATING_box.DrawFn(&loading_INITIATING_box); -- incompatible calling convention
        asm volatile ("call *%1\n"
            : : "a" (&loading_INITIATING_box), "g" (loading_INITIATING_box.DrawFn));
        if ((loading_INITIATING_box.Flags & GBxFlg_Unkn1000) != 0)
            finished++;
        draw_purple_screen();

        swap_wscreen();
        game_update();
    }
    while (finished <= game_num_fps/2);

    loading_INITIATING_box.Flags = GBxFlg_Unkn0001;
    wait_for_sound_sample_finish(118);
}

/******************************************************************************/
