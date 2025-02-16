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

ubyte flashy_draw_purple_text_box(struct ScreenTextBox *p_box)
{
#if 1
    ubyte ret;
    asm volatile ("call ASM_flashy_draw_purple_text_box\n"
        : "=r" (ret) : "a" (p_box));
    return ret;
#endif
}

ubyte flashy_draw_purple_button(struct ScreenButton *button)
{
    ubyte ret;
    asm volatile ("call ASM_flashy_draw_purple_button\n"
        : "=r" (ret) : "a" (button));
    return ret;
}

ubyte button_text(struct ScreenButton *button)
{
    ubyte ret;
    asm volatile ("call ASM_button_text\n"
        : "=r" (ret) : "a" (button));
    return ret;
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
