/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file feequip.c
 *     Front-end desktop and menu system, equipment screen.
 * @par Purpose:
 *     Implement functions for equipment screen in front-end desktop.
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
#include "feequip.h"

#include "bftext.h"
#include "bfsprite.h"
#include "bfkeybd.h"
#include "bflib_joyst.h"
#include "femain.h"
#include "guiboxes.h"
#include "guitext.h"
#include "display.h"
#include "campaign.h"
#include "game.h"
#include "cybmod.h"
#include "player.h"
#include "research.h"
#include "weapon.h"
#include "sound.h"
#include "swlog.h"
/******************************************************************************/
extern struct ScreenTextBox equip_name_box;
extern struct ScreenBox weapon_slots;
extern struct ScreenTextBox equip_list_head_box;
extern struct ScreenTextBox equip_list_box;
extern struct ScreenTextBox equip_display_box;
extern struct ScreenButton equip_offer_buy_button;
extern struct ScreenInfoBox equip_cost_box;
extern struct ScreenButton equip_all_agents_button;

extern ubyte cheat_research_weapon;
extern ubyte byte_1C4975;
extern ubyte byte_1C4976;
extern struct TbSprite *sprites_Icons0_0;

extern char unkn41_text[];
extern char equip_cost_text[20];

ubyte ac_display_weapon_info(struct ScreenTextBox *box);
ubyte ac_show_weapon_name(struct ScreenTextBox *box);
ubyte ac_show_weapon_list(struct ScreenTextBox *box);
ubyte ac_show_weapon_slots(struct ScreenBox *box);
ubyte ac_do_equip_offer_buy(ubyte click);
ubyte ac_sell_equipment(ubyte click);
ubyte ac_select_all_agents(ubyte click);

