/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file purpldrw.c
 *     Adding items to drawlist for purple projector screens.
 * @par Purpose:
 *     Implements functions for filling items within drawlist.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     22 Apr 2024 - 28 Sep 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "purpldrw.h"

#include "bfkeybd.h"
#include "bfutility.h"
#include "bfscreen.h"
#include "bfsprite.h"
#include "bftext.h"
#include "poly.h"
#include "bflib_joyst.h"
#include "ssampply.h"

#include "display.h"
#include "game.h"
#include "game_sprts.h"
#include "guiboxes.h"
#include "guitext.h"
#include "purpldrwlst.h"
#include "sound.h"
#include "swlog.h"
/******************************************************************************/
// TODO avoid referring to a specific box
extern struct ScreenTextBox brief_netscan_box;
extern struct ScreenTextBox world_city_info_box;
extern struct ScreenTextBox equip_display_box;
extern struct ScreenTextBox cryo_cybmod_list_box;

extern long dword_1DC5FC;
extern long dword_1DC600;
extern long dword_1DC624;
extern long dword_1DC628;

void draw_line_purple_list(int x1, int y1, int x2, int y2, int colour)
{
#if 0
    asm volatile (
      "push %4\n"
      "call ASM_draw_line_purple_list\n"
        : : "a" (x1), "d" (y1), "b" (x2), "c" (y2), "g" (colour));
    return;
#endif
    struct PurpleDrawItem *pditem;

    pditem = &purple_draw_list[purple_draw_index];
    purple_draw_index++;

    pditem->U.Line.X1 = lbDisplay.GraphicsWindowX + x1;
    pditem->U.Line.Y1 = lbDisplay.GraphicsWindowY + y1;
    pditem->U.Line.X2 = lbDisplay.GraphicsWindowX + x2;
    pditem->U.Line.Y2 = lbDisplay.GraphicsWindowY + y2;
    pditem->U.Line.Colour = colour;
    pditem->Flags = lbDisplay.DrawFlags;

    if ((x1 == x2) || (y1 == y2))
        pditem->Type = PuDT_HVLINE;
    else
        pditem->Type = PuDT_LINE;
}

void draw_box_purple_list(int x, int y, ulong width, ulong height, int colour)
{
#if 0
    asm volatile (
      "push %4\n"
      "call ASM_draw_box_purple_list\n"
        : : "a" (x), "d" (y), "b" (width), "c" (height), "g" (colour));
    return;
#endif
    struct PurpleDrawItem *pditem;

    pditem = &purple_draw_list[purple_draw_index];
    purple_draw_index++;

    pditem->U.Box.X = lbDisplay.GraphicsWindowX + x;
    pditem->U.Box.Y = lbDisplay.GraphicsWindowY + y;
    pditem->U.Box.Width = width;
    pditem->U.Box.Colour = colour;
    pditem->U.Box.Height = height;
    pditem->Flags = lbDisplay.DrawFlags;
    pditem->Type = PuDT_BOX;
}

void draw_text_purple_list2(int x, int y, const char *text, ushort line)
{
    asm volatile (
      "call ASM_draw_text_purple_list2\n"
        : : "a" (x), "d" (y), "b" (text), "c" (line));
}

void draw_sprite_purple_list(int x, int y, struct TbSprite *p_sprite)
{
#if 0
    asm volatile (
      "call ASM_draw_sprite_purple_list\n"
        : : "a" (x), "d" (y), "b" (p_sprite));
#endif
    struct PurpleDrawItem *pditem;

    pditem = &purple_draw_list[purple_draw_index];
    purple_draw_index++;

    pditem->U.Sprite.Sprite = p_sprite;
    pditem->U.Sprite.Colour = lbDisplay.DrawColour;
    pditem->Flags = lbDisplay.DrawFlags;
    pditem->U.Sprite.X = lbDisplay.GraphicsWindowX + x;
    pditem->U.Sprite.Y = lbDisplay.GraphicsWindowY + y;
    pditem->Type = PuDT_SPRITE;
}

void draw_trig_purple_list(long x2, long y2, long x3, long y3)
{
    asm volatile (
      "call ASM_draw_trig_purple_list\n"
        : : "a" (x2), "d" (y2), "b" (x3), "c" (y3));
}

