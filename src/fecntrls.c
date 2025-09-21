/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file fecntrls.c
 *     Front-end desktop and menu system, research screen.
 * @par Purpose:
 *     Implement functions for research screen in front-end desktop.
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
#include "fecntrls.h"

#include "bfkeybd.h"
#include "bftext.h"
#include "bfutility.h"
#include "bflib_joyst.h"

#include "femain.h"
#include "feshared.h"
#include "guiboxes.h"
#include "guitext.h"
#include "display.h"
#include "game_data.h"
#include "game_save.h"
#include "game_speed.h"
#include "game_sprts.h"
#include "game.h"
#include "keyboard.h"
#include "network.h"
#include "player.h"
#include "purpldrw.h"
#include "swlog.h"
/******************************************************************************/
extern struct ScreenBox controls_joystick_box;
struct ScreenTextBox controls_list_box;
extern struct ScreenButton controls_defaults_button;
extern struct ScreenButton controls_save_button;
extern struct ScreenButton controls_calibrate_button;

extern ubyte byte_1C4970;
extern ubyte controls_hlight_gkey;

ubyte ac_do_controls_defaults(ubyte click);
ubyte ac_do_controls_save(ubyte click);
ubyte ac_do_controls_calibrate(ubyte click);
ubyte ac_show_menu_controls_list_box(struct ScreenTextBox *p_box);

ubyte do_controls_defaults(ubyte click)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_do_controls_defaults\n"
        : "=r" (ret) : "a" (click));
    return ret;
#endif
    set_default_user_settings();
    return 1;
}

ubyte do_controls_save(ubyte click)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_do_controls_save\n"
        : "=r" (ret) : "a" (click));
    return ret;
#endif
    const char *msg_str;
    if (save_user_settings())
        msg_str = gui_strings[578];
    else
        msg_str = gui_strings[577];
    alert_box_text_fmt("%s", msg_str);
    return 1;
}

ubyte do_controls_calibrate(ubyte click)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_do_controls_calibrate\n"
        : "=r" (ret) : "a" (click));
    return ret;
#endif
    net_unkn_pos_02 = 1;
    alert_box_text_fmt("%s", gui_strings[574]);
    return 1;
}

ubyte show_controls_joystick_box(struct ScreenBox *p_box)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_show_controls_joystick_box\n"
        : "=r" (ret) : "a" (p_box));
    return ret;
