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

#include "bfgentab.h"
#include "bftext.h"
#include "bfkeybd.h"
#include "bfsprite.h"
#include "bftime.h"
#include "bfmemut.h"
#include "bfmouse.h"
#include "bfutility.h"
#include "bflib_joyst.h"

#include "app_gentab.h"
#include "app_sprite.h"
#include "campaign.h"
#include "display.h"
#include "febrief.h"
#include "fecntrls.h"
#include "fecryo.h"
#include "feequip.h"
#include "femail.h"
#include "fenet.h"
#include "feoptions.h"
#include "feresearch.h"
#include "feshared.h"
#include "festorage.h"
#include "guiboxes.h"
#include "guitext.h"
#include "game_data.h"
#include "game_save.h"
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

extern struct TbSprite *fe_icons_sprites;

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
extern ubyte enter_game;

extern short word_1C6F3E;
extern short word_1C6F40;
extern ubyte mo_from_agent;
extern char alert_text[200];
extern short alert_textpos;

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

short get_fe_max_detail_for_screen_res(short screen_width, short screen_height)
{
    short i, max_detail;
    max_detail = 0;
    for (i = 0; i <= MAX_SUPPORTED_SCREEN_HEIGHT/180; i++) {
        if ((320 * (i+1) > screen_width) || (240 * (i+1) > screen_height))
            break;
        max_detail = i;
    }
    return max_detail;
}

TbResult load_raw_to_screen_with_detail(const char *dir,
  const char *name, short raw_w, short raw_h, short styleno, short detail)
{
    char locstr[DISKPATH_SIZE];
    short x, y;
    TbResult ret;

    x = (lbDisplay.GraphicsScreenWidth - raw_w) / 2;
    y = (lbDisplay.GraphicsScreenHeight - raw_h) / 2;

    sprintf(locstr, "%s/%s%hd-%hd.raw", dir, name, styleno, detail);
    ret = cover_screen_rect_with_raw_file(x, y, raw_w, raw_h, locstr);
    return ret;
}

TbResult load_fe_background_up_to(const char *dir, const char *name,
  short *p_scale, short styleno, short max_detail)
{
    short detail;
    short raw_w, raw_h;
    TbResult ret;

    ret = Lb_FAIL;
    for (detail = max_detail; detail >= 0; detail--)
    {
        // Background raw image has extra 3/5 of both dimensions for aspect ratio compensation
        raw_w = 320 * (detail + 1) * 8 / 5;
        raw_h = 240 * (detail + 1) * 8 / 5;

        ret = load_raw_to_screen_with_detail(dir, name, raw_w, raw_h,
          styleno, detail);
        if (ret != Lb_FAIL)
            break;
    }
    if (detail < 0) {
        LOGERR("Some '%s%hu' raw images not loaded, tried detail %hu..0",
          name, styleno, max_detail);
        detail = 0;
    }
    if (p_scale != NULL)
        *p_scale = detail + 1;

    return ret;
}

TbResult load_fe_background_for_current_mode(void)
{
    char locname[16];
    struct Campaign *p_campgn;
    const char *campgn_mark;
    PathInfo *pinfo;
    TbResult ret;
    short max_detail;

    max_detail = get_fe_max_detail_for_screen_res(lbDisplay.GraphicsScreenWidth, lbDisplay.GraphicsScreenHeight);

    pinfo = &game_dirs[DirPlace_QData];
    p_campgn = &campaigns[background_type];
    campgn_mark = p_campgn->ProjectorFnMk;
    sprintf(locname, "proj-%s", campgn_mark);

    ret = load_fe_background_up_to(pinfo->directory, locname, NULL, 0, max_detail);
    return ret;
}