ubyte do_equip_offer_buy(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_do_equip_offer_buy\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

ubyte sell_equipment(ubyte click)
{
    ubyte ret;
    asm volatile ("call ASM_sell_equipment\n"
        : "=r" (ret) : "a" (click));
    return ret;
}

void check_buy_sell_button(void)
{
    asm volatile ("call ASM_check_buy_sell_button\n"
        :  :  : "eax" );
}

ubyte select_all_agents(ubyte click)
{
    selected_agent = 4;
    check_buy_sell_button();
    return 1;
}

void update_equip_cost_text(void)
{
    int cost;

    cost = 100 * weapon_defs[selected_weapon + 1].Cost;
    if (equip_offer_buy_button.CallBackFn == ac_do_equip_offer_buy)
        sprintf(equip_cost_text, "%d", cost);
    else
        sprintf(equip_cost_text, "%d", cost >> 1);
}

void switch_equip_offer_to_buy(void)
{
    equip_offer_buy_button.Text = gui_strings[436];
    equip_offer_buy_button.CallBackFn = ac_do_equip_offer_buy;
}

void show_equipment_screen(void)
{
    asm volatile ("call ASM_show_equipment_screen\n"
        :  :  : "eax" );
}

void init_weapon_anim(ubyte weapon)
{
#if 0
    asm volatile ("call ASM_init_weapon_anim\n"
        : : "a" (weapon));
#else
    struct Campaign *p_campgn;
    const char *campgn_mark;
    const char *flic_dir;
    ulong k;

    p_campgn = &campaigns[background_type];
    campgn_mark = p_campgn->ProjectorFnMk;
    // TODO FNAMES the convention with mark char is broken for "s"
    if (strcmp(campgn_mark, "s") == 0)
        campgn_mark = "";

    flic_dir = "data/equip";

    if (weapon >= 32)
    {
        k = anim_slots[2];
        sprintf(animations[k].Filename, "%s/mod-%02d%s.fli", flic_dir, (int)weapon - 32, campgn_mark);
    }
    else
    {
        k = anim_slots[2];
        sprintf(animations[k].Filename, "%s/wep-%02d%s.fli", flic_dir, (int)weapon, campgn_mark);
    }
    flic_unkn03(2);
#endif
}

void switch_shared_equip_screen_buttons_to_equip(void)
{
    set_heading_box_text(gui_strings[370]);
    refresh_equip_list = 1;
    equip_cost_box.X = equip_offer_buy_button.X + equip_offer_buy_button.Width + 4;
    equip_cost_box.Width = equip_list_box.Width - 2 - equip_offer_buy_button.Width - 14;
    equip_cost_box.Y = 404;
    if (selected_weapon + 1 < 1)
    {
        equip_name_box.Text = 0;
    }
    else
    {
        init_weapon_anim(selected_weapon + 1 - 1);
        if (is_research_weapon_completed(selected_weapon + 1) || (login_control__State != 6))
        {
            struct Campaign *p_campgn;
            p_campgn = &campaigns[background_type];
            equip_name_box.Text = gui_strings[p_campgn->WeaponsTextIdShift + selected_weapon + 1 - 1];
        }
        else
        {
            equip_name_box.Text = gui_strings[65];
        }
    }
}

ubyte display_weapon_info(struct ScreenTextBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_display_weapon_info\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte show_weapon_name(struct ScreenTextBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_weapon_name\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte show_weapon_list(struct ScreenTextBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_weapon_list\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

ubyte show_weapon_slots(struct ScreenBox *box)
{
    ubyte ret;
    asm volatile ("call ASM_show_weapon_slots\n"
        : "=r" (ret) : "a" (box));
    return ret;
}

void init_equip_screen_boxes(void)
{
    const char *s;
    short scr_w, start_x;

    scr_w = lbDisplay.GraphicsWindowWidth;

    init_screen_text_box(&equip_list_head_box, 7u, 122u, 191u, 22,
      6, small_med_font, 1);
    init_screen_text_box(&equip_name_box, 425u, 122u, 208u, 22,
      6, small_med_font, 1);
    init_screen_box(&weapon_slots, 7u, 153u, 191u, 272, 6);
    init_screen_text_box(&equip_list_box, 207u, 122u, 209u, 303,
      6, small_med_font, 1);
    init_screen_text_box(&equip_display_box, 425u, 153u, 208u, 272,
      6, small_font, 3);
    init_screen_button(&equip_offer_buy_button, 430u, 404u,
      gui_strings[436], 6, med2_font, 1, 0);
    init_screen_info_box(&equip_cost_box, 504u, 404u, 124u,
      gui_strings[442], misc_text[0], 6, med_font, small_med_font, 1);
    weapon_slots.SpecialDrawFn = ac_show_weapon_slots;
    equip_name_box.DrawTextFn = ac_show_weapon_name;
    equip_name_box.Text = unkn41_text;
    equip_list_box.ScrollWindowOffset += 27;
    equip_name_box.Font = med_font;
    equip_display_box.DrawTextFn = ac_display_weapon_info;
    equip_list_box.DrawTextFn = ac_show_weapon_list;
    equip_cost_box.Text2 = equip_cost_text;
    equip_display_box.Flags |= 0x0300;
    equip_display_box.ScrollWindowHeight = 117;
    equip_list_head_box.DrawTextFn = ac_show_title_box;
    equip_list_head_box.Text = gui_strings[408];
    equip_list_head_box.Font = med_font;
    equip_list_box.Flags |= 0x0300;
    equip_list_box.BGColour = unk2_sprites[15].SHeight + 3;
    equip_list_box.ScrollWindowHeight -= 27;

    lbFontPtr = med2_font;
    if (my_string_width(gui_strings[436]) <= my_string_width(gui_strings[407]))
        s = gui_strings[407];
    else
        s = gui_strings[436];
    equip_offer_buy_button.Width = my_string_width(s) + 4;
    equip_offer_buy_button.CallBackFn = ac_do_equip_offer_buy;

    init_screen_button(&equip_all_agents_button, 7u, 96u,
      gui_strings[534], 6, med2_font, 1, 0);
    equip_all_agents_button.Width = 165;
    equip_all_agents_button.RadioValue = 4;
    equip_all_agents_button.Flags |= 0x0100;
    equip_all_agents_button.Radio = &selected_agent;

    start_x = (scr_w - weapon_slots.Width - equip_list_box.Width - equip_name_box.Width - 32) / 2;

    equip_all_agents_button.X = start_x + 7;
    equip_list_head_box.X = start_x + 7;
    weapon_slots.X = start_x + 7;
    equip_list_box.X = weapon_slots.X + weapon_slots.Width + 9;
    equip_name_box.X = equip_list_box.X + equip_list_box.Width + 9;
    equip_display_box.X = equip_name_box.X;
    equip_offer_buy_button.X = equip_display_box.X + 5;
    equip_cost_box.X = equip_offer_buy_button.X + equip_offer_buy_button.Width + 4;
    equip_cost_box.Width = equip_list_box.Width - 2 - equip_offer_buy_button.Width - 14;
}

void reset_equip_screen_boxes_flags(void)
{
    equip_list_head_box.Flags = 0x0001;
    weapon_slots.Flags = 0x0001;
    equip_name_box.Flags = 0x0001;
    equip_cost_box.Flags = 0x0001;
    equip_list_box.Flags = 0x0001 | 0x0100 | 0x0200;
    equip_display_box.Flags = 0x0001 | 0x0100 | 0x0200;
}

void set_flag01_equip_screen_boxes(void)
{
    equip_all_agents_button.Flags |= 0x0001;
    equip_offer_buy_button.Flags |= 0x0001;
    if (screentype == SCRT_CRYO)
        equip_cost_box.Flags |= 0x0008;
}
/******************************************************************************/