#endif
    char locstr[52];
    struct ScreenRect active_rect;
    PlayerInfo *p_locplayer;
    const char *text;
    short wpos_x, wpos_y;
    short ln_height;
    short tx_width;
    ubyte dmuser;
    int i;

    lbFontPtr = small_med_font;
    my_set_text_window(p_box->X + 4, p_box->Y + 4, p_box->Width - 8, p_box->Height - 8);
    ln_height = font_height('A');

    if ((p_box->Flags & GBxFlg_BkgndDrawn) == 0)
    {
        lbFontPtr = med_font;
        lbDisplay.DrawFlags |= Lb_TEXT_HALIGN_CENTER;
        text = gui_strings[489];
        draw_text_purple_list2(0, 110, text, 0);
        lbDisplay.DrawFlags &= ~Lb_TEXT_HALIGN_CENTER;

        lbFontPtr = small_med_font;
        wpos_x = 6;
        wpos_y = 10;
        text = gui_strings[459];
        draw_text_purple_list2(wpos_x, wpos_y, text, 0);
        wpos_y += ln_height + 8;

        for (i = 0; i < 4; i++)
        {
            sprintf(locstr, "%s %d", gui_strings[460], i + 1);
            text = loctext_to_gtext(locstr);
            draw_text_purple_list2(wpos_x, wpos_y, text, 0);
            wpos_y += ln_height + 4;
        }

        copy_box_purple_list(p_box->X + 4, p_box->Y + 4,
          p_box->Width - 8, p_box->Height - 8);
        p_box->Flags |= GBxFlg_BkgndDrawn;

        lbFontPtr = small_med_font;
        ln_height = font_height('A');
    }
    wpos_y = 126;

    lbDisplay.DrawFlags |= 0x8000;
    if (ctl_joystick_type == JTyp_EXT_DRIVER)
    {
      if (joy_func_063(locstr) != -1)
      {
        text = loctext_to_gtext(locstr);
        tx_width = my_string_width(locstr);
        wpos_x = (p_box->Width - tx_width) / 2;
        draw_text_purple_list2(wpos_x, wpos_y, text, 0);
      }
    }
    else
    {
        text = gui_strings[539 + ctl_joystick_type];
        tx_width = my_string_width(text);
        wpos_x = (p_box->Width - tx_width) / 2;
        draw_text_purple_list2(wpos_x, wpos_y, text, 0);
    }
    lbDisplay.DrawFlags &= ~0x8000;

    active_rect.X = text_window_x1 + wpos_x;
    active_rect.Width = tx_width;
    active_rect.Y = text_window_y1 + wpos_y;
    active_rect.Height = ln_height;
    if (mouse_down_over_box(&active_rect))
    {
        if (lbDisplay.LeftButton)
        {
            sbyte v23;
            ubyte v24;

            if (ctl_joystick_type != JTyp_NONE)
                joy_func_066(&joy);
            lbDisplay.LeftButton = 0;

            v23 = -1;
            v24 = ctl_joystick_type;
            while (v23 != 1)
            {
                if (++ctl_joystick_type >= JTyp_TYPES_COUNT)
                    ctl_joystick_type = JTyp_ANALG_2BTN; // first one
                if (v24 == ctl_joystick_type)
                {
                    v23 = 1;
                    ctl_joystick_type = JTyp_NONE;
                }
                if (unkn01_maskarr[ctl_joystick_type])
                    v23 = joy_func_067(&joy, ctl_joystick_type);
                if (!v24)
                    v24 = 1;
            }
        }
    }
    p_locplayer = &players[local_player_no];

    wpos_x = 140;
    wpos_y = 10;

    sprintf(locstr, "%d", p_locplayer->DoubleMode + 1);
    lbDisplay.DrawFlags |= 0x8000;
    text = loctext_to_gtext(locstr);
    draw_text_purple_list2(wpos_x, wpos_y, text, 0);
    tx_width = LbTextStringWidth(locstr);
    lbDisplay.DrawFlags &= ~0x8000;

    active_rect.X = text_window_x1 + wpos_x;
    active_rect.Width = tx_width;
    active_rect.Y = text_window_y1 + wpos_y;
    active_rect.Height = ln_height;
    if (mouse_down_over_box(&active_rect))
    {
        if (lbDisplay.LeftButton)
        {
            lbDisplay.LeftButton = 0;
            if (login_control__State != 5 || nsvc.I.Type == 1)
            {
                p_locplayer->DoubleMode++;
                if (p_locplayer->DoubleMode > 3)
                    p_locplayer->DoubleMode = 0;
            }
            else
            {
                p_locplayer->DoubleMode = 0;
            }

            for (dmuser = p_locplayer->DoubleMode + 1; dmuser < 4; dmuser++)
            {
                p_locplayer->UserInput[dmuser].ControlMode = UInpCtr_Mouse;
            }
        }
    }

    wpos_x = 100;
    wpos_y = ln_height + 18;
    lbDisplay.DrawFlags |= 0x8000;

    active_rect.Y = text_window_y1 + wpos_y;
    active_rect.Height = ln_height;
    for (dmuser = 0; dmuser < p_locplayer->DoubleMode + 1; dmuser++)
    {
        ushort ctlmode;

        ctlmode = p_locplayer->UserInput[dmuser].ControlMode;
        if (ctlmode >= UInpCtr_Joystick0)
        {
            int n_found;

            i = 0;
            n_found = 0;
            while (n_found < ctlmode - 1)
            {
                if (joy.Init[i])
                    n_found++;
                i++;
            }
            sprintf(locstr, "%s %d", gui_strings[463], i);
            text = loctext_to_gtext(locstr);
            draw_text_purple_list2(wpos_x, wpos_y, text, 0);
        }
        else
        {
            sprintf(locstr, "%s", gui_strings[461 + ctlmode]);
            text = loctext_to_gtext(locstr);
            draw_text_purple_list2(wpos_x, wpos_y, text, 0);
        }
        tx_width = LbTextStringWidth(locstr);

        active_rect.X = text_window_x1 + wpos_x;
        active_rect.Width = tx_width;
        if (mouse_down_over_box(&active_rect))
        {
            if (lbDisplay.LeftButton)
            {
                lbDisplay.LeftButton = 0;

                ctlmode = p_locplayer->UserInput[dmuser].ControlMode + 1;
                if (ctl_joystick_type == JTyp_NONE && ctlmode == UInpCtr_Joystick0)
                    ctlmode = UInpCtr_Keyboard;
                if (ctlmode >= UInpCtr_Joystick0 + joy.NumberOfDevices)
                    ctlmode = UInpCtr_Keyboard;
                p_locplayer->UserInput[dmuser].ControlMode = ctlmode;
            }
        }
        wpos_y += ln_height + 4;
    }
    lbDisplay.DrawFlags &= ~0x8000;

    //controls_calibrate_button.DrawFn(&controls_calibrate_button); -- incompatible calling convention
    {
        ubyte drawn;
        asm volatile ("call *%2\n"
            : "=r" (drawn) : "a" (&controls_calibrate_button), "g" (controls_calibrate_button.DrawFn));
    }
    return 0;
}

