/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file guigraph.c
 *     GUI graphs drawing for menu system.
 * @par Purpose:
 *     Implement functions for drawing graphs in front-end desktop GUI.
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
#include "guigraph.h"

#include <string.h>
#include "bftext.h"
#include "bfmemut.h"
#include "bfsprite.h"
#include "bfscreen.h"

#include "guiboxes.h"
#include "guitext.h"
#include "display.h"
#include "game.h"
#include "purpldrw.h"
#include "purpldrwlst.h"
#include "swlog.h"
/******************************************************************************/
extern struct TbSprite *fe_icons_sprites;

void draw_chartxy_axis_y_values(int x, int y, int height, int ax_min, int ax_max, int tot_values)
{
    char locstr[16];
    int i;
    int ax_val;
    int cy;

    ax_val = ax_max;
    cy = y;
    for (i = 0; i < tot_values; i++)
    {
        int twidth;
        const char *text;
        ulong spr_id;
        struct TbSprite *spr;

        lbDisplay.DrawFlags = 0;
        if (i != tot_values/2 && i != 0)
            spr_id = 146;
        else
            spr_id = 145;
        spr = &fe_icons_sprites[spr_id];
        sprintf(locstr, "%d", ax_val);
        twidth = LbTextStringWidth(locstr) + spr->SWidth;
        text = loctext_to_gtext(locstr);
        draw_text_purple_list2(x - twidth - 1, cy, text, 0);
        cy += height / tot_values;
        ax_val -= (ax_max - ax_min) / tot_values;
    }
}

void draw_chartxy_axis_x_values(int x, int y, int width, int ax_min, int ax_max, int tot_values)
{
    char locstr[16];
    int i;
    int ax_val;
    int cx;

    ax_val = ax_min;
    cx = x + 32;
    for (i = 0; i < tot_values; i++)
    {
        int twidth;
        const char *text;
        ulong spr_id;
        struct TbSprite *spr;
        int final_x;

        lbDisplay.DrawFlags = 0;
        if (i != tot_values / 2 - 1 && i != tot_values - 1)
            spr_id = 146;
        else
            spr_id = 147;
        spr = &fe_icons_sprites[spr_id];
        sprintf(locstr, "%d", ax_val);
        twidth = LbTextStringWidth(locstr);
        text = loctext_to_gtext(locstr);
        if (ax_val == 1)
            final_x = cx - (twidth - 2) + 3;
        else
            final_x = cx - (twidth - 2) + 1;
        draw_text_purple_list2(final_x, y + 4 + spr->SHeight, text, 0);
        cx += width / tot_values;
        ax_val += (ax_max - ax_min) / tot_values;
    }
}

void draw_chartxy_axis_y_grid(int x, int y, int width, int height, int tot_values)
{
    struct TbSprite *spr;
    int i;
    int cy;

    {
        lbDisplay.DrawFlags = 0;
        spr = &fe_icons_sprites[145];
        draw_sprite_purple_list(x - spr->SWidth + 1, y + height, spr);
    }

    cy = y;
    for (i = 0; i < tot_values; i++)
    {
        ulong spr_id;

        lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
        draw_line_purple_list(x + 1, cy, x + width, cy, height + 7);
        lbDisplay.DrawFlags = 0;
        if (i != tot_values/2 && i != 0)
            spr_id = 146;
        else
            spr_id = 145;
        spr = &fe_icons_sprites[spr_id];
        draw_sprite_purple_list(x - spr->SWidth - 1, cy, spr);
        cy += height / tot_values;
    }
}

void draw_chartxy_axis_x_main(int x, int y, int width)
{
    draw_box_purple_list(x, y, width + 1, 2, 247);
}

void draw_chartxy_axis_y_main(int x, int y, int height)
{
    draw_box_purple_list(x - 1, y, 2, height + 1, 247);
}

