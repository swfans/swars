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
    {  0,  0, 0, 0, NULL,}, // Torso walk blue
    {  2,  8,11,16, NULL,},
    { 18,  8,11,16, NULL,},
    { 34,  8,11,16, NULL,},
    { 50,  8,14,16, NULL,},
    { 66,  8,12,16, NULL,},
    { 82,  8,11,16, NULL,},
    { 98,  8,10,16, NULL,},
    {114,  8,13,16, NULL,},
    {129,  8,14,16, NULL,},
    {146,  8,12,16, NULL,},
    {162,  8,12,16, NULL,},
    {178,  8,13,16, NULL,},
    {193,  8,12,16, NULL,},
    {209,  8,12,16, NULL,},
    {226,  8,13,16, NULL,},
    {241,  8,13,16, NULL,},
    {256,  8,14,16, NULL,},
    {273,  8,12,16, NULL,},
    {290,  8,11,16, NULL,},
    {305,  8,12,16, NULL,},
    {  2, 32,12,16, NULL,},
    { 18, 32,12,16, NULL,},
    { 34, 32,10,16, NULL,},
    { 49, 32,13,16, NULL,},
    { 66, 32,12,16, NULL,},
    { 82, 32,12,16, NULL,},
    { 98, 32,11,16, NULL,},
    {113, 32,14,16, NULL,},
    {130, 32,13,16, NULL,},
    {146, 32,12,16, NULL,},
    {161, 32,12,16, NULL,},
    {178, 32,12,16, NULL,},
    {194, 32,11,16, NULL,}, // Torso walk red
    {210, 32,11,16, NULL,},
    {226, 32,11,16, NULL,},
    {242, 32,14,16, NULL,},
    {258, 32,12,16, NULL,},
    {274, 32,11,16, NULL,},
    {290, 32,10,16, NULL,},
    {306, 32,13,16, NULL,},
    {  1, 56,14,16, NULL,},
    { 18, 56,12,16, NULL,},
    { 34, 56,12,16, NULL,},
    { 50, 56,14,16, NULL,},
    { 65, 56,12,16, NULL,},
    { 81, 57,12,15, NULL,},
    { 99, 55,12,17, NULL,},
    {113, 57,13,15, NULL,},
    {129, 58,14,14, NULL,},
    {145, 57,12,15, NULL,},
    {162, 58,10,14, NULL,},
    {176, 58,12,14, NULL,},
    {194, 58,13,14, NULL,},
    {210, 58,12,14, NULL,},
    {227, 57, 9,15, NULL,},
    {240, 57,13,15, NULL,},
    {258, 57,11,15, NULL,},
    {275, 57,11,15, NULL,},
    {290, 55,11,17, NULL,},
    {304, 57,13,15, NULL,},
    {  3, 80,12,16, NULL,},
    { 18, 80,12,16, NULL,},
    { 33, 80,12,16, NULL,},
    { 50, 81,12,15, NULL,},
    { 65, 83,15,13, NULL,}, // Torso drowning blue
    { 81, 83,15,13, NULL,},
    { 97, 83,15,13, NULL,}, // Torso drowning red
    {113, 83,15,13, NULL,},
    {130, 77,12,19, NULL,}, // Burned body to ash
    {146, 77,12,19, NULL,},
    {162, 77,12,19, NULL,},
    {178, 77,12,19, NULL,},
    {192, 80,14,16, NULL,},
    {208, 84,14,12, NULL,},
    {223, 88,18, 8, NULL,},
    {242, 91,14, 5, NULL,},
    {257, 93,13, 3, NULL,},
    {274, 80,12,16, NULL,},
    {222, 88, 7, 8, NULL,}, // Small light
    {310, 91, 3, 5, NULL,}, // Flame
    {  5,104, 6,16, NULL,},
    { 21,110, 6,10, NULL,},
    { 36,108, 8,12, NULL,},
    { 52,108, 8,12, NULL,},
    { 68,105, 9,15, NULL,},
    { 82, 99,14,21, NULL,},
    { 99,116,10, 4, NULL,}, // Floor ring
    {114,113,13, 7, NULL,},
    {133,116, 8, 4, NULL,}, // Flag blue
    {149,115, 8, 5, NULL,},
    {165,116, 8, 4, NULL,}, // Flag red
    {181,115, 8, 5, NULL,},
    {193,107,15,13, NULL,}, // Golden wings artifact
    {210,107,13,13, NULL,}, // Medusa head
    {227,105,12,15, NULL,}, // Torso walk blue
    {245,114, 4, 6, NULL,}, // Hand up
    {262,112, 3, 8, NULL,},
    {278,110, 3,10, NULL,},
    {292,113, 4, 7, NULL,},
    {308,115, 4, 5, NULL,},
    {  6,137, 2, 7, NULL,},
    { 18,130,11,14, NULL,}, // Torso front no hands blue
    { 34,130,11,14, NULL,}, // Torso front no hands red
    { 49,124,12,20, NULL,}, // Person in red salto
    { 64,127,16,17, NULL,},
    { 80,122,16,22, NULL,},
    { 96,122,13,22, NULL,},
    {112,122,16,22, NULL,},
    {129,122,15,22, NULL,},
    {144,122,16,22, NULL,},
    {161,122, 8,22, NULL,},
    {178,125, 9,19, NULL,},
    {196,131, 8,13, NULL,}, // Red something L
    {210,129,12,15, NULL,}, // Torso punch red
    {226,128,13,16, NULL,}, // Torso swamp run blue
    {244,137,12, 7, NULL,},
    {263,138, 4, 6, NULL,}, // Hand
    {280,136, 3, 8, NULL,},
    {293,131, 8,13, NULL,}, // Red something R
    {307,132,10,12, NULL,}, // Puff smoke

    {  0,  0, 0, 0, NULL,},
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