void set_controls_key(ushort hlight_gkey, ushort key)
{
    GameKey gkey;
    TbBool is_joystick;

    if (hlight_gkey > (GKey_KEYS_COUNT - 1)) {
        gkey = hlight_gkey - (GKey_KEYS_COUNT - 1);
        is_joystick = true;
    } else {
        gkey = hlight_gkey;
        is_joystick = false;
    }

    if (is_joystick)
        set_gamekey_joy(gkey, key);
    else
        set_gamekey_kbd(gkey, key);
}

void unset_controls_key(ushort hlight_gkey)
{
    GameKey gkey;
    TbBool is_joystick;

    if (hlight_gkey > (GKey_KEYS_COUNT - 1)) {
        gkey = hlight_gkey - (GKey_KEYS_COUNT - 1);
        is_joystick = true;
    } else {
        gkey = hlight_gkey;
        is_joystick = false;
    }

    if (is_joystick)
        set_gamekey_joy(gkey, 0);
    else
        set_gamekey_kbd(gkey, KC_UNASSIGNED);
}

TbBool is_hardcoded_hlight_gkey(ushort hlight_gkey)
{
    if (hlight_gkey >= (GKey_KEYS_COUNT - 1) + GKey_UP &&
      hlight_gkey <= (GKey_KEYS_COUNT - 1) + GKey_RIGHT)
        return true;

    return false;
}

/** Check inputs for controls box in system menu screen.
 *
 * @return Gives 0 on no action, 1 on non-control-changing action, 2 on control key update/
 */