void copy_box_purple_list(long x, long y, ulong width, ulong height)
{
    asm volatile (
      "call ASM_copy_box_purple_list\n"
        : : "a" (x), "d" (y), "b" (width), "c" (height));
}

void draw_hotspot_purple_list(int x, int y)
{
    asm volatile (
      "call ASM_draw_hotspot_purple_list\n"
        : : "a" (x), "d" (y));
}

ubyte flashy_draw_purple_shape(struct ScreenShape *shape)
{
    ubyte ret;
    asm volatile ("call ASM_flashy_draw_purple_shape\n"
        : "=r" (ret) : "a" (shape));
    return ret;
}

ubyte flashy_draw_purple_box(struct ScreenBox *p_box)
{
    ubyte ret;
    asm volatile ("call ASM_flashy_draw_purple_box\n"
        : "=r" (ret) : "a" (p_box));
    return ret;
}

ubyte flashy_draw_purple_text_box_text(struct ScreenTextBox *p_box)
{
    ubyte text_drawn;

    lbDisplay.DrawFlags = 0;
    if ((p_box->Flags & GBxFlg_TextRight) != 0)
    {
        lbDisplay.DrawFlags |= Lb_TEXT_HALIGN_RIGHT;
    }
    else if ((p_box->Flags & GBxFlg_TextCenter) != 0)
    {
        lbDisplay.DrawFlags |= Lb_TEXT_HALIGN_CENTER;
    }

    text_drawn = true;

    if ((p_box->Flags & GBxFlg_TextCopied) != 0)
        return text_drawn;

    if (p_box->DrawTextFn != NULL)
    {
        ubyte drawn;
        //drawn = p_box->DrawTextFn(p_box); -- incompatible calling convention
        asm volatile ("call *%2\n"
          : "=r" (drawn) : "a" (p_box), "g" (p_box->DrawTextFn));
        text_drawn = text_drawn && drawn;
    }
    else if (p_box->Text != NULL)
    {
        ubyte drawn;
        drawn = flashy_draw_text(0,  0, p_box->Text, p_box->TextSpeed, p_box->field_38,
              &p_box->TextFadePos, p_box->DrawTextFn != NULL);
        text_drawn = text_drawn && drawn;
    }

    if (text_drawn && ((p_box->Flags & GBxFlg_RadioBtn) == 0))
    {
          copy_box_purple_list(p_box->X - 3, p_box->Y - 3, p_box->Width + 6, p_box->Height + 6);
          p_box->Flags |= GBxFlg_TextCopied;
    }

    return text_drawn;
}

ubyte flashy_draw_purple_text_box_children(struct ScreenTextBox *p_box)
{
    ushort i;
    ubyte all_drawn;

    lbDisplay.DrawFlags = 0;
    for (i = 0; i < 2; i++)
    {
        struct ScreenButton *p_button;
        struct ScreenInfoBox *p_info;

        p_button = p_box->Buttons[i];
        if (p_button != NULL) {
            ubyte drawn;
            //p_button->DrawFn(p_button); -- incompatible calling convention
            asm volatile ("call *%2\n"
                : "=r" (drawn) : "a" (p_button), "g" (p_button->DrawFn));
            all_drawn = all_drawn && drawn;
        }
        p_info = p_box->Infos[i];
        if (p_info != NULL) {
            ubyte drawn;
            //p_info->DrawFn(p_info); -- incompatible calling convention
            asm volatile ("call *%2\n"
                : "=r" (drawn) : "a" (p_info), "g" (p_info->DrawFn));
            all_drawn = all_drawn && drawn;
        }
    }
    return all_drawn;
}

ubyte flashy_draw_purple_text_box(struct ScreenTextBox *p_box)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_flashy_draw_purple_text_box\n"
        : "=r" (ret) : "a" (p_box));
    return ret;