void reload_background(void)
{
    struct ScreenBufBkp bkp;

    proj_origin.X = lbDisplay.GraphicsScreenWidth / 2 - 1;
    proj_origin.Y = ((480 * 143) >> 8) + 1;
    if (screentype == SCRT_MAINMENU || screentype == SCRT_LOGIN || restore_savegame)
    {
        screen_switch_to_custom_buffer(&bkp, back_buffer,
          lbDisplay.GraphicsScreenWidth, lbDisplay.GraphicsScreenHeight);

        cover_screen_rect_with_sprite(0, 0, lbDisplay.GraphicsScreenWidth,
          lbDisplay.GraphicsScreenHeight, &fe_icons_sprites[168]);

        screen_load_backup_buffer(&bkp);
    }
    else
    {
        screen_switch_to_custom_buffer(&bkp, back_buffer,
          lbDisplay.GraphicsScreenWidth, lbDisplay.GraphicsScreenHeight);

        LbScreenClear(0);
        load_fe_background_for_current_mode();

        screen_load_backup_buffer(&bkp);
    }

    if (screentype == SCRT_EQUIP)
    {
        equip_update_for_selected_weapon();
    }
    if (screentype == SCRT_CRYO)
    {
        cryo_update_for_selected_cybmod();
    }
}

ubyte main_do_my_quit(ubyte click)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_main_do_my_quit\n"
        : "=r" (ret) : "a" (click));
    return ret;
#endif
    stop_sample_using_heap(0, 122);
    exit_game = 1;
    return 1;
}

ubyte main_do_map_editor(ubyte click)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_main_do_map_editor\n"
        : "=r" (ret) : "a" (click));
    return ret;
#endif
    map_editor = 1;
    return 1;
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

    main_login_button.AccelKey = KC_RETURN;
    main_quit_button.AccelKey = KC_ESCAPE;
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
#if 0
    ubyte ret;
    asm volatile ("call ASM_alert_OK\n"
        : "=r" (ret) : "a" (click));
    return ret;
#endif
    screentype = old_screentype;
    redraw_screen_flag = 1;
    if (old_screentype == SCRT_SYSMENU)
        enter_game = 1;
    return 1;
}

void show_alert_box(void)
{
#if 0
    asm volatile ("call ASM_show_alert_box\n"
        :  :  : "eax" );
#endif
    ubyte drawn = 0;

    if ((alert_box.Flags & 0x01) != 0)
    {
        short nlines, lnheight;

        lbFontPtr = small_med_font;
        my_set_text_window(alert_box.X + 4, alert_box.Y + 4, alert_box.Width - 8, alert_box.Height - 8);
        alert_textpos = -5;
        my_preprocess_text(alert_text);
        nlines = my_count_lines(alert_text);
        lnheight = font_height('A') + 4;
        alert_box.Y = alert_OK_button.Y - lnheight * nlines - 4;
        alert_box.Height = alert_OK_button.Height + 8 + lnheight * nlines;
    }
    asm volatile ("call *%2\n"
      : "=r" (drawn) : "a" (&alert_box), "g" (alert_box.DrawFn));
    //drawn = alert_box.DrawFn(&alert_box);
    if (drawn == 3)
    {
        lbFontPtr = small_med_font;
        my_set_text_window(alert_box.X + 4, alert_box.Y + 4, alert_box.Width - 8, alert_box.Height - 8);
        lbDisplay.DrawFlags = Lb_TEXT_HALIGN_CENTER;
        flashy_draw_text(0, 0, alert_text, 3, 0, &alert_textpos, 0);
        lbDisplay.DrawFlags = 0;
        asm volatile ("call *%2\n"
          : "=r" (drawn) : "a" (&alert_OK_button), "g" (alert_OK_button.DrawFn));
        //alert_OK_button.DrawFn(&alert_OK_button);
    }
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
    alert_OK_button.AccelKey = KC_RETURN;
}

void reset_alert_screen_boxes_flags(void)
{
    alert_box.Flags = GBxFlg_Unkn0001;
}

void set_flag01_alert_screen_boxes(void)
{
    alert_OK_button.Flags |= GBxFlg_Unkn0001;
}