ubyte menu_controls_inputs(struct ScreenTextBox *p_box)
{
    ubyte ret;
    int i;

    ret = 0;

    if (!net_unkn_pos_01b && !net_unkn_pos_02)
    {
        if (is_key_pressed(KC_DOWN, KMod_DONTCARE))
        {
            clear_key_pressed(KC_DOWN);
            controls_hlight_gkey++;
            if (controls_hlight_gkey > 2 * (GKey_KEYS_COUNT - 1))
                controls_hlight_gkey = 1;
            ret = 1;
        }
        if (is_key_pressed(KC_UP, KMod_DONTCARE))
        {
            clear_key_pressed(KC_UP);
            controls_hlight_gkey--;
            if (controls_hlight_gkey < 1)
                controls_hlight_gkey = 2 * (GKey_KEYS_COUNT - 1);
            ret = 1;
        }
        if (is_key_pressed(KC_RIGHT, KMod_DONTCARE))
        {
            // Next column
            clear_key_pressed(KC_RIGHT);
            controls_hlight_gkey += (GKey_KEYS_COUNT - 1);
            if (controls_hlight_gkey > 2 * (GKey_KEYS_COUNT - 1))
                controls_hlight_gkey -= 2 * (GKey_KEYS_COUNT - 1);
            ret = 1;
        }
        if (is_key_pressed(KC_LEFT, KMod_DONTCARE))
        {
            // Prev column
            clear_key_pressed(KC_LEFT);
            controls_hlight_gkey -= (GKey_KEYS_COUNT - 1);
            if (controls_hlight_gkey < 1)
                controls_hlight_gkey += 2 * (GKey_KEYS_COUNT - 1);
            ret = 1;
        }

        if (is_hardcoded_hlight_gkey(controls_hlight_gkey))
        {
            if (byte_1C4970)
            {
                if (byte_1C4970 == 1 && !is_key_pressed(KC_RETURN, KMod_DONTCARE))
                {
                    lbExtendedKeyPress = 0;
                    lbInkey = 0;
                    byte_1C4970 = 0;
                    net_unkn_pos_01b = controls_hlight_gkey;
                    ret = 2;
                }
            }
            else
            {
                if (is_key_pressed(KC_RETURN, KMod_DONTCARE))
                {
                    byte_1C4970++;
                    ret = 2;
                }
                if (is_key_pressed(KC_BACK, KMod_DONTCARE))
                {
                    clear_key_pressed(KC_BACK);
                    unset_controls_key(controls_hlight_gkey);
                    ret = 2;
                }
            }
        }
    }

    if (net_unkn_pos_01b != 0)
    {
        GameKey hlight_gkey;

        hlight_gkey = net_unkn_pos_01b;
        if (hlight_gkey > (GKey_KEYS_COUNT - 1))
        {
            if (joy.Buttons[0])
            {
                uint jbtn_pressed, jbtn_max, jbtn, jskey_flags;

                jskey_flags = 0;
                jbtn = 0;
                jbtn_max = joy.NumberOfButtons[0];
                if (jbtn_max > 0)
                {
                  jbtn_pressed = joy.Buttons[0];
                  for (i = 0; i < 4; i++)
                  {
                    if ((jbtn_pressed & (1 << jbtn)) != 0) {
                        jskey_flags |= (1 << jbtn);
                    }
                    jbtn++;
                    if (jbtn >= jbtn_max)
                        break;
                  }
                }
                set_controls_key(hlight_gkey, jskey_flags);
                ret = 2;
            }
        }
        else
        {
            if (lbInkey != 0)
            {
                if (lbExtendedKeyPress)
                {
                  set_controls_key(hlight_gkey, lbInkey | 0x80);
                  lbExtendedKeyPress = 0;
                  lbInkey = 0;
                }
                else
                {
                  if ((lbInkey & 0x7F) != KC_BACKSLASH)
                      set_controls_key(hlight_gkey, lbInkey & 0x7F);
                  lbInkey = 0;
                }
                ret = 2;
            }
        }
    }
    return ret;
}

const char *gamekey_text_action_name_for_draw(GameKey gkey)
{
    char locstr[52];
    const char *text;

    if ((gkey >= GKey_SEL_AGENT_1) && (gkey <= GKey_SEL_AGENT_4))
    {
        //TODO should we place Agent/Zealot/Outcast text ID in campaign config file, or keep at const position within campaign area?
        text = gui_strings[GSTR_CPG_CYBORG_NOUN_FULL + 100 * background_type];
        sprintf(locstr, "%s %d", text, (gkey - GKey_SEL_AGENT_1 + 1));
        text = loctext_to_gtext(locstr);
    }
    else
    {
        text = gui_strings[GSTR_SYS_GAME_KEYS + gkey - 1];
    }
    return text;
}

const char *gamekey_text_kbkey_name_for_draw(GameKey gkey)
{
    char locstr[52];
    const char *text;

    sprint_gamekey_combination_kbd(locstr, gkey);
    if (strlen(locstr) == 0) {
      strcpy(locstr, "...");
    }
    text = loctext_to_gtext(locstr);

    return text;
}