void draw_chartxy_axis_x_grid(int x, int y, int width, int height, int tot_values)
{
    struct TbSprite *spr;
    int i;
    int cx;

    {
        lbDisplay.DrawFlags = 0;
        spr = &fe_icons_sprites[147];
        draw_sprite_purple_list(64 - spr->SWidth + 1, y + height, spr);
    }

    cx = x;
    for (i = 0; i < tot_values; i++)
    {
        ulong spr_id;

        cx += width / tot_values;
        lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
        draw_line_purple_list(cx, y, cx, y + height - 1, height + 7);
        lbDisplay.DrawFlags = 0;
        if (i != tot_values / 2 - 1 && i != tot_values - 1)
            spr_id = 146;
        else
            spr_id = 147;
        spr = &fe_icons_sprites[spr_id];
        draw_sprite_purple_list(cx - spr->SWidth + 1, y + height + 2, spr);
    }
}

void draw_line_purple_thick(int x1, int y1, int x2, int y2, ubyte colour, ubyte bkcolor)
{
    draw_line_purple_list(x1, y1, x2, y2, 87);
    if (y1 - y2 <= x2 - x1)
    {
        draw_line_purple_list(x1, y1 + 1, x2, y2 + 1, colour);
        draw_line_purple_list(x1, y1 - 1, x2, y2 - 1, colour);
        draw_line_purple_list(x1, y1 + 2, x2, y2 + 2, bkcolor);
        draw_line_purple_list(x1, y1 + 3, x2, y2 + 3, bkcolor);
        draw_line_purple_list(x1, y1 + 4, x2, y2 + 4, bkcolor);
    }
    else
    {
        draw_line_purple_list(x1 + 1, y1, x2 + 1, y2, colour);
        draw_line_purple_list(x1 - 1, y1, x2 - 1, y2, colour);
        draw_line_purple_list(x1 + 2, y1, x2 + 2, y2, bkcolor);
        draw_line_purple_list(x1 + 3, y1, x2 + 3, y2, bkcolor);
        draw_line_purple_list(x1 + 4, y1, x2 + 4, y2, bkcolor);
    }
}

void draw_chartxy_curve(int x, int y, int width, int height, ushort *y_vals, int n_y_vals, int y_scale, ushort y_trend_delta, int tot_values)
{
    int cday, progress, prev_progress, progress_scale;
    int cx, cy, mcy;
    int next_cx, next_cy, next_mcy;
    int delta_x, delta_y, delta_u;
    int remain_day;

    // Draw existing points curve
    delta_x = width / tot_values;
    delta_y = height / tot_values;
    delta_u = delta_x;
    progress_scale = y_scale / tot_values;
    cday = 0;
    progress = y_vals[cday];
    cx = x;
    // To increase accuracy, we keep y value multiplied by progress_scale
    mcy = (y + height) * progress_scale - (delta_y * progress);
    for (cday++; cday < n_y_vals; cday++)
    {
        progress = y_vals[cday];
        prev_progress = y_vals[cday-1];
        next_mcy = mcy - delta_y * (progress - prev_progress);
        next_cx = cx + delta_x;
        cy = mcy / progress_scale;
        next_cy = next_mcy / progress_scale;
        draw_line_purple_thick(cx, cy, next_cx, next_cy, 87, 0);
        cx = next_cx;
        mcy = next_mcy;
    }
    // Draw trend curve
    lbDisplay.DrawFlags = Lb_SPRITE_TRANSPAR4;
    remain_day = 11 - cday;
    {
        progress = y_trend_delta;
        next_mcy = mcy - (delta_y * progress * remain_day);
        cy = mcy / progress_scale;
        next_cy = next_mcy / progress_scale;
        delta_u = delta_x * remain_day;
        if (next_cy >= y) {
            // Everything fits on the chart
            next_cx = cx + delta_u;
        } else {
            // Cap cy, and adjust cx accordingly
            next_cx = cx + cy * delta_u / (cy - next_cy);
            if (next_cx > width)
                next_cx = width;
            next_cy = y;
        }
        draw_line_purple_thick(cx, cy, next_cx, next_cy, 87, 0);
    }
    lbDisplay.DrawFlags = 0;
}

/******************************************************************************/
