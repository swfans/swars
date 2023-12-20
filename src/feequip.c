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
#include "bfmemut.h"
#include "bfscrcopy.h"
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
extern struct ScreenShape unk11_menu[5];

extern ubyte byte_155174; // = 166;
extern ubyte byte_155180; // = 109;
extern ubyte byte_1C4AA0;
extern ubyte cheat_research_weapon;
extern ubyte byte_1C4975;
extern ubyte byte_1C4976;

extern ubyte mo_from_agent;
extern struct TbSprite *sprites_Icons0_0;

extern char unkn41_text[];
extern char equip_cost_text[20];
extern ubyte byte_155175[];
extern ubyte byte_155181[];
extern ubyte weapon_nrg[31];
extern ubyte weapon_range[31];
extern ubyte weapon_damage[31];

short agent_panel_shape_points_x[] = {
      0,  23, 120, 103,  35,  22,   7,   0,   0,
};
short agent_panel_shape_points_y[] = {
     23,   0,   0,  17,  17,  30,  30,  23,  23,
};
short agent_name_shape_points_x[] = {
      0, 181, 164,   0,   0,
};
short agent_name_shape_points_y[] = {
      0,   0,  17,  17,   0,
};

ubyte ac_display_weapon_info(struct ScreenTextBox *box);
ubyte ac_show_weapon_name(struct ScreenTextBox *box);
ubyte ac_show_weapon_list(struct ScreenTextBox *box);
ubyte ac_show_weapon_slots(struct ScreenBox *box);
ubyte ac_do_equip_offer_buy(ubyte click);
ubyte ac_sell_equipment(ubyte click);
ubyte ac_select_all_agents(ubyte click);
void ac_weapon_flic_data_to_screen(void);

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

void set_flag02_equipment_screen_boxes(void)
{
    short i;

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
}

TbBool weapon_available_for_purchase(short weapon)
{
    return ((weapon != WEP_NULL) && (weapon != WEP_ENERGYSHLD) && (weapon != WEP_NAPALMMINE)
            && (weapon != WEP_SONICBLAST) && ((weapon != WEP_PERSUADER2) || (background_type != 1))
            && (research.WeaponsCompleted & (1 << (weapon-1))))
            || (login_control__State == 5 && login_control__TechLevel >= weapon_tech_level[weapon]);
}

ubyte draw_agent_panel_shape(struct ScreenShape *shape, ushort spridx, ubyte gbstate)
{
    ubyte drawn;
    if (byte_1C4975 == 0)
    {
        if ((gbstate == GBxSta_NORMAL) || (gbstate == GBxSta_HLIGHT1)) {
            lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
        } else {
            lbDisplay.DrawFlags = 0;
        }
        drawn = flashy_draw_purple_shape(shape);
    }
    else
    {
        if ((gbstate == GBxSta_NORMAL) || (gbstate == GBxSta_HLIGHT1) || (gbstate == GBxSta_HLIGHT2)) {
            lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
        } else {
            lbDisplay.DrawFlags = 0;
        }
        draw_sprite_purple_list(shape->PtX[0] - 59, shape->PtY[2] - 3,
          &sprites_Icons0_0[139]);
        if ((gbstate == GBxSta_HLIGHT2) || (gbstate == GBxSta_HLIGHT1)) {
            lbDisplay.DrawFlags = 0;
        }
        if (gbstate == GBxSta_HLIGHT2) {
            draw_sprite_purple_list(shape->PtX[0] - 59, shape->PtY[2] - 3,
              &sprites_Icons0_0[144]);
        }
        lbDisplay.DrawFlags |= 0x8000;
        draw_sprite_purple_list(shape->PtX[1] - 16, shape->PtY[1] + 8,
          &sprites_Icons0_0[spridx]);
        lbDisplay.DrawFlags &= ~0x8000;
        drawn = 3;
    }
    lbDisplay.DrawFlags = 0;
    return drawn;
}

/** Checks if mouse is over the shape which is agent panel.
 *
 * This will work properly on a shape which represents agent panel. It is optimized
 * for it by checking two slant boxes rather than any shape.
 */
TbBool mouse_over_agent_panel_shape(struct ScreenShape *shape)
{
    return (mouse_move_over_rect_adv(shape->PtX[0] + 6, shape->PtY[0] - 6, 30, 15, 0) ||
      mouse_move_over_rect_adv(shape->PtX[1], shape->PtY[1], 98, 18, 0));
}