const char *gamekey_text_jskey_name_for_draw(GameKey gkey)
{
    char locstr[52];
    const char *text;

    sprint_gamekey_combination_joy(locstr, gkey);
    if (strlen(locstr) == 0) {
      strcpy(locstr, "...");
    }
    text = loctext_to_gtext(locstr);

    return text;
}

#define GAMEKEY_ACTIVE_WIDTH_MIN 50

ubyte show_menu_controls_list_box(struct ScreenTextBox *p_box)
{
    short ln_height;
    short wpos_x, wpos_y;
    short tx_kb_width[GKey_KEYS_COUNT];
    short tx_js_width[GKey_KEYS_COUNT];
    int i, i_limit;
    ubyte kchange;

    // This needs to be done before setting my_text_window
    i_limit = p_box->field_38 + get_text_box_lines_visible(p_box);
    if (i_limit > GKey_KEYS_COUNT - 1)
        i_limit = GKey_KEYS_COUNT - 1;

    my_set_text_window(p_box->X + 4, p_box->Y + 4, p_box->Width - 8, p_box->Height - 8);
    if ((p_box->Flags & GBxFlg_BkgndDrawn) == 0) // never set anyway
    {
        const char *text;

        lbFontPtr = med_font;
        text = gui_strings[486];
        draw_text_purple_list2(4, 4, text, 0);
        text = gui_strings[487];
        draw_text_purple_list2(200, 4, text, 0);
        text = gui_strings[488];
        draw_text_purple_list2(300, 4, text, 0);

        lbDisplay.DrawFlags = 0;
    }

    lbFontPtr = p_box->Font;
    ln_height = font_height('A');

    // Names column
    wpos_x = 4;
    wpos_y = 28;
    for (i = p_box->field_38; i < i_limit; i++)
    {
        const char *text;
        GameKey gkey;

        gkey = i + 1;
        text = gamekey_text_action_name_for_draw(gkey);
        draw_text_purple_list2(wpos_x, wpos_y, text, 0);
        wpos_y += p_box->LineHeight;
    }

    // Keyboard keys column
    wpos_x = 200;
    wpos_y = 28;
    for (i = p_box->field_38; i < i_limit; i++)
    {
        const char *text;
        GameKey gkey;

        gkey = i + 1;
        if (controls_hlight_gkey == gkey)
        {
            lbDisplay.DrawFlags = Lb_TEXT_ONE_COLOR;
            lbDisplay.DrawColour = 0x57;
        }
        else
        {
            lbDisplay.DrawFlags = 0;
        }
        text = gamekey_text_kbkey_name_for_draw(gkey);
        tx_kb_width[gkey] = my_string_width(text);

        lbDisplay.DrawFlags |= 0x8000;
        draw_text_purple_list2(wpos_x, wpos_y, text, 0);
        lbDisplay.DrawFlags &= ~0x8000;

        wpos_y += p_box->LineHeight;
    }

    // Joystick keys column
    wpos_x = 300;
    wpos_y = 28;
    for (i = p_box->field_38; i < i_limit; i++)
    {
        const char *text;
        GameKey gkey;

        gkey = i + 1;
        if (controls_hlight_gkey == gkey + (GKey_KEYS_COUNT - 1))
        {
          lbDisplay.DrawFlags = Lb_TEXT_ONE_COLOR;
          lbDisplay.DrawColour = 87;
        }
        else
        {
          lbDisplay.DrawFlags = 0;
        }
        text = gamekey_text_jskey_name_for_draw(gkey);
        tx_js_width[gkey] = my_string_width(text);

        lbDisplay.DrawFlags |= 0x8000;
        draw_text_purple_list2(wpos_x, wpos_y, text, 0);
        lbDisplay.DrawFlags &= ~0x8000;

        wpos_y += p_box->LineHeight;
    }
    lbDisplay.DrawFlags = 0;

    wpos_x = 200;
    wpos_y = 28;
    for (i = p_box->field_38; i < i_limit; i++)
    {
        short col_width;
        GameKey gkey;

        gkey = i + 1;
        col_width = max(tx_kb_width[gkey], GAMEKEY_ACTIVE_WIDTH_MIN);
        if (lbDisplay.LeftButton || joy.Buttons[0])
        {
            if (mouse_down_over_box_coords(text_window_x1 + wpos_x, text_window_y1 + wpos_y,
              text_window_x1 + col_width + wpos_x, text_window_y1 + wpos_y + ln_height))
            {
                lbDisplay.LeftButton = 0;
                controls_hlight_gkey = gkey;
                net_unkn_pos_01b = gkey;
                lbExtendedKeyPress = 0;
                lbInkey = 0;
            }
        }

        wpos_y += p_box->LineHeight;
    }

    wpos_x = 300;
    wpos_y = 28;
    for (i = p_box->field_38; i < i_limit; i++)
    {
        short col_width;
        GameKey gkey;

        gkey = i + 1;
        col_width = max(tx_js_width[gkey], GAMEKEY_ACTIVE_WIDTH_MIN);
        if ( lbDisplay.LeftButton || joy.Buttons[0] )
        {
            if (mouse_down_over_box_coords(text_window_x1 + wpos_x, text_window_y1 + wpos_y,
              text_window_x1 + wpos_x + col_width, text_window_y1 + wpos_y + ln_height))
            {
                lbDisplay.LeftButton = 0;
                controls_hlight_gkey = gkey + (GKey_KEYS_COUNT - 1);
                net_unkn_pos_01b = gkey + (GKey_KEYS_COUNT - 1);
                lbExtendedKeyPress = 0;
                lbInkey = 0;
            }
        }
        wpos_y += p_box->LineHeight;
    }

    kchange = menu_controls_inputs(p_box);

    // If moved key, make sure the new highlight is visible
    if ((kchange != 0) && (controls_hlight_gkey != 0))
    {
        int i;

        if (controls_hlight_gkey < GKey_KEYS_COUNT)
            i = controls_hlight_gkey - 1;
        else
            i = controls_hlight_gkey - 1 - (GKey_KEYS_COUNT - 1);
        if (p_box->field_38 > i)
            p_box->field_38 = i;
        else if (p_box->field_38 + i_limit < i + 1)
            p_box->field_38 = i + 1 - i_limit;

    }

    //controls_defaults_button.DrawFn(&controls_defaults_button); -- incompatible calling convention
    asm volatile ("call *%1\n"
        : : "a" (&controls_defaults_button), "g" (controls_defaults_button.DrawFn));
    //controls_save_button.DrawFn(&controls_save_button); -- incompatible calling convention
    asm volatile ("call *%1\n"
        : : "a" (&controls_save_button), "g" (controls_save_button.DrawFn));

    return 0;
}

