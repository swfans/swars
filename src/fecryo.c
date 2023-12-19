/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file fecryo.c
 *     Front-end desktop and menu system, cryogenic chamber screen.
 * @par Purpose:
 *     Implement functions for cryogenic chamber screen in front-end desktop.
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
#include "fecryo.h"

#include <assert.h>
#include "bfmemut.h"
#include "ssampply.h"
#include "specblit.h"
#include "campaign.h"
#include "cybmod.h"
#include "femain.h"
#include "feequip.h"
#include "guiboxes.h"
#include "guitext.h"
#include "display.h"
#include "game.h"
#include "sound.h"
#include "swlog.h"
/******************************************************************************/
extern struct ScreenBox cryo_blokey_box;
extern struct ScreenTextBox cryo_agent_list_box;
extern struct ScreenTextBox cryo_cybmod_list_box;
extern struct ScreenButton cryo_offer_cancel_button;

extern char cybmod_name_text[];

extern ubyte current_frame;

// Shared with equip screen
extern char equip_cost_text[20];
extern struct ScreenTextBox equip_name_box;
extern struct ScreenTextBox equip_list_box;
extern struct ScreenInfoBox equip_cost_box;
extern struct ScreenButton equip_offer_buy_button;

ubyte ac_do_cryo_offer_cancel(ubyte click);
ubyte ac_show_cryo_agent_list(struct ScreenTextBox *box);
ubyte ac_show_cryo_cybmod_list_box(struct ScreenTextBox *box);
ubyte ac_show_cryo_blokey(struct ScreenBox *box);

