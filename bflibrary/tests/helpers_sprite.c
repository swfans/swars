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
#include "bfpalette.h"
#include "bfutility.h"
#include "bftstlog.h"

static struct TbAnyWindow sprites1_list[] = {
    {0, 0, 0, 0, NULL,},
};

void generate_example_sprites_from_png(int sprfile_no, const ubyte *pal,
  ubyte *p_dat, TbSprite *p_tab)
{
    const char *img_fname;
    TbSprite *p_spr;
    struct TbAnyWindow *p_wnd;
    int tot_sprites, i;

    switch (sprfile_no)
    {
    case 1:
        img_fname = "referenc/spr_devpac-mans16.png";
        tot_sprites = sizeof(sprites1_list)/sizeof(sprites1_list[0]);
        p_wnd = &sprites1_list[i];
        break;
    default:
        img_fname = NULL;
        tot_sprites = 0;
        p_wnd = NULL;
        break;
    }
    // Load the sprite image
 
    // Convert each area into a sprite
    p_spr = p_tab;
    for (i = 0; i < tot_sprites; i++)
    {
        int len;

        LbScreenSetGraphicsWindow(p_wnd->x, p_wnd->y, p_wnd->width, p_wnd->height);
        len = LbScreenSpriteEncode(p_spr, p_dat);
        p_dat += len;
        p_spr++;
        p_wnd++;
    }

}

/******************************************************************************/