#endif
    short box_w, box_h;
    ushort spr1;
    short lines_visible;
    TbBool text_remains_dynamic; /**< text drawing callback never sets GBxFlg_TextCopied (maybe make this into a box flag?) */

    spr1 = 11 + (p_box->Colour1 != 247);

    text_remains_dynamic = (p_box == &world_city_info_box) || (p_box == &equip_display_box) || (p_box == &cryo_cybmod_list_box);

    {
        short scr_scroll_w, scr_scroll_h;

        if ((p_box->Flags & GBxFlg_RadioBtn) != 0)
            scr_scroll_w = 12;
        else
            scr_scroll_w = 0;

        if ((p_box->Buttons[0] != NULL) || (p_box->Infos[0] != NULL) || (p_box == &brief_netscan_box))
            scr_scroll_h = p_box->ScrollWindowHeight + 2;
        else
            scr_scroll_h = p_box->ScrollWindowHeight + 23;

        my_set_text_window(p_box->X + 4, p_box->ScrollWindowOffset + p_box->Y + 4, p_box->Width - 8 - scr_scroll_w, scr_scroll_h);
    }
    lbFontPtr = p_box->Font;
    byte_197160 = p_box->LineSpacing;
    if (p_box->LineHeight == 0)
        p_box->LineHeight = byte_197160 + font_height('A');
    {
        short text_window_w;
        text_window_w = text_window_y2 - text_window_y1 - 1;
        lines_visible = (text_window_w + 2) / p_box->LineHeight;
    }

    box_w = p_box->Width - 1;
    box_h = p_box->Height - 1;

    if ((p_box->Flags & GBxFlg_Unkn0001) != 0)
    {
        int i;

        p_box->Timer = 0;
        p_box->Flags &= ~(GBxFlg_Unkn0001|GBxFlg_Unkn0080);
        p_box->Flags |= GBxFlg_Unkn0080;

        for (i = 0; i < 2; i++)
        {
            struct ScreenButton *p_button;
            struct ScreenInfoBox *p_info;

            p_button = p_box->Buttons[i];
            if (p_button != NULL)
                p_button->Flags |= GBxFlg_Unkn0001;

            p_info = p_box->Infos[i];
            if (p_info != NULL)
                p_info->Flags |= GBxFlg_Unkn0001;
        }
    }

    if ((p_box->Flags & GBxFlg_Unkn0002) != 0)
    {
        const char *p_text;
        int i;

        p_box->Flags &= ~(GBxFlg_Unkn0002|GBxFlg_Unkn0001);
        p_box->Timer = -1;

        p_text = p_box->Text;
        if (p_text != NULL)
            p_box->TextFadePos = my_str_len(p_text);
        else
            p_box->TextFadePos = -5;

        for (i = 0; i < 2; i++)
        {
            struct ScreenButton *p_button;
            struct ScreenInfoBox *p_info;

            p_button = p_box->Buttons[i];
            if (p_button != NULL)
                p_button->Flags |= GBxFlg_Unkn0002;

            p_info = p_box->Infos[i];
            if (p_info != NULL)
                p_info->Flags |= GBxFlg_Unkn0002;
        }
    }

    if (p_box->Timer <= 24)
    {
        struct TbSprite *p_spr;
        int scr_x1, scr_y1, scr_x2, scr_y2;
        ushort advance;

        advance = p_box->Timer;
        scr_x1 = proj_origin.X + (advance * (p_box->X - proj_origin.X)) / 24;
        scr_y1 = proj_origin.Y + (advance * (p_box->Y - proj_origin.Y)) / 24;
        scr_x2 = proj_origin.X + (advance * (p_box->X + box_w - proj_origin.X)) / 24;
        scr_y2 = proj_origin.Y + (advance * (p_box->Y + box_h - proj_origin.Y)) / 24;

        lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
        draw_line_purple_list(proj_origin.X, proj_origin.Y, scr_x1, scr_y1, p_box->Colour1);
        draw_line_purple_list(proj_origin.X, proj_origin.Y, scr_x2, scr_y2, p_box->Colour1);
        lbDisplay.DrawFlags = 0;

        p_spr = &fe_mouseptr_sprites[spr1];
        draw_sprite_purple_list(scr_x1 - 1, scr_y1 - 1, p_spr);
        draw_sprite_purple_list(scr_x2 - 1, scr_y2 - 1, p_spr);

        p_box->Timer += p_box->DrawSpeed;
        return 0;
    }

    if (p_box->Timer <= 72)
    {
        if (!IsSamplePlaying(0, 4, 0))
            play_sample_using_heap(0, 110, 127, 64, 100, 0, 1u);
    }

    if (p_box->Timer <= 48)
    {
        struct TbSprite *p_spr;
        short scr_x, scr_y;
        ushort advance;

        advance = ((p_box->Timer - 24) * box_w) / 24;
        vec_colour = p_box->Colour1;
        dword_1DC5FC = p_box->X + advance;
        dword_1DC600 = p_box->Y;
        dword_1DC624 = dword_1DC5FC - 15;
        if (dword_1DC624 < p_box->X)
            dword_1DC624 = p_box->X;
        dword_1DC628 = p_box->Y;
        draw_trig_purple_list(dword_1DC5FC, dword_1DC600, dword_1DC624, dword_1DC628);
        draw_line_purple_list(p_box->X, dword_1DC600, dword_1DC5FC, dword_1DC628, p_box->Colour1);

        dword_1DC5FC = p_box->X + box_w - advance;
        dword_1DC600 = p_box->Y + box_h;
        dword_1DC624 = dword_1DC5FC + 15;
        if (dword_1DC624 > p_box->X + box_w)
            dword_1DC624 = p_box->X + box_w;
        dword_1DC628 = p_box->Y + box_h;
        draw_trig_purple_list(dword_1DC5FC, dword_1DC600, dword_1DC624, dword_1DC628);
        draw_line_purple_list(dword_1DC5FC, dword_1DC600, p_box->X + box_w, dword_1DC628, p_box->Colour1);

        p_spr = &fe_mouseptr_sprites[spr1];
        scr_x = p_box->X + advance - 1;
        scr_y = p_box->Y - 1;
        draw_sprite_purple_list(scr_x, scr_y, p_spr);
        scr_x = p_box->X + box_w - advance - 1;
        scr_y = p_box->Y + p_box->Height - 2;
        draw_sprite_purple_list(scr_x, scr_y, p_spr);

        p_box->Timer += p_box->DrawSpeed;
        return 0;
    }

    if (p_box->Timer <= 72)
    {
        struct TbSprite *p_spr;
        short scr_x, scr_y;
        ushort advance;

        advance = ((p_box->Timer - 48) * box_h) / 24;
        vec_colour = p_box->Colour1;
        dword_1DC5FC = p_box->X + box_w;
        dword_1DC600 = p_box->Y + advance;
        dword_1DC624 = p_box->X + box_w;
        dword_1DC628 = dword_1DC600 - 15;
        if (dword_1DC628 < p_box->Y)
            dword_1DC628 = p_box->Y;
        draw_trig_purple_list(dword_1DC5FC, dword_1DC600, dword_1DC624, dword_1DC628);
        draw_line_purple_list(dword_1DC5FC, p_box->Y, dword_1DC624, dword_1DC600, p_box->Colour1);

        dword_1DC5FC = p_box->X;
        dword_1DC600 = p_box->Y + box_h - advance;
        dword_1DC624 = p_box->X;
        dword_1DC628 = dword_1DC600 + 15;
        if (dword_1DC628 > p_box->Y + box_h)
            dword_1DC628 = p_box->Y + box_h;
        draw_trig_purple_list(dword_1DC5FC, dword_1DC600, dword_1DC624, dword_1DC628);
        draw_line_purple_list(dword_1DC5FC, dword_1DC600, dword_1DC624, p_box->Y + box_h, p_box->Colour1);

        p_spr = &fe_mouseptr_sprites[spr1];
        scr_x = p_box->X - 1;
        scr_y = p_box->Y + box_h - advance - 1;
        draw_sprite_purple_list(scr_x, scr_y, p_spr);
        scr_x = p_box->X + p_box->Width - 2;
        scr_y = p_box->Y + advance - 1;
        draw_sprite_purple_list(scr_x, scr_y, p_spr);

        draw_line_purple_list(p_box->X, p_box->Y,
            p_box->X + box_w, p_box->Y, p_box->Colour1);
        draw_line_purple_list(p_box->X, p_box->Y + box_h,
            p_box->X + box_w, p_box->Y + box_h, p_box->Colour1);

        p_box->Timer += p_box->DrawSpeed;
        return 2;
    }

    if ((p_box->Flags & GBxFlg_RadioBtn) == 0)
    {
        if ((p_box->Flags & GBxFlg_Unkn0080) != 0) {
            p_box->TextFadePos = -5;
            p_box->Flags &= ~GBxFlg_Unkn0080;
        }
    }

    if (((p_box->Flags & GBxFlg_RadioBtn) != 0) && ((p_box->Flags & GBxFlg_Unkn0080) != 0))
    {
        if (p_box->Lines == 0) {
            p_box->Lines = my_count_lines(p_box->Text);
        }
        p_box->ScrollBarSize = p_box->ScrollWindowHeight * lines_visible / p_box->Lines;
        if ((p_box->ScrollBarSize < p_box->ScrollWindowHeight) || ((p_box->Flags & GBxFlg_IsMouseOver) != 0))
        {
            if (p_box->ScrollBarSize >= p_box->ScrollWindowHeight)
                p_box->ScrollBarSize = p_box->ScrollWindowHeight;
            p_box->ScrollBarPos = 0;
        }
        else
        {
            p_box->Flags &= ~GBxFlg_RadioBtn;
        }
        p_box->field_38 = 0;
        p_box->Flags &= ~(GBxFlg_IsPushed|GBxFlg_Unkn0080);
        if (p_box->Timer != 255) {
            p_box->TextFadePos = -5;
        }
    }

    if (((p_box->Flags & GBxFlg_RadioBtn) == 0) &&
      (((p_box->Flags & GBxFlg_BkCopied) != 0) || ((p_box->Flags & GBxFlg_NoBkCopy) != 0)))
    {
        // If childern drawing has also finished, it is safe to cap the scrollbar hole
        if (((p_box->Flags & GBxFlg_TextCopied) != 0) || text_remains_dynamic)
        {
            if ((p_box->Width > 13) && (p_box->ScrollWindowHeight > 0) && (p_box->ScrollWindowOffset >= 0))
            {
                lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
                draw_box_purple_list(p_box->X + p_box->Width - 13,
                  p_box->Y + p_box->ScrollWindowOffset + 4,
                  8, p_box->ScrollWindowHeight + 2,
                  p_box->BGColour);
            }
        }
        flashy_draw_purple_text_box_text(p_box);
        flashy_draw_purple_text_box_children(p_box);
        return 3;
    }

    struct ScreenBox scroll_area_abv_box;	// empty area above handle
    struct ScreenBox scroll_handle_box;		// draggable handle box
    struct ScreenBox scroll_area_blw_box;	// empty area below handle
    struct ScreenBox scroll_arrow_up_box;	// up arrow at bottom
    struct ScreenBox scroll_arrow_dn_box;	// down arrow at bottom

    init_screen_box(&scroll_area_abv_box, p_box->X + p_box->Width - 12,
      p_box->Y + p_box->ScrollWindowOffset + 5,
      6, p_box->ScrollBarPos, p_box->DrawSpeed);

    init_screen_box(&scroll_handle_box, p_box->X + p_box->Width - 12,
      p_box->Y + p_box->ScrollWindowOffset + 5 + p_box->ScrollBarPos,
      6, p_box->ScrollBarSize, p_box->DrawSpeed);

    init_screen_box(&scroll_area_blw_box, p_box->X + p_box->Width - 12,
      p_box->Y + p_box->ScrollWindowOffset + 5 + p_box->ScrollBarPos + p_box->ScrollBarSize,
      6, p_box->ScrollWindowHeight + 1 - (p_box->ScrollBarPos + p_box->ScrollBarSize), p_box->DrawSpeed);

    init_screen_box(&scroll_arrow_up_box, p_box->X + p_box->Width - 13,
      p_box->Y + p_box->ScrollWindowHeight + p_box->ScrollWindowOffset + 9,
      8, 9, p_box->DrawSpeed);

    init_screen_box(&scroll_arrow_dn_box, p_box->X + p_box->Width - 13,
      p_box->Y + p_box->ScrollWindowHeight + p_box->ScrollWindowOffset + 18,
      8, 9, p_box->DrawSpeed);

    if (lbDisplay.MLeftButton && ((p_box->Flags & GBxFlg_IsPushed) != 0))
    {
        p_box->ScrollBarPos = mouse_move_position_vertical_scrollbar_over_text_box(p_box);
        p_box->field_38 = p_box->Lines * p_box->ScrollBarPos / p_box->ScrollWindowHeight;
    }

    if (((p_box->Flags & GBxFlg_RadioBtn) != 0) && (p_box->ScrollBarPos >= 0))
    {
        // Clicking the scroll bar area above the handle
        if (mouse_move_over_box(&scroll_area_abv_box))
        {
            if (lbDisplay.LeftButton)
            {
                int delta;

                delta = p_box->ScrollBarPos - (lines_visible - 1) * p_box->ScrollWindowHeight / p_box->Lines;
                if (delta < 0)
                    delta = 0;
                else if (delta + p_box->ScrollBarSize > p_box->ScrollWindowHeight)
                    delta = p_box->ScrollWindowHeight - p_box->ScrollBarSize;
                p_box->ScrollBarPos = delta;
                p_box->field_38 = p_box->Lines * p_box->ScrollBarPos / p_box->ScrollWindowHeight;
            }
        }
        // Clicking the scroll bar area below the handle
        else if (mouse_move_over_box(&scroll_area_blw_box))
        {
            if (lbDisplay.LeftButton)
            {
                int delta;

                delta = p_box->ScrollBarPos + (lines_visible - 1) * p_box->ScrollWindowHeight / p_box->Lines;
                if (delta < 0)
                    delta = 0;
                else if (delta + p_box->ScrollBarSize > p_box->ScrollWindowHeight)
                    delta = p_box->ScrollWindowHeight - p_box->ScrollBarSize;
                p_box->ScrollBarPos = delta;
                p_box->field_38 = p_box->Lines * p_box->ScrollBarPos / p_box->ScrollWindowHeight;
            }
        }

        // Input draging the scroll bar handle
        if (((p_box->Flags & GBxFlg_IsPushed) != 0) || mouse_move_over_box(&scroll_handle_box))
        {
            if (lbDisplay.LeftButton)
            {
                int ms_y;
                lbDisplay.LeftButton = 0;
                p_box->Flags |= GBxFlg_IsPushed;
                ms_y = lbDisplay.ScreenMode == 1 ? 2 * lbDisplay.MouseY : lbDisplay.MouseY;
                p_box->GrabPos = ms_y - p_box->ScrollBarPos;
                play_sample_using_heap(0, 125, 127, 64, 100, 0, 1);
            }
        }

        // Using keyboard to scroll
        if (mouse_move_over_box(p_box)
          && (lbKeyOn[KC_UP] || lbKeyOn[KC_DOWN] || lbKeyOn[KC_PGUP] || lbKeyOn[KC_PGDOWN])
          && ((p_box->Flags & GBxFlg_IsPushed) == 0))
        {
            int delta;

            delta = 0;
            if (lbKeyOn[KC_DOWN])
                delta = p_box->ScrollWindowHeight / p_box->Lines;
            else if (lbKeyOn[KC_UP])
                delta = - p_box->ScrollWindowHeight / p_box->Lines;
            p_box->ScrollBarPos += delta;

            delta = 0;
            if (lbKeyOn[KC_PGDOWN])
                delta = p_box->ScrollWindowHeight * (lines_visible - 1) / p_box->Lines;
            else if (lbKeyOn[KC_PGUP])
                delta = - (lines_visible - 1) * p_box->ScrollWindowHeight / p_box->Lines;
            p_box->ScrollBarPos += delta;

            delta = p_box->ScrollBarPos;
            if (delta < 0)
                delta = 0;
            else if (delta + p_box->ScrollBarSize > p_box->ScrollWindowHeight)
                delta = p_box->ScrollWindowHeight - p_box->ScrollBarSize;
            p_box->ScrollBarPos = delta;

            p_box->field_38 = p_box->ScrollBarPos * p_box->Lines / p_box->ScrollWindowHeight;
        }

        // Input from up arrow in bottom part of the scroll bar
        if (mouse_move_over_box(&scroll_arrow_up_box))
        {
            int delta;

            if (lbDisplay.MLeftButton || joy.Buttons[0])
            {
                if (lbDisplay.LeftButton)
                    play_sample_using_heap(0, 125, 127, 64, 100, 0, 1);
                lbDisplay.LeftButton = 0;
                p_box->Flags |= GBxFlg_IsRPushed;
                scroll_arrow_up_box.Flags |= GBxFlg_IsRPushed;

                p_box->ScrollBarPos -= p_box->ScrollWindowHeight / p_box->Lines;
                delta = p_box->ScrollBarPos;
                if (delta < 0)
                    delta = 0;
                else if (delta + p_box->ScrollBarSize > p_box->ScrollWindowHeight)
                    delta = p_box->ScrollWindowHeight - p_box->ScrollBarSize;
                p_box->ScrollBarPos = delta;

                p_box->field_38 = p_box->Lines * p_box->ScrollBarPos / p_box->ScrollWindowHeight;
            }
        }

        // Input from down arrow in bottom part of the scroll bar
        if (mouse_move_over_box(&scroll_arrow_dn_box))
        {
            int delta;

            if (lbDisplay.MLeftButton || joy.Buttons[0])
            {
                if (lbDisplay.LeftButton)
                    play_sample_using_heap(0, 125, 127, 64, 100, 0, 1);
                lbDisplay.LeftButton = 0;
                p_box->Flags |= GBxFlg_IsRPushed;
                scroll_arrow_dn_box.Flags |= GBxFlg_IsRPushed;

                p_box->ScrollBarPos += p_box->ScrollWindowHeight / p_box->Lines;
                delta = p_box->ScrollBarPos;
                if (delta < 0)
                    delta = 0;
                else if (delta + p_box->ScrollBarSize > p_box->ScrollWindowHeight)
                    delta = p_box->ScrollWindowHeight - p_box->ScrollBarSize;
                p_box->ScrollBarPos = delta;

                p_box->field_38 = p_box->Lines * p_box->ScrollBarPos / p_box->ScrollWindowHeight;
            }
        }
    }

    if (!lbDisplay.MLeftButton && !joy.Buttons[0])
        p_box->Flags &= ~(GBxFlg_IsRPushed|GBxFlg_IsPushed);

    // Draw the static background
    if ((p_box->Flags & GBxFlg_BkCopied) == 0)
    {
        short w, h;
        lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
        // main area, left of scrollbar
        w = max(p_box->Width - 16, 0);
        draw_box_purple_list(p_box->X - 3, p_box->Y - 3,
          w + 6, p_box->Height + 6, p_box->BGColour);
        // strip on right of scrollbar
        if (p_box->Width >= 5)
            draw_box_purple_list(p_box->X + p_box->Width - 5,  p_box->Y - 3,
              8, p_box->Height + 6, p_box->BGColour);
        if (p_box->Width > 13)
        {
            // above the scrollbar
            draw_box_purple_list(p_box->X + p_box->Width - 13, p_box->Y - 3,
              8, p_box->ScrollWindowOffset + 7, p_box->BGColour);
            // below the scrollbar
            h = max(p_box->ScrollWindowHeight + 3 + p_box->ScrollWindowOffset, 1);
            if (p_box->Height > h)
                draw_box_purple_list(p_box->X + p_box->Width - 13,
                  p_box->Y + h + 3,
                  8, p_box->Height - h,
                  p_box->BGColour);
        }
        // box outline
        lbDisplay.DrawFlags = Lb_SPRITE_OUTLINE;
        draw_box_purple_list(p_box->X, p_box->Y, p_box->Width, p_box->Height, p_box->Colour1);
        lbDisplay.DrawFlags = 0;

        if ((p_box->Flags & GBxFlg_NoBkCopy) == 0)
        {
            copy_box_purple_list(p_box->X - 3, p_box->Y - 3,
              p_box->Width + 6, p_box->Height + 6);
            p_box->Flags |= GBxFlg_BkCopied;
        }
    }

    if (((p_box->Flags & GBxFlg_RadioBtn) != 0) && (p_box->ScrollBarPos >= 0))
    {
        struct TbSprite *p_spr;

        // Draw the scroll bar handle
        draw_box_purple_list(scroll_handle_box.X, scroll_handle_box.Y,
          scroll_handle_box.Width, scroll_handle_box.Height, 174);

        // Draw scroll arrows
        lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
        if ((scroll_arrow_up_box.Flags & GBxFlg_IsRPushed) != 0)
            lbDisplay.DrawFlags = 0;
        if (p_box->Text != NULL)
            lbDisplay.DrawFlags |= 0x8000;

        p_spr = &fe_mouseptr_sprites[9];
        draw_sprite_purple_list(scroll_arrow_up_box.X, scroll_arrow_up_box.Y, p_spr);

        lbDisplay.DrawFlags = 0;

        if (mouse_move_over_box(&scroll_arrow_up_box))
        {
            p_spr = &fe_mouseptr_sprites[13];
            draw_sprite_purple_list(scroll_arrow_up_box.X, scroll_arrow_up_box.Y, p_spr);
        }

        lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
        if ((scroll_arrow_dn_box.Flags & GBxFlg_IsRPushed) != 0)
            lbDisplay.DrawFlags = 0;
        if (p_box->Text != NULL)
            lbDisplay.DrawFlags |= 0x8000;

        p_spr = &fe_mouseptr_sprites[10];
        draw_sprite_purple_list(scroll_arrow_dn_box.X, scroll_arrow_dn_box.Y, p_spr);

        lbDisplay.DrawFlags = 0;

        if (mouse_move_over_box(&scroll_arrow_dn_box))
        {
            p_spr = &fe_mouseptr_sprites[14];
            draw_sprite_purple_list(scroll_arrow_dn_box.X, scroll_arrow_dn_box.Y, p_spr);
        }
    }

    flashy_draw_purple_text_box_text(p_box);
    flashy_draw_purple_text_box_children(p_box);
    return 3;
}