ubyte update_settings_controls_alert(struct ScreenTextBox *p_box)
{
    if (!net_unkn_pos_02 || (net_unkn_pos_02 - 1) > 5)
        return 0;

    switch (net_unkn_pos_02)
    {
    case 1:
        if (screentype == SCRT_ALERTBOX)
        {
            if (!joy.Buttons[0])
                break;
            net_unkn_pos_02++;
            break;
        }
        net_unkn_pos_02 = 0;
        break;
    case 2:
        if (joy.Buttons[0])
            break;
        alert_box_text_fmt("%s", gui_strings[575]);
        net_unkn_pos_02++;
        break;
    case 3:
        if (screentype == SCRT_ALERTBOX)
        {
            if (!joy.Buttons[0])
                break;
            net_unkn_pos_02++;
            break;
        }
        net_unkn_pos_02 = 0;
        break;
    case 4:
        if (joy.Buttons[0])
            break;
        alert_box_text_fmt("%s", gui_strings[576]);
        net_unkn_pos_02++;
        break;
    case 5:
        if (screentype == SCRT_ALERTBOX)
        {
            if (!joy.Buttons[0])
                break;
            net_unkn_pos_02++;
            break;
        }
        net_unkn_pos_02 = 0;
        break;
    case 6:
        show_alert = 1;
        if (joy.Buttons[0])
            break;
        net_unkn_pos_02 = 0;
        show_alert = 0;
        break;
    }
    return 0;
}