void set_flag02_sysmenu_boxes(void)
{
    int i;

    unkn13_SYSTEM_button.Flags |= GBxFlg_Unkn0002;
    for (i = 0; i != SYSMNU_BUTTONS_COUNT; i++)
        sysmnu_buttons[i].Flags |= GBxFlg_Unkn0002;
}

void alert_box_text_va(const char *fmt, va_list arg)
{
    vsnprintf(alert_text, sizeof(alert_text), fmt, arg);
    show_alert = 1;
}

void alert_box_text_fmt(const char *fmt, ...)
{
    va_list val;
    va_start(val, fmt);
    alert_box_text_va(fmt, val);
    va_end(val);
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
    int i;
    ubyte sysscrn_no;
    ubyte drawn;
    ubyte v2;

    if ((game_projector_speed && is_sys_scr_shared_header_flag01()) || (is_key_pressed(KC_SPACE, KMod_DONTCARE) && !edit_flag))
    {
        clear_key_pressed(KC_SPACE);

        set_flag02_sysmenu_boxes();
        set_flag02_sys_scr_shared_boxes();
        switch (game_system_screen)
        {
        case 1:
            set_flag02_net_screen_boxes();
            break;
        case 2:
            set_flag02_storage_screen_boxes();
            break;
        case 3:
            set_flag02_controls_screen_boxes();
            break;
        case 4:
            set_flag02_audio_screen_boxes();
            break;
        case 5:
            set_flag02_gfx_screen_boxes();
            break;
        }
    }

    v2 = 1;
    sysscrn_no = 0;
    if (enter_game) {
        sysscrn_no = game_system_screen;
        enter_game = 0;
    }

    //drawn = unkn13_SYSTEM_button.DrawFn(&unkn13_SYSTEM_button); -- incompatible calling convention
    asm volatile ("call *%2\n"
        : "=r" (drawn) : "a" (&unkn13_SYSTEM_button), "g" (unkn13_SYSTEM_button.DrawFn));
    if (drawn)
    {
        for (i = 0; i < SYSMNU_BUTTONS_COUNT; i++)
        {
            if (((ingame.Flags & 0x0010) != 0) && (i == 1 || i == 2))
                continue;
            if (restore_savegame && i < 5)
                continue;
            //drawn = sysmnu_buttons[i].DrawFn(&sysmnu_buttons[i]); -- incompatible calling convention
            asm volatile ("call *%2\n"
                : "=r" (drawn) : "a" (&sysmnu_buttons[i]), "g" (sysmnu_buttons[i].DrawFn));
            if (!drawn)
                v2 = 0;
            if (enter_game) {
                sysscrn_no = i + 1;
                enter_game = 0;
            }
        }
        if (v2 && (game_system_screen != 0) && (game_system_screen < 6))
        {
            drawn = show_sys_scr_shared_header();
            if (drawn && (game_system_screen - 1) <= 4)
            {
                switch (game_system_screen)
                {
                case 1:
                    show_netgame_unkn_case1();
                    break;
                case 2:
                    show_storage_screen();
                    break;
                case 3:
                    show_options_controls_screen();
                    break;
                case 4:
                    show_options_audio_screen();
                    break;
                case 5:
                    show_options_visual_screen();
                    break;
                }
            }
        }
    }

    if (sysscrn_no)
    {
        game_system_screen = sysscrn_no;
        unkn13_SYSTEM_button.Flags &= ~(GBxFlg_TextCopied|GBxFlg_BkCopied);
        reset_sys_scr_shared_boxes_flags();
        update_sys_scr_shared_header(sysscrn_no);
        if (game_projector_speed)
        {
            set_flag02_sys_scr_shared_boxes();
        }
        switch (game_system_screen)
        {
        case 1:
            game_projector_speed = 1;
            reset_net_screen_boxes_flags();
            set_flag01_net_screen_boxes();
            break;
        case 2:
            save_slot_base = 0;
            load_save_slot_names();
            reset_storage_screen_boxes_flags();
            break;
        case 3:
            reset_controls_screen_boxes_flags();
            break;
        case 4:
            reset_options_audio_boxes_flags();
            break;
        case 5:
            reset_options_visual_boxes_flags();
            break;
        case 6:
            if (login_control__State == 5)
            {
                network_players[LbNetworkPlayerNumber()].Type = 13;
                byte_15516D = -1;
                byte_15516C = -1;
                switch_net_screen_boxes_to_initiate();
                net_unkn_func_33();
            }
            screentype = SCRT_MAINMENU;
            if (restore_savegame) {
                restore_savegame = 0;
                sysmnu_buttons[5].Y += 150;
            }
            game_system_screen = 0;
            if ((ingame.Flags & 0x0010) != 0)
                save_game_write(0, save_active_desc);
            break;
        }
        edit_flag = 0;
        reload_background_flag = 1;
    }
}