ubyte do_cryo_offer_cancel(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_cryo_offer_cancel\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

#define PURPLE_MOD_AREA_WIDTH 139
#define PURPLE_MOD_AREA_HEIGHT 295

void init_next_blokey_flic(void)
{
#if 0
    asm volatile ("call ASM_init_next_blokey_flic\n"
        :  :  : "eax" );
#else
    struct Campaign *p_campgn;
    const char *campgn_mark;
    const char *flic_dir;
    ushort cmod, stage;
    int k;

    p_campgn = &campaigns[background_type];
    campgn_mark = p_campgn->ProjectorFnMk;
    // TODO FNAMES the convention with mark char is broken for "s"
    if (strcmp(campgn_mark, "s") == 0)
        campgn_mark = "m";

    flic_dir = "qdata/equip";

    stage = 0;

    if (stage == 0)
    {
        for (cmod = 1; cmod < 4; cmod = (cmod+1) % 4)
        {
            if (((mod_draw_states[cmod] & 0x08) != 0) &&
              ((mod_draw_states[cmod] & 0x04) != 0)) {
                stage = 1;
                break;
            }
            if (cmod == 0)
                break;
        }
    }

    if (stage == 0)
    {
        for (cmod = 0; cmod < 4; cmod++)
        {
            if (((mod_draw_states[cmod] & 0x08) != 0) &&
              ((mod_draw_states[cmod] & 0x04) == 0) &&
              (flic_mods[cmod] != 0)) {
                stage = 2;
                break;
            }
        }
    }

    if (stage == 0)
    {
        if (current_drawing_mod == 4) {
            stage = 3;
        } else
        for (cmod = 0; cmod < 4; cmod++)
        {
            if (flic_mods[cmod] == 0) {
                stage = 3;
                break;
            }
        }
    }

    switch (stage)
    {
    case 0:
        if (!byte_1DDC40)
        {
            byte_1DDC40 = 1;
            play_sample_using_heap(0, 134, 127, 64, 100, 0, 3u);
        }
        else if (!IsSamplePlaying(0, 134, 0))
        {
            k = anim_slots[8];
            sprintf(animations[k].Filename, "%s/%s%da%d.fli", flic_dir, campgn_mark, flic_mods[0], flic_mods[2]);
            flic_unkn03(8u);
            play_sample_using_heap(0, 126, 127, 64, 100, 0, 1u);
            current_frame = 0;
            new_current_drawing_mod = 4;
            byte_1DDC40 = 0;
        }
        break;
    case 1:
        k = anim_slots[3];
        switch (cmod)
        {
        case 0:
            sprintf(animations[k].Filename, "%s/%s%dbo.fli", flic_dir, campgn_mark, old_flic_mods[0]);
            break;
        case 1:
            sprintf(animations[k].Filename, "%s/%s%dbbo.fli", flic_dir, campgn_mark, old_flic_mods[0]);
            break;
        case 2:
            sprintf(animations[k].Filename, "%s/%s%da%do.fli", flic_dir, campgn_mark, old_flic_mods[0], old_flic_mods[2]);
            break;
        case 3:
            sprintf(animations[k].Filename, "%s/%s%dl%do.fli", flic_dir, campgn_mark, old_flic_mods[0], old_flic_mods[3]);
            break;
        default:
            assert(!"unreachable");
            break;
        }
        flic_unkn03(3);
        new_current_drawing_mod = cmod;
        mod_draw_states[cmod] |= 0x02;
        cryo_blokey_box.Flags &= ~0x0100;
        play_sample_using_heap(0, 132, 127, 64, 100, 0, 3);
        byte_1DDC40 = 0;
        break;
    case 2:
        k = anim_slots[3];
        switch (cmod)
        {
          case 0:
            sprintf(animations[k].Filename, "%s/%s%dbi.fli", flic_dir, campgn_mark, flic_mods[0]);
            break;
          case 1:
            sprintf(animations[k].Filename, "%s/%s%dbbi.fli", flic_dir, campgn_mark, flic_mods[0]);
            break;
          case 2:
            sprintf(animations[k].Filename, "%s/%s%da%di.fli", flic_dir, campgn_mark, flic_mods[0], flic_mods[2]);
            break;
          case 3:
            sprintf(animations[k].Filename, "%s/%s%dl%di.fli", flic_dir, campgn_mark, flic_mods[0], flic_mods[3]);
            break;
          default:
            assert(!"unreachable");
            break;
        }
        flic_unkn03(3);
        new_current_drawing_mod = cmod;
        mod_draw_states[cmod] |= 0x01;
        mod_draw_states[cmod] &= ~0x08;
        old_flic_mods[cmod] = flic_mods[cmod];
        cryo_blokey_box.Flags &= ~0x0100;
        byte_1DDC40 = 0;
        break;
    case 3:
        // No further action
        break;
    }
#endif
}

void purple_mods_data_to_screen(void)
{
#if 0
    asm volatile ("call ASM_purple_mods_data_to_screen\n"
        :  :  : "eax" );
#else
    struct Campaign *p_campgn;
    const char *campgn_mark;
    const char *flic_dir;
    char str[52];
    ubyte *buf;
    ubyte *o[2];

    p_campgn = &campaigns[background_type];
    campgn_mark = p_campgn->ProjectorFnMk;
    // TODO FNAMES the convention with mark char is broken for "s"
    if (strcmp(campgn_mark, "s") == 0)
        campgn_mark = "";

    flic_dir = "qdata/equip";

    sprintf(str, "%s/bgman%s.dat", flic_dir, campgn_mark);

    buf = back_buffer - PURPLE_MOD_AREA_WIDTH*PURPLE_MOD_AREA_HEIGHT;
    LbFileLoadAt(str, buf);

    o[1] = back_buffer;
    o[0] = lbDisplay.WScreen;

    copy_buffer_to_double_bufs(buf, PURPLE_MOD_AREA_WIDTH, PURPLE_MOD_AREA_HEIGHT,
        o, 275, 123,
        lbDisplay.GraphicsScreenWidth, lbDisplay.GraphicsScreenHeight);
#endif
}

void blokey_flic_data_to_screen(void)
{
#if 1
    asm volatile ("call ASM_blokey_flic_data_to_screen\n"
        :  :  : "eax" );
#else
#endif
}

void blokey_static_flic_data_to_screen(void)
{
#if 0
    asm volatile ("call ASM_blokey_static_flic_data_to_screen\n"
        :  :  : "eax" );
#else
    struct Campaign *p_campgn;
    const char *campgn_mark;
    const char *flic_dir;
    char str[52];
    ubyte *buf;
    ubyte *o[2];
    int k;

    p_campgn = &campaigns[background_type];
    campgn_mark = p_campgn->ProjectorFnMk;
    // TODO FNAMES the convention with mark char is broken for "s"
    if (strcmp(campgn_mark, "s") == 0)
        campgn_mark = "m";

    flic_dir = "qdata/equip";

    o[1] = back_buffer;
    o[0] = lbDisplay.WScreen;

    for (k = 0; k < 4; k++)
    {
        long len;

        if (flic_mods[k] == 0)
            continue;

        switch (k)
        {
        case 0:
            sprintf(str, "%s/%s%db.dat", flic_dir, campgn_mark, flic_mods[0]);
            break;
        case 1:
            sprintf(str, "%s/%s%dbb.dat", flic_dir, campgn_mark, flic_mods[0]);
            break;
        case 2:
            sprintf(str, "%s/%s%da%d.dat", flic_dir, campgn_mark, flic_mods[0], flic_mods[2]);
            break;
        case 3:
            sprintf(str, "%s/%s%dl%d.dat", flic_dir, campgn_mark, flic_mods[0], flic_mods[3]);
            break;
        }

        buf = unkn_buffer_05 + 0x8000;
        len = LbFileLoadAt(str, buf);
        if (len < 4) {
            LbMemorySet(buf, 0, flic_mod_widths[k] * flic_mod_heights[k]);
        }

        copy_buffer_to_double_bufs_with_trans(buf, flic_mod_widths[k], flic_mod_heights[k],
          o, flic_mod_coords_b[2*k], flic_mod_coords_b[2*k+1],
          lbDisplay.GraphicsScreenWidth, lbDisplay.GraphicsScreenHeight, 0);

        mod_draw_states[k] = 4;
    }
#endif
}

void update_cybmod_cost_text(void)
{
    int cost;

    cost = 10 * (int)mod_defs[selected_mod + 1].Cost;
    sprintf(equip_cost_text, "%d", cost);
}

ubyte show_cryo_blokey(struct ScreenBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_cryo_blokey\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte show_cryo_agent_list(struct ScreenTextBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_cryo_agent_list\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte show_cryo_cybmod_list_box(struct ScreenTextBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_cryo_cybmod_list_box\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

void show_cryo_chamber_screen(void)
{
    asm volatile ("call ASM_show_cryo_chamber_screen\n"
        :  :  : "eax" );
}

void init_cryo_screen_boxes(void)
{
    short scr_w, start_x;

    scr_w = lbDisplay.GraphicsWindowWidth;

    init_screen_text_box(&cryo_agent_list_box, 7u, 122u, 196u, 303, 6,
        small_med_font, 1);
    cryo_agent_list_box.BGColour = 25;
    cryo_agent_list_box.DrawTextFn = ac_show_cryo_agent_list;
    cryo_agent_list_box.ScrollWindowOffset += 27;
    cryo_agent_list_box.Flags |= 0x0300;
    cryo_agent_list_box.ScrollWindowHeight -= 27;

    init_screen_box(&cryo_blokey_box, 212u, 122u, 203u, 303, 6);
    cryo_blokey_box.SpecialDrawFn = ac_show_cryo_blokey;

    init_screen_text_box(&cryo_cybmod_list_box, 425u, 153u, 208u, 272,
      6, small_med_font, 1);
    cryo_cybmod_list_box.DrawTextFn = ac_show_cryo_cybmod_list_box;
    cryo_cybmod_list_box.Flags |= 0x0300;
    cryo_cybmod_list_box.ScrollWindowHeight = 117;

    init_screen_button(&cryo_offer_cancel_button, 628u, 404u,
      gui_strings[437], 6, med2_font, 1, 0x80);
    cryo_offer_cancel_button.CallBackFn = ac_do_cryo_offer_cancel;

    start_x = (scr_w - cryo_agent_list_box.Width - cryo_blokey_box.Width - cryo_cybmod_list_box.Width - 33) / 2;

    cryo_agent_list_box.X = start_x + 7;
    cryo_blokey_box.X = cryo_agent_list_box.X + cryo_agent_list_box.Width + 9;
    cryo_cybmod_list_box.X = cryo_blokey_box.X + cryo_blokey_box.Width + 10;
    cryo_offer_cancel_button.X = cryo_cybmod_list_box.X + cryo_cybmod_list_box.Width - cryo_offer_cancel_button.Width - 5;
}

void switch_shared_equip_screen_buttons_to_cybmod(void)
{
    set_heading_box_text(gui_strings[369]);
    equip_cost_box.X = cryo_cybmod_list_box.X + 5;
    equip_cost_box.Width = cryo_cybmod_list_box.Width - 10;
    equip_cost_box.Y = 383;
    equip_name_box.Text = cybmod_name_text;
    if (selected_mod < 0)
        cybmod_name_text[0] = '\0';
    else
        init_weapon_anim(selected_mod + 32);
    switch_equip_offer_to_buy();
}

void set_flag01_cryo_screen_boxes(void)
{
    cryo_offer_cancel_button.Flags |= 0x0001;
}

void reset_cryo_screen_boxes_flags(void)
{
    cryo_cybmod_list_box.Flags = 0x0001 | 0x0100 | 0x0200;
    cryo_agent_list_box.Flags = 0x0001 | 0x0100 | 0x0200;
    cryo_blokey_box.Flags = 0x0001;
}

/******************************************************************************/
