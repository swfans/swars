/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file guiboxes.c
 *     GUI boxes for menu system.
 * @par Purpose:
 *     Implement functions for handling various boxes for game GUI.
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
#include "guiboxes.h"

#include "bfscreen.h"
#include "swlog.h"
/******************************************************************************/

TbBool in_box(short x, short y, short box_x, short box_y, short box_w, short box_h)
{
    return x > box_x && x < box_x + box_w
        && y > box_y && y < box_y + box_h;
}

TbBool in_box_coords(short x, short y, short box_x1, short box_y1, short box_x2, short box_y2)
{
    return x > box_x1 && x < box_x2
        && y > box_y1 && y < box_y2;
}

TbBool over_box(short x, short y, short box_x, short box_y, short box_w, short box_h)
{
    return x >= box_x && x <= box_x + box_w
        && y >= box_y && y <= box_y + box_h;
}

TbBool over_box_coords(short x, short y, short box_x1, short box_y1, short box_x2, short box_y2)
{
    return x >= box_x1 && x <= box_x2
        && y >= box_y1 && y <= box_y2;
}

TbBool mouse_move_over_box_coords(short box_x1, short box_y1, short box_x2, short box_y2)
{
    short ms_x, ms_y;

    ms_x = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MMouseX : lbDisplay.MMouseX;
    ms_y = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MMouseY : lbDisplay.MMouseY;
    return over_box_coords(ms_x, ms_y, box_x1, box_y1, box_x2, box_y2);
}

TbBool mouse_down_over_box_coords(short box_x1, short box_y1, short box_x2, short box_y2)
{
    short ms_x, ms_y;

    ms_x = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MouseX : lbDisplay.MouseX;
    ms_y = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MouseY : lbDisplay.MouseY;
    return over_box_coords(ms_x, ms_y, box_x1, box_y1, box_x2, box_y2);
}

// TODO switch order of arguments and to above function, remove when this is no longer used
TbBool mouse_move_over_rect(short box_x1, short box_x2, short box_y1, short box_y2)
{
    short ms_x, ms_y;

    ms_x = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MMouseX : lbDisplay.MMouseX;
    ms_y = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MMouseY : lbDisplay.MMouseY;
    return over_box_coords(ms_x, ms_y, box_x1, box_y1, box_x2, box_y2);
}

TbBool mouse_move_over_rect_adv(short x1, short y1, short width, short height, TbBool a5)
{
    short ms_x, ms_y;
    short dx, dy;

    ms_x = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MMouseX : lbDisplay.MMouseX;
    ms_y = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MMouseY : lbDisplay.MMouseY;

    if (a5)
    {
        if ((ms_y >= y1) && (ms_y < y1 + height))
        {
            dx = ms_x - x1;
            dy = ms_y - y1;
            if ((dx >= dy) && (dx - dy < width))
                return 1;
        }
    }
    else
    {
        if ((ms_y >= y1) && (ms_y < y1 + height))
        {
            dx = ms_x - x1;
            dy = ms_y - y1;
            if ((dx >= -dy) && (dx + dy < width))
                return 1;
        }
    }
    return 0;
}

short mouse_move_position_horizonal_over_bar_coords(short x, short w)
{
    short ms_x;

    ms_x = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MMouseX : lbDisplay.MMouseX;
    return (ms_x - x);
}

short mouse_down_position_horizonal_over_bar_coords(short x, short w)
{
    short ms_x;

    ms_x = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MouseX : lbDisplay.MouseX;
    return (ms_x - x);
}

TbBool is_over_box_base(short x, short y, struct ScreenBoxBase *box)
{
    return (x >= box->X) && (x <= box->X + box->Width)
        && (y >= box->Y) && (y <= box->Y + box->Height);
}

TbBool is_over_slant_box_base(short x, short y, struct ScreenBoxBase *box)
{
    if ((x >= box->X) && (x <= box->X + box->Width + box->Height)
        && (y >= box->Y) && (y <= box->Y + box->Height))
    {
        short dx, dy, hh, hw;
        hh = box->Height >> 1;
        hw = box->Height + box->Width;
        dx = x - box->X;
        dy = y - box->Y;
        return (dy + dx >= hh) && (dy + dx <= hw);
    }
    return false;
}

TbBool mouse_move_over_box_base(struct ScreenBoxBase *box)
{
    short ms_x, ms_y;

    ms_x = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MMouseX : lbDisplay.MMouseX;
    ms_y = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MMouseY : lbDisplay.MMouseY;
    return is_over_box_base(ms_x, ms_y, box);
}

TbBool mouse_down_over_box_base(struct ScreenBoxBase *box)
{
    short ms_x, ms_y;

    ms_x = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MouseX : lbDisplay.MouseX;
    ms_y = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MouseY : lbDisplay.MouseY;
    return is_over_box_base(ms_x, ms_y, box);
}

TbBool mouse_move_over_slant_box_base(struct ScreenBoxBase *box)
{
    short ms_x, ms_y;

    ms_x = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MMouseX : lbDisplay.MMouseX;
    ms_y = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MMouseY : lbDisplay.MMouseY;
    return is_over_slant_box_base(ms_x, ms_y, box);
}

short mouse_move_position_horizonal_over_box_base(struct ScreenBoxBase *box)
{
    return mouse_move_position_horizonal_over_bar_coords(box->X, box->Width);
}

short mouse_move_y_coord_over_box_base(struct ScreenBoxBase *box)
{
    short ms_y;

    ms_y = lbDisplay.GraphicsScreenHeight < 400 ? 2 * lbDisplay.MMouseY : lbDisplay.MMouseY;
    return ms_y - box->Y;
}

void init_screen_box(struct ScreenBox *box, ushort x, ushort y, ushort width, ushort height, int drawspeed)
{
    asm volatile (
      "push %5\n"
      "push %4\n"
      "call ASM_init_screen_box\n"
        : : "a" (box), "d" (x), "b" (y), "c" (width), "g" (height), "g" (drawspeed));
}

void init_screen_text_box(struct ScreenTextBox *box, ushort x, ushort y, ushort width, ushort height, int drawspeed, struct TbSprite *font, int textspeed)
{
    asm volatile (
      "push %7\n"
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_init_screen_text_box\n"
        : : "a" (box), "d" (x), "b" (y), "c" (width), "g" (height), "g" (drawspeed), "g" (font), "g" (textspeed));
}

void init_screen_button(struct ScreenButton *box, ushort x, ushort y, const char *text, int drawspeed, struct TbSprite *font, int textspeed, int flags)
{
    asm volatile (
      "push %7\n"
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_init_screen_button\n"
        : : "a" (box), "d" (x), "b" (y), "c" (text), "g" (drawspeed), "g" (font), "g" (textspeed), "g" (flags));
}

void init_screen_info_box(struct ScreenInfoBox *box, ushort x, ushort y, ushort width, const char *text1, const char *text2, int drawspeed, struct TbSprite *font1, struct TbSprite *font2, int textspeed)
{
    asm volatile (
      "push %9\n"
      "push %8\n"
      "push %7\n"
      "push %6\n"
      "push %5\n"
      "push %4\n"
      "call ASM_init_screen_info_box\n"
        : : "a" (box), "d" (x), "b" (y), "c" (width), "g" (text1), "g" (text2), "g" (drawspeed), "g" (font1), "g" (font2), "g" (textspeed));
}

/******************************************************************************/