ubyte do_sysmnu_button(ubyte click)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_do_sysmnu_button\n"
        : "=r" (ret) : "a" (click));
    return ret;
#endif
    enter_game = 1;
    return 1;
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

/** Get global text pointer to a current date string.
 * @see loctext_to_gtext()
 */
static const char *main_gtext_current_date(void)
{
    char locstr[48];

    // Draw current date
    sprintf(locstr, "%02d:%02d:%02d", (int)global_date.Day,
      (int)global_date.Month, (int)global_date.Year);

    return loctext_to_gtext(locstr);
}

/** Get global text pointer to a current calendar string.
 */
static const char *main_gtext_current_calendar(void)
{
    return misc_text[3];
}

void global_date_inputs(void)
{
    if ((ingame.UserFlags & UsrF_Cheats) != 0)
    {
        if (is_key_pressed(KC_PERIOD, KMod_DONTCARE)) {
            clear_key_pressed(KC_PERIOD);
            ingame.Credits += 10000;
        }
    }
}

static void global_date_box_draw(void)
{
    const char *text;
    short cx, cy;

    cx = global_top_bar_box.X + 63;
    cy = global_top_bar_box.Y;

    lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
    draw_box_purple_list(cx + 0, cy + 0, 81, global_top_bar_box.Height, 56);
    lbDisplay.DrawFlags = Lb_SPRITE_OUTLINE;
    draw_box_purple_list(cx + 1, cy + 1, 79, global_top_bar_box.Height - 2, 247);
    lbDisplay.DrawFlags = 0;


    text = main_gtext_current_date();
    lbFontPtr = small_med_font;
    my_set_text_window(cx + 1, cy + 1, 79, global_top_bar_box.Height - 2);
    draw_text_purple_list2(3, 3, text, 0);

    text = main_gtext_current_calendar();
    lbFontPtr = small_font;
    draw_text_purple_list2(66, 5, text, 0);

}

/** Get global text pointer to a current time string.
 * @see loctext_to_gtext()
 */
static const char *main_gtext_current_time(void)
{
    char locstr[48];

    // Draw current time
    if (global_date.Hour == 0)
        sprintf(locstr, "%02d:%02d", 12, (int)global_date.Minute);
    else if (global_date.Hour > 12)
        sprintf(locstr, "%02d:%02d", (int)global_date.Hour - 12, (int)global_date.Minute);
    else
        sprintf(locstr, "%02d:%02d", (int)global_date.Hour, (int)global_date.Minute);

    return loctext_to_gtext(locstr);
}

/** Get global text pointer to a current time string.
 * @see loctext_to_gtext()
 */
static const char *main_gtext_current_part_of_day(void)
{
    char locstr[48];
    const char *subtext;

    if (global_date.Hour >= 12)
          subtext = "PM";
    else
          subtext = "AM";
    sprintf(locstr, "%s", subtext);

    return loctext_to_gtext(locstr);
}

