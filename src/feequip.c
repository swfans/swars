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
extern ubyte mo_from_agent;
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
#if 1
    asm volatile ("call ASM_show_equipment_screen\n"
        :  :  : "eax" );
#else
    if ((unk11_menu[0].Flags & 0x01) != 0)
    {
        byte_1C4975 = 0;
        byte_1C4976 = 0;
    }
    if (((game_projector_speed != 0) && is_heading_flag01()) ||
      (lbKeyOn[KC_SPACE] && !edit_flag))
    {
        short i;
        lbKeyOn[KC_SPACE] = 0;
        set_flag02_heading_screen_boxes();
        equip_list_head_box.Flags |= 0x02;
        weapon_slots.Flags |= 0x02;
        equip_display_box.Flags |= 0x02;
        equip_name_box.Flags |= 0x02;
        equip_list_box.Flags |= 0x02;
        equip_offer_buy_button.Flags |= 0x02;
        equip_cost_box.Flags |= 0x02;
        equip_all_agents_button.Flags |= 0x02;
        for (i = 0; i < 5; i++) {
            unk11_menu[i].Flags = 0x02;
        }
        byte_1C4975 = 1;
        byte_1C4976 = 1;
    }
    if ((ingame.UserFlags & 0x04) != 0)
    {
        if ( lbKeyOn[KC_0] )
        {
            lbKeyOn[KC_0] = 0;
            if (cheat_research_weapon < WEP_TYPES_COUNT) {
                refresh_equip_list = 1;
                research.WeaponsCompleted |= 1 << cheat_research_weapon;
                cheat_research_weapon++;
            }
        }
    }
    if (refresh_equip_list)
    {
        short wtype;
        equip_list_box.Lines = 0;
        for (wtype = 1; wtype < WEP_TYPES_COUNT; wtype++)
        {
          if (((wtype != WEP_ENERGYSHLD) && (wtype != WEP_NAPALMMINE) && (wtype != WEP_SONICBLAST)
            && ((wtype != WEP_PERSUADER2) || (background_type != 1))
            && (research.WeaponsCompleted & (1 << (wtype-1))))
            || (login_control__State == 5 && login_control__TechLevel >= weapon_tech_level[wtype]))
          {
            equip_list_box.Lines++;
          }
        }
        equip_list_box.Flags |= 0x0080;
        refresh_equip_list = 0;
    }
    if (draw_heading_box())
    {
        short delta_h = 110;
        sbyte nagent;
        ubyte v45[4];
        ubyte boxes_drawn;
        boxes_drawn = 1;
        for (nagent = 4; nagent >= 0; nagent--)
        {
            struct ScreenShape *shape;
            ushort spridx;

            shape = &unk11_menu[nagent];
            spridx = 140 + nagent;
            if (nagent == 4)
            {
                if (byte_1C4976 == 0)
                {
                    byte_1C4976 = flashy_draw_purple_shape(shape) == 3;
                }
                else if (byte_1C4976 == 1)
                {
                    lbDisplay.DrawFlags = 0x0004;
                    draw_box_purple_list(shape->field_0[0] - 3, shape->field_12[0] - 3,
                      168, 24, 56);
                    draw_triangle_purple_list(shape->field_0[1] + 7, shape->field_12[1] - 3,
                      shape->field_0[2] + 1, shape->field_12[2] + 3,
                      shape->field_0[2] + 1, shape->field_12[1] - 3, 56);
                    lbDisplay.DrawFlags = 0;
                    flashy_draw_purple_shape(shape);
                    if (/*(selected_agent >= 0) && */(selected_agent < 4))
                    {
                        ushort rndname;
                        lbFontPtr = med_font;
                        my_set_text_window(11, 76, 163, 18);
                        rndname = cryo_agents.RandomName[selected_agent];
                        if (background_type == 1)
                        {
                            if (cryo_agents.Sex & (1 << selected_agent))
                                draw_text_purple_list2(0, 0, gui_strings[227 + rndname], 0);
                            else
                                draw_text_purple_list2(0, 0, gui_strings[177 + rndname], 0);
                        }
                        else
                        {
                            draw_text_purple_list2(0, 0, gui_strings[77 + rndname], 0);
                        }
                    }
                }
            }
            else
            {
                v45[nagent] = 0;
                if (lbKeyOn[KC_1 + nagent])
                {
                    lbKeyOn[KC_1 + nagent] = 0;
                    if (cryo_agents.NumAgents > nagent)
                    {
                        selected_agent = nagent;
                        check_buy_sell_button();
                    }
                }
                if (mouse_move_over_rect_adv(224 + delta_h * nagent, 89, 30, 15, 0) ||
                  mouse_move_over_rect_adv(201 + delta_h * nagent, 72, 98, 18, 0))
                {
                    if ((shape->Flags & 0x200) == 0) {
                        play_sample_using_heap(0, 123, 127, 64, 100, 0, 1u);
                        shape->Flags |= 0x0200;
                    }
                    if (lbDisplay.MLeftButton || (joy.Buttons[0] != 0))
                    {
                        lbDisplay.LeftButton = 0;
                        shape->Flags |= 0x0400;
                        if (byte_1C4975 == 0)
                        {
                            boxes_drawn = flashy_draw_purple_shape(shape);
                            v45[nagent] = boxes_drawn == 3;
                        }
                        else
                        {
                            draw_sprite_purple_list(shape->field_0[0] - 59, shape->field_12[2] - 3,
                              &sprites_Icons0_0[139]);
                            lbDisplay.DrawFlags |= 0x8000;
                            draw_sprite_purple_list(shape->field_0[1] - 16, shape->field_12[1] + 8,
                              &sprites_Icons0_0[spridx]);
                            lbDisplay.DrawFlags &= ~0x8000;
                        }
                    }
                    else
                    {
                        if ((nagent >= cryo_agents.NumAgents) || (mo_weapon == -1))
                        {
                            if ((shape->Flags & 0x0400) != 0)
                            {
                                if (nagent < cryo_agents.NumAgents)
                                {
                                    play_sample_using_heap(0, 111, 127, 64, 100, 0, 2u);
                                    selected_agent = nagent;
                                    check_buy_sell_button();
                                }
                                else
                                {
                                    play_sample_using_heap(0, 129, 127, 64, 100, 0, 2u);
                                }
                                shape->Flags &= 0x0400;
                            }
                        }
                        else
                        {
                            if (free_slot(nagent, mo_weapon))
                            {
                                player_transfer_weapon_between_agents(mo_from_agent, nagent, mo_weapon+1);
                            }
                            mo_weapon = -1;
                            shape->Flags &= ~0x0400;
                        }
                        if (byte_1C4975 == 0)
                        {
                            boxes_drawn = flashy_draw_purple_shape(shape);
                            v45[nagent] = boxes_drawn == 3;
                        }
                        else
                        {
                            lbDisplay.DrawFlags = 0x0004;
                            draw_sprite_purple_list(shape->field_0[0] - 59, shape->field_12[2] - 3,
                              &sprites_Icons0_0[139]);
                            lbDisplay.DrawFlags = 0;
                            draw_sprite_purple_list(shape->field_0[0] - 59, shape->field_12[2] - 3,
                              &sprites_Icons0_0[144]);
                            lbDisplay.DrawFlags |= 0x8000;
                            draw_sprite_purple_list(shape->field_0[1] - 16, shape->field_12[1] + 8,
                              &sprites_Icons0_0[spridx]);
                            lbDisplay.DrawFlags &= ~0x8000;
                        }
                    }
                }
                else
                {
                    if (shape->Flags & 0x0200)
                        shape->Flags &= ~0x0200;
                    if (shape->Flags & 0x0400)
                        shape->Flags &= ~0x0400;
                    lbDisplay.DrawFlags = 0x0004;
                    if (byte_1C4975)
                    {
                        draw_sprite_purple_list(
                          shape->field_0[0] - 59,
                          shape->field_12[2] - 3,
                          sprites_Icons0_0 + 139);
                        if ((selected_agent == nagent) || (selected_agent == 4))
                            lbDisplay.DrawFlags = 0;
                        lbDisplay.DrawFlags |= 0x8000;
                        draw_sprite_purple_list(shape->field_0[1] - 16, shape->field_12[1] + 8,
                          &sprites_Icons0_0[spridx]);
                        lbDisplay.DrawFlags &= ~0x8000;
                    }
                    else
                    {
                        boxes_drawn = flashy_draw_purple_shape(shape);
                        v45[nagent] = boxes_drawn == 3;
                    }
                    lbDisplay.DrawFlags = 0;
                }
            }
        }

        if (!byte_1C4975) {
            byte_1C4975 = v45[0] && v45[1] && v45[2] && v45[3];
        }
        if (boxes_drawn)
        {
            ubyte ret;
            //equip_all_agents_button.DrawFn(&equip_all_agents_button); -- incompatible calling convention
            asm volatile ("call *%2\n"
                : "=r" (ret) : "a" (&equip_all_agents_button), "g" (equip_all_agents_button.DrawFn));
            //equip_list_head_box.DrawFn(&equip_list_head_box); -- incompatible calling convention
            asm volatile ("call *%2\n"
                : "=r" (ret) : "a" (&equip_list_head_box), "g" (equip_list_head_box.DrawFn));
            //if (weapon_slots.DrawFn(&weapon_slots)) -- incompatible calling convention
            asm volatile ("call *%2\n"
                : "=r" (ret) : "a" (&weapon_slots), "g" (weapon_slots.DrawFn));
            if (ret)
            {
                //if (equip_list_box.DrawFn(&equip_list_box)) { -- incompatible calling convention
                asm volatile ("call *%2\n"
                    : "=r" (ret) : "a" (&equip_list_box), "g" (equip_list_box.DrawFn));
                if (ret) {
                    //equip_name_box.DrawFn(&equip_name_box); { -- incompatible calling convention
                    asm volatile ("call *%2\n"
                        : "=r" (ret) : "a" (&equip_name_box), "g" (equip_name_box.DrawFn));
                    //equip_display_box.DrawFn(&equip_display_box); { -- incompatible calling convention
                    asm volatile ("call *%2\n"
                        : "=r" (ret) : "a" (&equip_display_box), "g" (equip_display_box.DrawFn));
                }
            }
        }
    }
    if (mo_weapon != -1)
    {
        short ms_x, ms_y;
        struct WeaponDef *wdef;

        wdef = &weapon_defs[mo_weapon + 1];
        ms_x = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MMouseX : lbDisplay.MMouseX;
        ms_y = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MMouseY : lbDisplay.MMouseY;
        draw_sprite_purple_list(ms_x, ms_y, &unk2_sprites[(wdef->Sprite & 0xFF) + 27]);
    }
#endif
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