ubyte flashy_draw_purple_button(struct ScreenButton *button)
{
    ubyte ret;
    asm volatile ("call ASM_flashy_draw_purple_button\n"
        : "=r" (ret) : "a" (button));
    return ret;
}

ubyte button_text(struct ScreenButton *p_btn)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_button_text\n"
        : "=r" (ret) : "a" (p_btn));
    return ret;
#endif
    short text_x, text_y;
    short text_w, text_h;

    lbFontPtr = p_btn->Font;
    text_w = my_string_width(p_btn->Text);
    text_h = font_height('A');
    text_x = p_btn->X + ((p_btn->Width - text_w) >> 1) + 1;
    text_y = p_btn->Y + ((p_btn->Height - text_h) >> 1);
    my_set_text_window(text_x, text_y, lbDisplay.GraphicsScreenWidth, text_y + text_h);
    if (p_btn->Flags & 0x80) {
        p_btn->TextFadePos = -3;
        p_btn->Flags &= ~0x80;
    }
    return flashy_draw_text(0, 0, p_btn->Text, p_btn->TextSpeed, p_btn->TextTopLine,
      &p_btn->TextFadePos, 1);
}

void draw_triangle_purple_list(int x1, int y1, int x2, int y2, int x3, int y3, TbPixel colour)
{
    asm volatile (
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_draw_triangle_purple_list\n"
        : : "a" (x1), "d" (y1), "b" (x2), "c" (y2), "g" (x3), "g" (y3), "g" (colour));
}

void draw_flic_purple_list(void (*fn)())
{
#if 0
    asm volatile ("call ASM_draw_flic_purple_list\n"
        : : "a" (fn));
    return;
#endif
    struct PurpleDrawItem *pditem;

    pditem = &purple_draw_list[purple_draw_index];
    purple_draw_index++;

    pditem->U.Flic.Function = fn;
    pditem->U.Flic.Colour = lbDisplay.DrawColour;
    pditem->Flags = lbDisplay.DrawFlags;
    pditem->Type = PuDT_FLIC;
}

void draw_noise_box_purple_list(int x, int y, ulong width, ulong height)
{
    draw_box_purple_list(x, y, width, height, 0);
    purple_draw_list[purple_draw_index - 1].Type = PuDT_NOISEBOX;
}

/******************************************************************************/