static void global_time_box_draw(void)
{
    const char *text;
    short cx, cy;

    cx = global_top_bar_box.X + 4;
    cy = global_top_bar_box.Y;

    lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
    draw_box_purple_list(cx + 0, cy + 0, 59, global_top_bar_box.Height, 56);
    lbDisplay.DrawFlags = Lb_SPRITE_OUTLINE;
    draw_box_purple_list(cx + 1, cy + 1, 57, global_top_bar_box.Height - 2, 247);
    lbDisplay.DrawFlags = 0;

    text = main_gtext_current_time();
    lbFontPtr = small_med_font;
    my_set_text_window(cx + 1, cy + 1, 57, global_top_bar_box.Height - 2);
    draw_text_purple_list2(3, 3, text, 0);

    text = main_gtext_current_part_of_day();
    lbFontPtr = small_font;
    draw_text_purple_list2(43, 5, text, 0);
}

static void global_credits_box_draw(void)
{
    const char *text;
    char *lzstr;
    uint n;
    uint usedlen;
    char locstr[48];
    short tx;
    short cx, cy;

    cx = global_top_bar_box.X + 511;
    cy = global_top_bar_box.Y;

    lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
    draw_box_purple_list(cx + 0, cy + 0, 121, global_top_bar_box.Height, 56);
    lbDisplay.DrawFlags = Lb_SPRITE_OUTLINE;
    draw_box_purple_list(cx + 1, cy + 1, 119, global_top_bar_box.Height - 2, 247);
    lbDisplay.DrawFlags = 0;
    my_set_text_window(cx + 1, cy + 1, 119, global_top_bar_box.Height - 2);

    // Divide locstr into two parts: store credits in first, leading zeros in second
    sprintf(locstr, "%ld", ingame.Credits);

    // Fill leading zeros; drawn separately as these are half transparent
    usedlen = strlen(locstr) + 1;
    lzstr = locstr + 24;
    for (n = 0; n < 12 - (usedlen - 1); n++) {
        lzstr[n] = '0';
    }
    lzstr[n] = '\0';

    // Draw leading zeros
    text = loctext_to_gtext(lzstr);
    lbFontPtr = small_med_font;
    tx = 3;
    lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
    draw_text_purple_list2(tx, 3, text, 0);

    // Now the actual credits amount
    tx += LbTextStringWidth(text);
    text = loctext_to_gtext(locstr);
    lbDisplay.DrawFlags = 0;
    draw_text_purple_list2(tx, 3, text, 0);

    text = misc_text[1];
    lbFontPtr = small_font;
    draw_text_purple_list2(111, 5, text, 0);
}

static void global_citydrop_box_draw(void)
{
    const char *text;
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
    text = loctext_to_gtext(locstr);
    draw_text_purple_list2(3, 3, text, 0);
}

static void global_techlevel_box_draw(void)
{
    const char *text;
    char locstr[48];
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
    text = loctext_to_gtext(locstr);
    draw_text_purple_list2(3, 3, text, 0);
}

void show_date_time(void)
{
    global_date_box_draw();
    global_time_box_draw();

    if (login_control__State == 5)
    {
        global_citydrop_box_draw();
        global_techlevel_box_draw();
    }

    global_credits_box_draw();
}

void update_date_time(void)
{
    global_date_tick();
}

TbBool input_date_time(void)
{
    global_date_inputs();
    return false;
}

void reset_system_menu_boxes_flags(void)
{
    int i;

    unkn13_SYSTEM_button.Flags = GBxFlg_Unkn0001;

    for (i = 0; i < SYSMNU_BUTTONS_COUNT; i++) {
        sysmnu_buttons[i].Flags = GBxFlg_Unkn0010 | GBxFlg_Unkn0001;
    }
}

void mark_system_menu_screen_boxes_redraw(void)
{
    unkn13_SYSTEM_button.Flags &= ~(GBxFlg_BkgndDrawn|GBxFlg_TextRight|GBxFlg_BkCopied);
}