ubyte show_options_controls_screen(void)
{
    ubyte drawn;

    //drawn = controls_list_box.DrawFn(&controls_list_box); -- incompatible calling convention
    asm volatile ("call *%2\n"
        : "=r" (drawn) : "a" (&controls_list_box), "g" (controls_list_box.DrawFn));
    if (drawn == 3) {
        update_settings_controls_alert(&controls_list_box);
        //drawn = controls_joystick_box.DrawFn(&controls_joystick_box); -- incompatible calling convention
        asm volatile ("call *%2\n"
            : "=r" (drawn) : "a" (&controls_joystick_box), "g" (controls_joystick_box.DrawFn));
    }
    return drawn;
}

void init_controls_screen_boxes(void)
{
    short scr_w, start_x;

    scr_w = lbDisplay.GraphicsWindowWidth;

    init_screen_text_box(&controls_list_box, 213u, 72u, 420u, 354, 6, small_med_font, 1);
    controls_list_box.DrawTextFn = ac_show_menu_controls_list_box;
    controls_list_box.ScrollWindowHeight = 296;
    controls_list_box.Buttons[0] = &controls_defaults_button;
    controls_list_box.Buttons[1] = &controls_save_button;
    controls_list_box.Lines = GKey_KEYS_COUNT;
    controls_list_box.Flags |= (GBxFlg_RadioBtn | GBxFlg_IsMouseOver);

    controls_list_box.ScrollWindowOffset += 27;
    init_screen_button(&controls_defaults_button, 219u, 405u,
      gui_strings[484], 6, med2_font, 1, 0);
    controls_defaults_button.CallBackFn = ac_do_controls_defaults;
    init_screen_button(&controls_save_button, 627u, 405u,
      gui_strings[439], 6, med2_font, 1, 0x80);
    controls_save_button.CallBackFn = ac_do_controls_save;

    init_screen_box(&controls_joystick_box, 7u, 252u, 197u, 174, 6);
    controls_joystick_box.SpecialDrawFn = show_controls_joystick_box;
    init_screen_button(&controls_calibrate_button, 57u, 405u,
      gui_strings[485], 6, med2_font, 1, 0);
    controls_calibrate_button.CallBackFn = ac_do_controls_calibrate;

    start_x = (scr_w - controls_joystick_box.Width - controls_list_box.Width - 23) / 2;

    controls_joystick_box.X = start_x + 7;
    controls_calibrate_button.X = controls_joystick_box.X + 50;

    controls_list_box.X = controls_joystick_box.X + controls_joystick_box.Width + 9;
    controls_defaults_button.X = controls_list_box.X + 6;
    // Additional 12 px left to fit scroll bar buttons
    controls_save_button.X = controls_list_box.X + controls_list_box.Width - controls_save_button.Width - 6 - 12;
}

void reset_controls_screen_boxes_flags(void)
{
    controls_list_box.Flags = GBxFlg_Unkn0001;
    controls_list_box.Flags |= (GBxFlg_RadioBtn | GBxFlg_IsMouseOver);
    controls_joystick_box.Flags = GBxFlg_Unkn0001;
}

void set_flag01_controls_screen_boxes(void)
{
    controls_list_box.Flags |= GBxFlg_Unkn0001;
    controls_save_button.Flags |= GBxFlg_Unkn0001;
    controls_defaults_button.Flags |= GBxFlg_Unkn0001;
    controls_calibrate_button.Flags |= GBxFlg_Unkn0001;
}

void set_flag02_controls_screen_boxes(void)
{
    controls_defaults_button.Flags |= GBxFlg_Unkn0002;
    controls_calibrate_button.Flags |= GBxFlg_Unkn0002;
    controls_save_button.Flags |= GBxFlg_Unkn0002;
    controls_joystick_box.Flags |= GBxFlg_Unkn0002;
    controls_list_box.Flags |= GBxFlg_Unkn0002;
}

/******************************************************************************/
