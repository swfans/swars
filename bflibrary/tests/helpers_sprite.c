/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file helpers_sprite.c
 *     Implementation of sample sprite data creation for tests.
 * @par Purpose:
 *     Unknown.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     25 Nov 2024 - 21 Mar 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <string.h>
#include "helpers_sprite.h"
#include "bfanywnd.h"
#include "bfpalette.h"
#include "bfutility.h"
#include "bftstlog.h"

static struct TbAnyWindow sprites1_list[] = {
    {0, 0, 0, 0, NULL,},
};

void get_example_sprites_file_name(int sprfile_no, char *fname)
{
    switch (sprfile_no)
    {
    case 1:
        strcpy(fname, "referenc/spr_devpac-mans16.png");
        break;
    default:
        fname[0] = '\0';
        break;
    }
}

TbScreenMode get_example_sprites_screen_mode(int sprfile_no)
{
    char locstr[64];
    ulong img_width, img_height, img_bpp;
    ulong flags;

    switch (sprfile_no)
    {
    case 1:
        img_width = 320;
        img_height = 1024;
        img_bpp = 8;
        break;
    default:
        img_width = 4;
        img_height = 4;
        img_bpp = 8;
        break;
    }

    sprintf(locstr, "%lux%lux%lu", img_width, img_height, img_bpp);
    if (img_bpp <= 8)
        flags = Lb_VF_PALETTE;
    else
        flags = Lb_VF_RGBCOLOUR;
    return LbRegisterVideoMode(locstr, img_width, img_height, img_bpp, flags);
}

int get_example_sprites_total_count(int sprfile_no)
{
    int tot_sprites;

    switch (sprfile_no)
    {
    case 1:
        tot_sprites = sizeof(sprites1_list)/sizeof(sprites1_list[0]);
        break;
    default:
        tot_sprites = 0;
        break;
    }

    return tot_sprites;
}

int generate_example_sprites_from_screen(int sprfile_no, const ubyte *pal,
  ubyte *p_dat, TbSprite *p_tab)
{
    TbSprite *p_spr;
    ubyte *p_sprdt;
    struct TbAnyWindow *p_wnd;
    int tot_sprites, i;

    switch (sprfile_no)
    {
    case 1:
        p_wnd = &sprites1_list[0];
        break;
    default:
        p_wnd = NULL;
        break;
    }
    tot_sprites = get_example_sprites_total_count(sprfile_no);

    // Convert each area into a sprite
    p_spr = p_tab;
    p_sprdt = p_dat;
    for (i = 0; i < tot_sprites; i++)
    {
        int len;

        LbScreenSetGraphicsWindow(p_wnd->x, p_wnd->y, p_wnd->width, p_wnd->height);
        len = LbScreenSpriteEncode(p_spr, p_sprdt);
        p_sprdt += len;
        p_spr++;
        p_wnd++;
    }

    return p_sprdt - p_dat;
}

void palette_remap_to_screen(TbPixel *p_remap, const TbPixel *p_altpal)
{
    int i;
    for (i = 0; i < PALETTE_8b_COLORS; i++)
    {
        ubyte r, g, b;
        r = p_altpal[3*i + 0];
        g = p_altpal[3*i + 1];
        b = p_altpal[3*i + 2];
        p_remap[i] = LbPaletteFindColour(lbDisplay.Palette, r, g, b);
    }
}

/******************************************************************************/
