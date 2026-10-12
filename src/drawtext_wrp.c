/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file drawtext_wrp.c
 *     Drawing simple text within the 3D world, extended wrappers.
 * @par Purpose:
 *     Implement functions for drawing text, with its position transformed from
 *     in-mission map coordinated. Draws simple text, to be used for debug
 *     or in-game editors; for proper playtime text, use bitmap fonts instead.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 Aug 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "drawtext_wrp.h"

#include "bfbox.h"
#include "bfkeybd.h"
#include "bfline.h"
#include "bfscreen.h"
#include "bftime.h"
#include "bfwindows.h"

#include "display.h"
#include "drawtext.h"
#include "drawshape.h"
#include "engincolour.h"
#include "engintrns.h"

#include "bigmap.h"
#include "keyboard.h"
#include "swlog.h"
/******************************************************************************/

void draw_mapcoord_line_at_ground(int cor1_x, int cor1_z,
  int cor2_x, int cor2_z, TbPixel colour)
{
    int cor1_y_m8, cor2_y_m8;

    cor1_y_m8 = PRCCOORD_TO_YCOORD(alt_at_point(cor1_x, cor1_z));
    cor2_y_m8 = PRCCOORD_TO_YCOORD(alt_at_point(cor2_x, cor2_z));

    draw_mapcoord_line_col(cor1_x, cor1_y_m8, cor1_z,
      cor2_x, cor2_y_m8, cor2_z, colour);
}

void draw_mapcoord_simple_text_at_ground(int cor_x, int cor_z, const char *text)
{
    int cor_y_m8;

    cor_y_m8 = PRCCOORD_TO_YCOORD(alt_at_point(cor_x, cor_z));
    draw_mapcoord_simple_text(cor_x, cor_y_m8, cor_z,
      text, colour_lookup[ColLU_RED]);
}

void draw_mapcoord_simple_text_number_at_ground(int cor_x, int cor_z, int num)
{
    char locstr[52];
    int cor_y_m8;

    cor_y_m8 = PRCCOORD_TO_YCOORD(alt_at_point(cor_x, cor_z));
    sprintf(locstr, "%d", num);
    draw_mapcoord_simple_text(cor_x, cor_y_m8, cor_z,
      locstr, colour_lookup[ColLU_RED]);
}

void draw_purple_rect(int x, int y, int w, int h, ubyte active)
{
    TbPixel col1, col2;

    if (active) {
        col1 = 0x0E;
        col2 = 0x0C;
    } else {
        col1 = 0x10; // colour_lookup[ColLU_BLUE]
        col2 = 0x0E; // colour_lookup[ColLU_CYAN]
    }
    draw_bevel_rect(x, y, w, h, col1, col2);
}

static short count_chars_in_text(const char *text, char cc)
{
    const char *s;
    short i;

    s = text;
    for (i = 0; s[i] != 0; s[i] == cc ? i++ : *s++);
    return i;
}

void draw_message_box(const char *text)
{
    int x, y, w, h;
    short ln_count;
    short ln_height;

    ln_height = simple_text_char_height('A');
    ln_count = count_chars_in_text(text, '\10') + 1;
    w = simple_text_width(text) + 2 * ln_height;
    if (w > lbDisplay.GraphicsWindowWidth - 2)
        w = lbDisplay.GraphicsWindowWidth - 2;
    h = (2 + ln_count) * ln_height;
    if (h > lbDisplay.GraphicsWindowHeight - 2)
        h = lbDisplay.GraphicsWindowHeight - 2;
    x = (lbDisplay.GraphicsWindowWidth - w) / 2;
    y = (lbDisplay.GraphicsWindowHeight - h) / 2;

    draw_purple_rect(x, y, w, h, 0);
    draw_simple_text(x + ln_height, y + ln_height, text, 0xC);
}

void show_message(const char *text)
{
    draw_message_box(text);
    screen_wait_seconds_or_until_continue_key(5);
}

/******************************************************************************/