void show_equipment_screen(void)
{
#if 0
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
        lbKeyOn[KC_SPACE] = 0;
        set_flag02_heading_screen_boxes();
        set_flag02_equipment_screen_boxes();
        byte_1C4975 = 1;
        byte_1C4976 = 1;
    }
    if ((ingame.UserFlags & UsrF_Cheats) != 0)
    {
        if ( lbKeyOn[KC_0] )
        {
            lbKeyOn[KC_0] = 0;
            if (cheat_research_weapon < WEP_TYPES_COUNT) {
                refresh_equip_list = 1;
                research_weapon_complete(cheat_research_weapon + 1);
                cheat_research_weapon++;
            }
        }
    }
    if (refresh_equip_list)
    {
        short weapon;
        equip_list_box.Lines = 0;
        for (weapon = 1; weapon < WEP_TYPES_COUNT; weapon++)
        {
            if (weapon_available_for_purchase(weapon)) {
                equip_list_box.Lines++;
            }
        }
        equip_list_box.Flags |= 0x0080;
        refresh_equip_list = 0;
    }
    if (draw_heading_box())
    {
        sbyte nagent;
        ubyte agnt[4];
        ubyte boxes_drawn;
        boxes_drawn = 1;
        for (nagent = 4; nagent >= 0; nagent--)
        {
            struct ScreenShape *shape;

            shape = &unk11_menu[nagent];
            if (nagent == 4) // agent name box
            {
                if (byte_1C4976 == 0)
                {
                    byte_1C4976 = flashy_draw_purple_shape(shape) == 3;
                }
                else if (byte_1C4976 == 1)
                {
                    lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
                    draw_box_purple_list(shape->PtX[0] - 3, shape->PtY[0] - 3,
                      168, 24, 56);
                    draw_triangle_purple_list(shape->PtX[1] + 7, shape->PtY[1] - 3,
                      shape->PtX[2] + 1, shape->PtY[2] + 3,
                      shape->PtX[2] + 1, shape->PtY[1] - 3, 56);
                    lbDisplay.DrawFlags = 0;
                    flashy_draw_purple_shape(shape);
                    if (/*(selected_agent >= 0) && */(selected_agent < 4))
                    {
                        const char *name;
                        lbFontPtr = med_font;
                        my_set_text_window(shape->PtX[0] + 4, shape->PtY[0] + 4,
                          shape->PtX[2] - shape->PtX[0] - 1, shape->PtY[2] - shape->PtY[0] + 1);
                        name = get_cryo_agent_name(selected_agent);
                        draw_text_purple_list2(0, 0, name, 0);
                    }
                }
            }
            else
            {
                ubyte gbstate, drawn;
                ushort spridx;

                if (lbKeyOn[KC_1 + nagent])
                {
                    lbKeyOn[KC_1 + nagent] = 0;
                    if (nagent < cryo_agents.NumAgents)
                    {
                        selected_agent = nagent;
                        check_buy_sell_button();
                    }
                }
                if (mouse_over_agent_panel_shape(shape))
                {
                    if ((shape->Flags & 0x0200) == 0) {
                        play_sample_using_heap(0, 123, 127, 64, 100, 0, 1u);
                        shape->Flags |= 0x0200;
                    }
                    if (lbDisplay.MLeftButton || (joy.Buttons[0] != 0))
                    {
                        lbDisplay.LeftButton = 0;
                        shape->Flags |= 0x0400;
                        gbstate = GBxSta_PUSHED;
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
                                shape->Flags &= ~0x0400;
                            }
                        }
                        else
                        {
                            if (free_slot(nagent, mo_weapon))
                            {
                                player_cryo_transfer_weapon_between_agents(mo_from_agent, nagent, mo_weapon+1);
                            }
                            mo_weapon = -1;
                            shape->Flags &= ~0x0400;
                        }
                        gbstate = GBxSta_HLIGHT2;
                    }
                }
                else
                {
                    if (shape->Flags & 0x0200)
                        shape->Flags &= ~0x0200;
                    if (shape->Flags & 0x0400)
                        shape->Flags &= ~0x0400;
                    if ((selected_agent == nagent) || (selected_agent == 4))
                    {
                        gbstate = GBxSta_HLIGHT1;
                    }
                    else
                    {
                        gbstate = GBxSta_NORMAL;
                    }
                }
                spridx = 140 + nagent;
                drawn = draw_agent_panel_shape(shape, spridx, gbstate);
                // Is the flashy draw finished for current button
                agnt[nagent] = (drawn == 3);
                // When all buttons started actually drawing, we can begin flashy draw of the panels below
                boxes_drawn &= (drawn > 1);
            }
        }

        if (!byte_1C4975) {
            byte_1C4975 = agnt[0] && agnt[1] && agnt[2] && agnt[3];
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

void weapon_flic_data_to_screen(void)
{
#if 0
    asm volatile ("call ASM_weapon_flic_data_to_screen\n"
        :  :  : "eax" );
#else
    short w, h;

    w = equip_display_box.Width - 8;
    h = w * 7 / 10;
    LbScreenSetGraphicsWindow(equip_display_box.X + 4, equip_display_box.Y + 4, w, h);
    LbScreenCopy(unkn_buffer_05, lbDisplay.GraphicsWindowPtr, lbDisplay.GraphicsWindowHeight);
    LbScreenSetGraphicsWindow(0, 0, lbDisplay.GraphicsScreenWidth,
        lbDisplay.GraphicsScreenHeight);
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

/** Determines if buy or sell should be available in the equip weapon offer.
 *
 * @return Gives 0 if button unavailable, 1 for buy, 2 for sell.
 */
ubyte equip_offer_can_buy_or_sell(ubyte weapon)
{
    if (selected_agent == 4) // All agents selected
    {
        short i;

        if (is_research_weapon_completed(weapon) ||
          (login_control__State != 6))
            return 1;

        for (i = 0; i < 4; i++)
        {
            if (!player_agent_has_weapon(i, weapon))
              break;
        }
        if (i == 4) // all agents have that weapon
            return 2;

        return 0;
    }

    if (player_agent_has_weapon(selected_agent, weapon))
        return 2;

    if (is_research_weapon_completed(weapon) ||
      (login_control__State != 6))
        return 1;

    return 0;
}

ubyte display_weapon_info(struct ScreenTextBox *box)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_display_weapon_info\n"
        : "=r" (ret) : "a" (box));
    return ret;
#else
    short cx, cy;
    short i, k;
    const char *text;

    if ((box->Flags & 0x8000) == 0)
    {
        lbDisplay.DrawFlags = 0x0004;
        my_set_text_window(box->X + 4, box->Y + 4,
            box->Width - 8, box->Height - 8);
        draw_box_purple_list(box->X + 4, box->Y + 158,
            box->Width - 8, 13, 56);
        draw_box_purple_list(box->X + 4, box->Y + 174,
            box->Width - 8, 73, 56);
        lbDisplay.DrawFlags = 0x0100;
        lbFontPtr = small_med_font;
        draw_text_purple_list2(0, 144, gui_strings[426], 0);
        draw_text_purple_list2(0, 173, gui_strings[427], 0);
        draw_text_purple_list2(0, 198, gui_strings[428], 0);
        draw_text_purple_list2(0, 223, gui_strings[429], 0);
        cy = box->Y + 187;
        for (i = 0; i < 3; i++)
        {
            cx = box->X + 8;
            for (k = 0; k < 8; k++)
            {
                if (i == 1)
                    draw_box_purple_list(cx, cy, 22, 7, byte_155180);
                else
                    draw_box_purple_list(cx, cy, 22, 7, byte_155174);
                cx += 24;
            }
            cy += 25;
        }
        lbDisplay.DrawFlags = 0;
        box->Flags |= 0x8000;
        copy_box_purple_list(box->X + 4, box->Y + 146, box->Width - 8, box->Height - 143);
    }
    my_set_text_window(box->X + 4, box->Y + 4, box->Width - 8, box->Height - 8);
    if (selected_weapon == -1)
        return 0;
    lbDisplay.DrawFlags = 0x0100;
    lbFontPtr = small_med_font;

    if (is_research_weapon_completed(selected_weapon + 1) || (login_control__State != 6))
        text = gui_strings[59 + (weapon_defs[selected_weapon + 1].Sprite >> 8)];
    else
        text = gui_strings[65];
    draw_text_purple_list2(0, 157, text, 0);

    cy = box->Y + 187;
    cx = box->X + 8;
    for (i = 0; i < 8; i++)
    {
        if (i < weapon_damage[selected_weapon])
        draw_box_purple_list(cx, cy, 22, 7, byte_155175[i]);
        cx += 24;
    }

    cy = cy + 25;
    cx = box->X + 8;
    for (i = 0; i < 8; i++)
    {
        if (i < weapon_range[selected_weapon])
            draw_box_purple_list(cx, cy, 22, 7, byte_155181[i]);
        cx += 24;
    }

    cy = cy + 25;
    cx = box->X + 8;
    for (i = 0; i < 8; i++)
    {
        if (i < weapon_nrg[selected_weapon])
            draw_box_purple_list(cx, cy, 22, 7, byte_155175[i]);
        cx += 24;
    }

    if (equip_offer_can_buy_or_sell(selected_weapon + 1))
    {
        //equip_offer_buy_button.DrawFn(&equip_offer_buy_button); -- incompatible calling convention
        asm volatile ("call *%1\n"
            : : "a" (&equip_offer_buy_button), "g" (equip_offer_buy_button.DrawFn));
    }

    //equip_cost_box.DrawFn(&equip_cost_box); -- incompatible calling convention
    asm volatile ("call *%1\n"
        : : "a" (&equip_cost_box), "g" (equip_cost_box.DrawFn));

    draw_hotspot_purple_list(529, 257);

    if (byte_1C4AA0)
    {
        lbFontPtr = box->Font;
        my_set_text_window(box->X + 4, box->ScrollWindowOffset + box->Y + 4,
          box->Width - 20, box->ScrollWindowHeight + 23);
        flashy_draw_text(0, 0, box->Text, box->TextSpeed, box->field_38,
          &box->TextFadePos, 0);
    }
    else
    {
        xdo_next_frame(2);
        draw_flic_purple_list(ac_weapon_flic_data_to_screen);
    }

    if (lbDisplay.LeftButton && mouse_down_over_box_coords(429, 157, 629, 297))
    {
        lbDisplay.LeftButton = 0;
        byte_1C4AA0 = (byte_1C4AA0 == 0);
        if (!byte_1C4AA0)
        {
            init_weapon_anim(selected_weapon);
            return 0;
        }
        equip_display_box.Flags |= 0x0080;
        equip_display_box.Lines = 0;
        if (is_research_weapon_completed(selected_weapon + 1) ||
          (login_control__State != 6))
            box->Text = &weapon_text[weapon_text_index[selected_weapon]];
        else
            box->Text = gui_strings[536];
        box->TextFadePos = -5;
    }

    return 0;
#endif
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

void init_equip_screen_shapes(void)
{
#if 0
    asm volatile ("call ASM_init_equip_screen_shapes\n"
        :  :  : "eax" );
#else
    ushort i, k;
    short x, y;
    short scr_w, start_x;

    scr_w = lbDisplay.GraphicsWindowWidth;

    start_x = (scr_w - 640) / 2;

    x = 178 + start_x;
    y = 72;
    for (i = 0; i < 4; i++)
    {
        LbMemoryCopy(unk11_menu[i].PtX, agent_panel_shape_points_x, sizeof(agent_panel_shape_points_x));
        LbMemoryCopy(unk11_menu[i].PtY, agent_panel_shape_points_y, sizeof(agent_panel_shape_points_y));
        for (k = 0; k < 9; k++)
        {
            if (k < sizeof(agent_panel_shape_points_x)/sizeof(agent_panel_shape_points_x[0]))
            {
                unk11_menu[i].PtX[k] = agent_panel_shape_points_x[k] + x;
                unk11_menu[i].PtY[k] = agent_panel_shape_points_y[k] + y;
            }
            else
            {
                unk11_menu[i].PtX[k] = 0;
                unk11_menu[i].PtY[k] = 0;
            }
        }
        unk11_menu[i].field_24 = 6;
        unk11_menu[i].field_25 = 0;
        unk11_menu[i].Flags = 0x01;
        unk11_menu[i].Colour = 174;
        unk11_menu[i].BGColour = 8;
        x += 110;
    }
    x = 7 + start_x;
    y = 72;
    i = 4;
    {
        for (k = 0; k < 9; k++)
        {
            if (k < sizeof(agent_name_shape_points_x)/sizeof(agent_name_shape_points_x[0]))
            {
                unk11_menu[i].PtX[k] = agent_name_shape_points_x[k] + x;
                unk11_menu[i].PtY[k] = agent_name_shape_points_y[k] + y;
            }
            else
            {
                unk11_menu[i].PtX[k] = 0;
                unk11_menu[i].PtY[k] = 0;
            }
        }
        unk11_menu[i].field_24 = 6;
        unk11_menu[i].field_25 = 0;
        unk11_menu[i].Flags = 0x01;
        unk11_menu[i].Colour = 247;
        unk11_menu[i].BGColour = 4;
    }
#endif
}

void reset_equip_screen_boxes_flags(void)
{
    short i;

    equip_list_head_box.Flags = 0x0001;
    weapon_slots.Flags = 0x0001;
    equip_name_box.Flags = 0x0001;
    equip_cost_box.Flags = 0x0001;
    equip_list_box.Flags = 0x0001 | 0x0100 | 0x0200;
    equip_display_box.Flags = 0x0001 | 0x0100 | 0x0200;
    for (i = 0; i < 5; i++) {
        unk11_menu[i].Flags = 0x0001;
    }
}

void set_flag01_equip_screen_boxes(void)
{
    equip_all_agents_button.Flags |= 0x0001;
    equip_offer_buy_button.Flags |= 0x0001;
    if (screentype == SCRT_CRYO)
        equip_cost_box.Flags |= 0x0008;
}
/******************************************************************************/
