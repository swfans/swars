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

void draw_line_transformed_at_ground(int cor1_x, int cor1_z,
  int cor2_x, int cor2_z, TbPixel colour)
{
    int cor1_y_m8, cor2_y_m8;

    cor1_y_m8 = PRCCOORD_TO_YCOORD(alt_at_point(cor1_x, cor1_z));
    cor2_y_m8 = PRCCOORD_TO_YCOORD(alt_at_point(cor2_x, cor2_z));

    draw_mapcoord_line_col(cor1_x, cor1_y_m8, cor1_z,
      cor2_x, cor2_y_m8, cor2_z, colour);
}

void draw_text_transformed_at_ground(int cor_x, int cor_z, const char *text)
{
    int cor_y_m8;

    cor_y_m8 = PRCCOORD_TO_YCOORD(alt_at_point(cor_x, cor_z));
    draw_text_transformed_col(cor_x, cor_y_m8, cor_z,
      text, colour_lookup[ColLU_RED]);
}

void draw_number_transformed_at_ground(int cor_x, int cor_z, int num)
{
    char locstr[52];
    int cor_y_m8;

    cor_y_m8 = PRCCOORD_TO_YCOORD(alt_at_point(cor_x, cor_z));
    sprintf(locstr, "%d", num);
    draw_text_transformed_col(cor_x, cor_y_m8, cor_z,
      locstr, colour_lookup[ColLU_RED]);
}

void show_message(const char *text)
{
    int x, y, w, h;

    x = lbDisplay.GraphicsScreenWidth / 12;
    y = lbDisplay.GraphicsScreenHeight / 4;
    w = lbDisplay.GraphicsScreenWidth - 2 * x;
    h = lbDisplay.GraphicsScreenHeight / 8;

    lbDisplay.DrawFlags = 0;
    LbDrawBox(x, y, w, h, colour_lookup[4]);
    lbDisplay.DrawFlags = 0;
    draw_text(x * 3 / 2, y * 1 / 16, text, colour_lookup[1]);

    swap_wscreen();

    screen_wait_seconds_or_until_continue_key(5);
}

/******************************************************************************/