void draw_app_icon_hilight(short x, short y, ubyte iconid, ubyte aframe)
{
    struct TbSprite *spr;

    lbDisplay.DrawFlags |= 0x8000;
    spr = &fe_icons_sprites[aframe + byte_155124[iconid] + byte_15512C[iconid]];
    draw_sprite_purple_list(x, y, spr);
    lbDisplay.DrawFlags = 0;
    spr = &fe_icons_sprites[aframe + byte_155124[iconid]];
    draw_sprite_purple_list(x, y, spr);
    lbDisplay.DrawFlags = 0;
}

void draw_app_icon_normal(short x, short y, ubyte iconid, ubyte aframe)
{
    struct TbSprite *spr;

    lbDisplay.DrawFlags |= 0x8000;
    spr = &fe_icons_sprites[aframe + byte_155124[iconid] + byte_15512C[iconid]];
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
        spr = &fe_icons_sprites[79];
        draw_sprite_purple_list(x, y, spr);
        break;
    case 2:
        play_sample_using_heap(0, 112, 127, 64, 100, 0, 1);
        // fall through
    case 3:
    case 4:
    case 5:
        spr = &fe_icons_sprites[77 + aframe];
        draw_sprite_purple_list(x, y, spr);
        lbDisplay.DrawFlags = 0;
        spr = &fe_icons_sprites[96 + aframe];
        draw_sprite_purple_list(x, y, spr);
        break;
    case 6:
        spr = &fe_icons_sprites[82];
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

    spr = &fe_icons_sprites[byte_155124[iconid]];
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

    spr = &fe_icons_sprites[byte_155124[iconid]];
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

    spr = &fe_icons_sprites[79];
    if ((is_key_pressed(KC_RETURN, KMod_DONTCARE)
        && ((game_system_screen != SCRT_WORLDMAP && game_system_screen != SCRT_MISSION)
            || screentype != SCRT_SYSMENU) && !edit_flag)
        || mouse_move_over_rect(cx, cx + 1 + spr->SWidth, cy, cy + 1 + spr->SHeight))
    {
        if (!byte_1C4980 && !is_key_pressed(KC_RETURN, KMod_DONTCARE))
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

    spr = &fe_icons_sprites[79];
    if ((is_key_pressed(KC_RETURN, KMod_DONTCARE)
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
            if (word_1C498A == 50 || is_key_pressed(KC_RETURN, KMod_DONTCARE))
            {
                word_1C498A = 0;
                clear_key_pressed(KC_RETURN);
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
    char locstr[48];
    const char *text;
    short iconid;
    short tx;

    iconid = bri - word_1C6F40;
    lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
    spr = &fe_icons_sprites[102];
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
        lbDisplay.DrawFlags |= Lb_TEXT_ONE_COLOR;
    my_set_text_window(cx, cy, spr->SWidth + 2, spr->SHeight);
    draw_text_purple_list2(8, 3, misc_text[4], 0);

    lbFontPtr = med2_font;
    sprintf(locstr, "%d", brief_store[bri].RefNum);
    text = loctext_to_gtext(locstr);
    tx = (35 - LbTextStringWidth(locstr)) >> 1;
    draw_text_purple_list2(tx, 10, text, 0);

    lbFontPtr = small2_font;
    sprintf(locstr, "%02d/%02d", (int) brief_store[bri].RecvDay,
        (int) brief_store[bri].RecvMonth);
    text = loctext_to_gtext(locstr);
    tx = (35 - LbTextStringWidth(locstr)) >> 1;
    draw_text_purple_list2(tx, 23, text, 0);

    sprintf(locstr, "%02dNC", (int) brief_store[bri].RecvYear);
    text = loctext_to_gtext(locstr);
    tx = (35 - LbTextStringWidth(locstr)) >> 1;
    draw_text_purple_list2(tx, 30, text, 0);

    draw_text_purple_list2(4, 37, gui_strings[375], 0);
    lbDisplay.DrawFlags = 0;
}

TbBool get_purple_app_email_icon_inputs(short cx, short cy, short bri)
{
    struct TbSprite *spr;

    spr = &fe_icons_sprites[102];
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
            spr = &fe_icons_sprites[byte_155124[iconid]];
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
        spr = &fe_icons_sprites[102];
        cx = global_apps_bar_box.X + global_apps_bar_box.Width - spr->SWidth;
    }

    for (bri = word_1C6F40; bri < next_brief; bri++)
    {
        if (bri >= word_1C6F40 + PURPLE_APPS_EMAIL_ICONS_LIMIT)
            break;

        draw_purple_app_email_icon(cx, cy, bri);

        {
            struct TbSprite *spr;
            spr = &fe_icons_sprites[102];
            cx -= spr->SWidth + 3;
        }
    }
}

TbBool input_purple_apps_selection_bar(void)
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
            spr = &fe_icons_sprites[byte_155124[iconid]];
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
        spr = &fe_icons_sprites[102];
        cx = global_apps_bar_box.X + global_apps_bar_box.Width - spr->SWidth;
    }

    for (bri = word_1C6F40; bri < next_brief; bri++)
    {
        if (bri >= word_1C6F40 + PURPLE_APPS_EMAIL_ICONS_LIMIT)
            break;

        get_purple_app_email_icon_inputs(cx, cy, bri);

        {
            struct TbSprite *spr;
            spr = &fe_icons_sprites[102];
            cx -= spr->SWidth + 3;
        }
    }

    if (!net_unkn_pos_01b)
    {
        if (is_key_pressed(KC_F1, KMod_DONTCARE))
        {
            clear_key_pressed(KC_F1);
            change_screen = ChSCRT_SYSMENU;
        }
        if (is_key_pressed(KC_F2, KMod_DONTCARE))
        {
            clear_key_pressed(KC_F2);
            change_screen = ChSCRT_WORLDMAP;
        }
        if (is_key_pressed(KC_F3, KMod_DONTCARE))
        {
            clear_key_pressed(KC_F3);
            change_screen = ChSCRT_CRYO;
        }
        if (is_key_pressed(KC_F4, KMod_DONTCARE))
        {
            clear_key_pressed(KC_F4);
            change_screen = ChSCRT_EQUIP;
        }
        if (is_key_pressed(KC_F5, KMod_DONTCARE))
        {
            clear_key_pressed(KC_F5);
            if (research.NumBases > 0)
                change_screen = ChSCRT_RESEARCH;
        }
        if (is_key_pressed(KC_F6, KMod_DONTCARE))
        {
            clear_key_pressed(KC_F6);
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
    global_apps_bar_box.Y = lbDisplay.GraphicsWindowHeight - global_apps_bar_box.Height;
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
        if ((loading_INITIATING_box.Flags & GBxFlg_TextCopied) != 0)
            finished++;
        draw_purple_screen();

        swap_wscreen();
        game_update();
    }
    while (finished <= game_num_fps/2);

    loading_INITIATING_box.Flags = GBxFlg_Unkn0001;
    wait_for_sound_sample_finish(118);
}

TbResult load_small_font_for_current_purple_mode(void)
{
    PathInfo *pinfo;
    short max_detail;
    TbResult ret;

    max_detail = 0;//UNKN_sprites_scale / 2;
    pinfo = &game_dirs[DirPlace_Data];
    ret = load_sprites_small_font_up_to(pinfo->directory, max_detail);
    setup_sprites_small_font();
    return ret;
}

TbResult load_mapout(ubyte **pp_buf, const char *dir)
{
    char locstr[52];
    ubyte *p_buf;
    long len;
    int i;
    TbResult ret;

    p_buf = *pp_buf;
    ret = Lb_OK;

    for (i = 0; i < 6; i++)
    {
        dword_1C529C[i] = (short *)p_buf;
        sprintf(locstr, "%s/mapout%02d.dat", dir, i);
        len = LbFileLoadAt(locstr, dword_1C529C[i]);
        if (len == -1) {
            LOGERR("Could not read file '%s'", locstr);
            ret = Lb_FAIL;
            len = 64;
            LbMemorySet(p_buf, '\0', len);
        }
        p_buf += len;
    }

    landmap_2B4 = (short *)p_buf;
    sprintf(locstr, "%s/mapinsid.dat", dir);
    len = LbFileLoadAt(locstr, p_buf);
    if (len == -1) {
        ret = Lb_FAIL;
        len = 64;
        LbMemorySet(p_buf, '\0', len);
    }
    p_buf += len;

    *pp_buf = p_buf;
    return ret;
}

TbResult init_read_all_sprite_files(void)
{
    PathInfo *pinfo;
    ubyte *p_buf;
    TbResult tret, ret;

    p_buf = (ubyte *)&purple_draw_list[750];
    tret = Lb_OK;

    pinfo = &game_dirs[DirPlace_LangData];

    ret = load_sprites_fe_icons(&p_buf, pinfo->directory, 0, 1);
    if (tret == Lb_OK)
        tret = ret;

    pinfo = &game_dirs[DirPlace_Data];

    ret = load_sprites_wicons(&p_buf, pinfo->directory);
    if (tret == Lb_OK)
        tret = ret;

    ret = load_sprites_fepanel(&p_buf, pinfo->directory, 0, 0);
    if (tret == Lb_OK)
        tret = ret;

    ret = load_sprites_fe_mouse_pointers(&p_buf, pinfo->directory, 0, 1);
    if (tret == Lb_OK)
        tret = ret;

    ret = load_sprites_med_font(&p_buf, pinfo->directory, 0, 1);
    if (tret == Lb_OK)
        tret = ret;

    ret = load_sprites_big_font(&p_buf, pinfo->directory, 1, 2);
    if (tret == Lb_OK)
        tret = ret;

    ret = load_sprites_small_med_font(&p_buf, pinfo->directory, 1, 0);
    if (tret == Lb_OK)
        tret = ret;

    ret = load_sprites_med2_font(&p_buf, pinfo->directory, 2, 1);
    if (tret == Lb_OK)
        tret = ret;

    ret = load_sprites_small2_font(&p_buf, pinfo->directory, 2, 0);
    if (tret == Lb_OK)
        tret = ret;

    dword_1C6DE4 = p_buf;
    p_buf += 24480;
    dword_1C6DE8 = p_buf;
    p_buf += 24480;

    ret = load_mapout(&p_buf, pinfo->directory);
    if (tret == Lb_OK)
        tret = ret;

    // No pointer stored to frame buffer - use back_buffer minus size
    p_buf += cryo_cyborg_part_buf_max_size();
    back_buffer = p_buf;

    setup_sprites_icons();
    setup_sprites_wicons();
    setup_sprites_fepanel();
    setup_sprites_fe_mouse_pointers();
    setup_sprites_small_font();
    setup_sprites_small2_font();
    setup_sprites_small_med_font();
    setup_sprites_med_font();
    setup_sprites_med2_font();
    setup_sprites_big_font();

    if (tret == Lb_FAIL) {
        LOGERR("Some files were not loaded successfully");
        ingame.DisplayMode = DpM_UNKN_1;
    }
    return tret;
}

void init_menu_screen_colors_and_sprites(void)
{
    init_read_all_sprite_files();
    LbMouseChangeSpriteOffset(0, 0);
    LbFileLoadAt("data/s-proj.pal", display_palette);
    // Colour tables should be loaded when we can provide palette data
    LbColourTablesLoad(display_palette, "data/bgtables.dat");
    LbGhostTableGenerate(display_palette, 66, "data/startgho.dat");
    LbExtraGhostTableGenerate(display_palette, 66, 170, 170, 170,
      appixmap.ghost_add_table, "data/bgghoscr.dat");
    SCANNER_init_palette_bright();
    SCANNER_init_bright_limit_table();

    show_black_screen();
    LbPaletteSet(display_palette);
    show_black_screen();
    reload_background();
}
/******************************************************************************/
