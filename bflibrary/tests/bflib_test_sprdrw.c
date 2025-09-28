/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_test_sprdrw.c
 *     Test application for LbSpriteDraw() functions family.
 * @par Purpose:
 *     Testing implementation of bflibrary routines.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     25 May 2022 - 29 Jun 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bfsprite.h"

#include "../tests/helpers_sprite.h"
#include "../tests/mock_bfmouse.h"
#include "../tests/mock_bfpalette.h"
#include "../tests/mock_bfscreen.h"
#include "../tests/mock_bfwindows.h"
#include "bfmemory.h"
#include "bfmemut.h"
#include "bfpalette.h"
#include "bfpalcrss.h"
#include "bfplanar.h"
#include "bffile.h"
#include "bfpng.h"
#include "bfscrcopy.h"
#include "bfgentab.h"
#include "bfutility.h"
#include "bftstlog.h"
#include "insspr.h"

#include <SDL.h>

#if defined WIN32 && defined main
// Anti SDL
# undef main
#endif

enum DrawSprFuncType {
    DrwSprFn_Normal,
    DrwSprFn_OneColour,
    DrwSprFn_Remap,
};

/******************************************************************************/

/** Test drawing random sprites using the LbSpriteDraw(Normal/OneColour/Remap)() functions.
 * @pal 256-colour palette buffer
 * @res_h Scaling factor in form of horizonal resolution
 * @drwtype Type of the draw function to use (Normal/OneColour/Remap)
 * @p_sprlist Sprites list pointer
 * @amount Amount of sprites to draw
 * @tot_sprites Count of sprites in the list
 */
void test_sprite_draw_random_sprites(const ubyte *pal, short res_h, ubyte drwtype, TbSprite *p_sprlist, int amount, int tot_sprites)
{
    int i, scale;
    TbPixel colour;
    ubyte cmap[PALETTE_8b_COLORS];

    scale = 16 * res_h / 480;

    if (drwtype == DrwSprFn_Remap)
    {
        for (i = 0; i < PALETTE_8b_COLORS; i++)
        {
            cmap[i] = LbPaletteFindColour(pal, 0x3f - pal[3*i+0],
              0x3f - pal[3*i+1], 0x3f - pal[3*i+2]);
        }
    }

    for (i = 0; i < amount; i++)
    {
        struct TbPoint point_a;
        TbSprite *p_spr;
        ushort rnd;

        rnd = LbRandomAnyShort();
        if (drwtype == DrwSprFn_OneColour)
        {
            // Random colour (used only for OneColor draw type)
            colour = LbPaletteFindColour(pal, (rnd >> 0) & 0x3f,
              (rnd >> 5) & 0x3f, (rnd >> 10) & 0x3f);
        }

        // Random draw flags
        lbDisplay.DrawFlags = 0;
        switch ((rnd >> 6) & 3)
        {
        case 1:
            lbDisplay.DrawFlags |= Lb_SPRITE_TRANSPAR4;
            break;
        case 2:
        case 3:
            lbDisplay.DrawFlags |= Lb_SPRITE_TRANSPAR8;
            break;
        default:
            break;
        }
        if (((rnd >> 4) & 3) == 0)
            lbDisplay.DrawFlags |= Lb_SPRITE_OUTLINE;

        // Random positions
        {
            rnd = LbRandomAnyShort();
            point_a.x = ((rnd >> 0) & 1023) - (1023 - 640) / 2;
            point_a.y = ((rnd >> 6) &  511) - (511 - 480) / 2;
        }
        {
            rnd = LbRandomAnyShort();
            p_spr = &p_sprlist[rnd % tot_sprites];
        }

        point_a.x = (scale * point_a.x) >> 4;
        point_a.y = (scale * point_a.y) >> 4;

        switch (drwtype)
        {
        case DrwSprFn_Normal:
            LbSpriteDraw(point_a.x, point_a.y, p_spr);
            break;
        case DrwSprFn_OneColour:
            LbSpriteDrawOneColour(point_a.x, point_a.y, p_spr, colour);
            break;
        case DrwSprFn_Remap:
            LbSpriteDrawRemap(point_a.x, point_a.y, p_spr, cmap);
            break;
        }
    }
}

/** Test drawing random sprites using the LbSpriteDrawScaled(Normal/OneColour/Remap)() functions.
 * @pal 256-colour palette buffer
 * @res_h Scaling factor in form of horizonal resolution
 * @drwtype Type of the draw function to use (Normal/OneColour/Remap)
 * @p_sprlist Sprites list pointer
 * @amount Amount of sprites to draw
 * @tot_sprites Count of sprites in the list
 */
void test_sprite_draw_random_sprites_scaled(const ubyte *pal, short res_h, ubyte drwtype, TbSprite *p_sprlist, int amount, int tot_sprites)
{
    int i, scale;
    TbPixel colour;
    ubyte cmap[PALETTE_8b_COLORS];

    scale = 16 * res_h / 480;

    if (drwtype == DrwSprFn_Remap)
    {
        for (i = 0; i < PALETTE_8b_COLORS; i++)
        {
            cmap[i] = LbPaletteFindColour(pal, 0x3f - pal[3*i+0],
              0x3f - pal[3*i+1], 0x3f - pal[3*i+2]);
        }
    }

    for (i = 0; i < amount; i++)
    {
        struct TbPoint point_a, size_a;
        TbSprite *p_spr;
        ushort rnd;

        rnd = LbRandomAnyShort();
        if (drwtype == DrwSprFn_OneColour)
        {
            // Random colour (used only for OneColor draw type)
            colour = LbPaletteFindColour(pal, (rnd >> 0) & 0x3f,
              (rnd >> 5) & 0x3f, (rnd >> 10) & 0x3f);
        }

        // Random draw flags
        lbDisplay.DrawFlags = 0;
        switch ((rnd >> 6) & 3)
        {
        case 1:
            lbDisplay.DrawFlags |= Lb_SPRITE_TRANSPAR4;
            break;
        case 2:
        case 3:
            lbDisplay.DrawFlags |= Lb_SPRITE_TRANSPAR8;
            break;
        default:
            break;
        }
        if (((rnd >> 4) & 3) == 0)
            lbDisplay.DrawFlags |= Lb_SPRITE_OUTLINE;

        // Random positions
        {
            rnd = LbRandomAnyShort();
            point_a.x = ((rnd >> 0) & 1023) - (1023 - 640) / 2;
            point_a.y = ((rnd >> 6) &  511) - (511 - 480) / 2;
        }
        {
            rnd = LbRandomAnyShort();
            p_spr = &p_sprlist[rnd % tot_sprites];

            rnd = LbRandomAnyShort();
            if (i < amount/64) // small part of 1/4 - enlarge with huge scale, similar proportions
            {
                int scale;

                scale = ((rnd >> 0) & 127);
                size_a.x = (p_spr->SWidth * (scale + scale/(((rnd >> 6) & 15) + 1))) + 1;
                size_a.y = (p_spr->SHeight * (scale + scale/(((rnd >> 10) & 15) + 1))) + 1;
            }
            else if (i < amount/4) // 1/4 enlarge without too much deviation in proportions
            {
                int scale;

                scale = ((rnd >> 0) & 7);
                size_a.x = (p_spr->SWidth * (scale + scale/(((rnd >> 6) & 3) + 1))) + 1;
                size_a.y = (p_spr->SHeight * (scale + scale/(((rnd >> 10) & 3) + 1))) + 1;
            }
            else if (i < 2*amount/4) // 1/4 - completely random sizes mith moderate maximum
            {
                size_a.x = ((rnd >> 0) & 127) + 1;
                size_a.y = ((rnd >> 6) & 127) + 1;
            }
            else if (i < 3*amount/4) // 1/4 - reduce size without too much deviation in proportions
            {
                int scale;

                scale = ((rnd >> 0) & 15);
                if (scale < 1)
                    scale = 1;
                size_a.x = (p_spr->SWidth  / (scale + scale/(((rnd >>  6) & 3) + 1)));
                size_a.y = (p_spr->SHeight / (scale + scale/(((rnd >> 10) & 3) + 1)));
            }
            else // 1/4 - enlarge or reduce size with aspect ratio kept
            {
                int scale;

                scale = ((rnd >> 0) & 63);
                size_a.x = ((p_spr->SWidth * scale) >> 3) + 1;
                size_a.y = ((p_spr->SHeight * scale) >> 3) + 1;
            }
        }

        point_a.x = (scale * point_a.x) >> 4;
        point_a.y = (scale * point_a.y) >> 4;
        size_a.x = (scale * size_a.x) >> 4;
        size_a.y = (scale * size_a.y) >> 4;

        switch (drwtype)
        {
        case DrwSprFn_Normal:
            LbSpriteDrawScaled(point_a.x, point_a.y, p_spr, size_a.x, size_a.y);
            break;
        case DrwSprFn_OneColour:
            LbSpriteDrawScaledOneColour(point_a.x, point_a.y, p_spr, size_a.x, size_a.y, colour);
            break;
        case DrwSprFn_Remap:
            LbSpriteDrawScaledRemap(point_a.x, point_a.y, p_spr, size_a.x, size_a.y, cmap);
            break;
        }
    }
}

/** Generate sprite data for testing sprites.
 *
 */
TbBool test_spritedraw_generate(int sprfile_no, ubyte *pal, ubyte **pp_sprdata, TbSprite **pp_sprlist, int *p_tot_sprites)
{
    TbPixel *sprite_screen;
    TbScreenModeInfo *mdinfo;
    TbScreenMode mode;
    int len;

    mode = get_example_sprites_screen_mode(sprfile_no);
    mdinfo = LbScreenGetModeInfo(mode);
    if (MockScreenSetupAnyMode(mode, mdinfo->Width, mdinfo->Height, pal) != Lb_SUCCESS) {
        LOGERR("mock screen initialization failed");
        return false;
    }

    MockScreenLock();

    { // Read image file containing sprites
        char loc_fname[64];
        ubyte ref_pal[PALETTE_8b_SIZE];
        ubyte colour_remap[PALETTE_8b_COLORS];
        ulong ref_width, ref_height;

        sprite_screen = malloc(mdinfo->Width * (mdinfo->Height + 1) * 1);
        get_example_sprites_file_name(sprfile_no, loc_fname);
        memset(ref_pal, 0, PALETTE_8b_SIZE);
        if (LbPngLoad(loc_fname, sprite_screen, &ref_width, &ref_height, ref_pal) != Lb_SUCCESS) {
            LOGERR("%s: unable to load PNG with sprites", loc_fname);
            return false;
        }
        if ((ref_width != mdinfo->Width) || (ref_height != mdinfo->Height)) {
            LOGERR("%s: unexpected sprites image size, %lux%lu", loc_fname, ref_width, ref_height);
            return false;
        }
        palette_remap_to_screen(colour_remap, ref_pal);
        LbScreenCopyRemap(sprite_screen, lbDisplay.WScreen, lbDisplay.GraphicsWindowHeight,
          colour_remap);
        free(sprite_screen);
    }

    // Allocate memory for DAT part
    *p_tot_sprites = get_example_sprites_total_count(sprfile_no);
    len = ((*p_tot_sprites) + 1) * 8192;
    *pp_sprdata = LbMemoryAlloc(len);
    LbMemorySet(*pp_sprdata, 0, len);

    // Allocate memory for TAB part
    len = ((*p_tot_sprites) + 1) * sizeof(TbSprite);
    *pp_sprlist = (TbSprite *)LbMemoryAlloc(len);
    LbMemorySet(*pp_sprlist, 0, len);

    // Generate sprites from bitmap data
    len = generate_example_sprites_from_screen(sprfile_no, pal, *pp_sprdata, *pp_sprlist);

    MockScreenUnlock();
    MockScreenReset();

    return true;
}


TbBool test_spritedraw(void)
{
    static ulong seeds[] = {0x0, 0xD15C1234, 0xD15C0000, 0xD15C0005, 0xD15C000F, 0xD15C03DC,
      0xD15C07DF, 0xD15CE896, 0xB00710FA, 0x37733773, };
    ubyte pal[PALETTE_8b_SIZE];
    ubyte ref_pal[PALETTE_8b_SIZE];
    TbPixel unaffected_colours[] = {0,};
    ubyte *p_sprdata;
    TbSprite *p_sprlist;
    TbPixel *ref_buffer;
    TbScreenModeInfo *mdinfo;
    TbScreenMode mode;
    ulong picno;
    int sprfile_no = 1;
    int tot_sprites;

    if (LbErrorLogSetup(NULL, "tst_sprdrw.log", Lb_ERROR_LOG_NEW) != Lb_SUCCESS) {
        LOGERR("execution log setup failed");
        return false;
    }

    if (MockBaseInitialise() != Lb_SUCCESS) {
        LOGERR("bullfrog library initialization failed");
        return false;
    }
    LbMemorySetup();

    // Prepare a palette, and colour tables for altering colours from that palette
    make_general_palette(pal);
    LbFileSaveAt("tst_gp.pal", &pal, sizeof(pal));
    LbColourTablesGenerate(pal, unaffected_colours, "tst_gptbl.dat");
    render_ghost = &pixmap.ghost_table[0*PALETTE_8b_COLORS];

    // Generate sprite DAT/TAB loaded data to test
    if (!test_spritedraw_generate(sprfile_no, pal, &p_sprdata, &p_sprlist, &tot_sprites)) {
        return false;
    }

    mode = Lb_SCREEN_MODE_640_480_8;
    mdinfo = LbScreenGetModeInfo(mode);
    if (MockScreenSetupAnyMode(mode, mdinfo->Width, mdinfo->Height, pal) != Lb_SUCCESS) {
        LOGERR("mock screen initialization failed");
        return false;
    }

    MockScreenLock();

    ref_buffer = malloc(mdinfo->Width * mdinfo->Height * (lbEngineBPP+7) / 8);
    if (ref_buffer == NULL) {
        LOGERR("reference screen buffer alloc failed");
        return false;
    }

    for (picno = 1; picno < sizeof(seeds)/sizeof(seeds[0]); picno++)
    {
        char loc_fname[64];
        ulong ref_width, ref_height;
        long maxdiff;
        ulong maxpos;
        ubyte drwtype;

        switch ((picno-1) % 3)
        {
        case 0:
            drwtype = DrwSprFn_Normal;
            break;
        case 1:
            drwtype = DrwSprFn_OneColour;
            break;
        case 2:
            drwtype = DrwSprFn_Remap;
            break;
        }

        LbScreenClear(0);
        lbSeed = seeds[picno];

        test_sprite_draw_random_sprites(pal, mdinfo->Height, drwtype, p_sprlist, 2000, tot_sprites);

        sprintf(loc_fname, "referenc/tst_sprdrwon%lu_rf.png", picno);
        if (LbPngLoad(loc_fname, ref_buffer, &ref_width, &ref_height, ref_pal) != Lb_SUCCESS) {
            LOGERR("%s: unable to load reference PNG", loc_fname);
            return false;
        }
        if ((ref_width != mdinfo->Width) || (ref_height != mdinfo->Height)) {
            LOGERR("%s: unexpected reference image size", loc_fname);
            return false;
        }

        sprintf(loc_fname, "tst_sprdrwon%lu.png", picno);
        LbPngSaveScreen(loc_fname, lbDisplay.WScreen, pal, true);

        // compare image with reference
        maxpos = 0;
        maxdiff = LbImageBuffersMaxDifference(lbDisplay.WScreen, pal, ref_buffer,
          ref_pal, mdinfo->Width * mdinfo->Height, &maxpos);
       if (maxdiff > 12) {
            LOGERR("%s: high pixel difference to reference (%ld) at (%lu,%lu)",
              loc_fname, maxdiff, maxpos % mdinfo->Width, maxpos / mdinfo->Width);
            return false;
        }
        LOGSYNC("%s: acceptable pixel difference to reference (%ld) at (%lu,%lu)",
            loc_fname, maxdiff, maxpos % mdinfo->Width, maxpos / mdinfo->Width);
    }

    for (picno = 1; picno < sizeof(seeds)/sizeof(seeds[0]); picno++)
    {
        char loc_fname[64];
        ulong ref_width, ref_height;
        long maxdiff;
        ulong maxpos;
        ubyte drwtype;

        switch ((picno-1) % 3)
        {
        case 0:
            drwtype = DrwSprFn_Normal;
            break;
        case 1:
            drwtype = DrwSprFn_OneColour;
            break;
        case 2:
            drwtype = DrwSprFn_Remap;
            break;
        }

        LbScreenClear(0);
        lbSeed = seeds[picno];

        test_sprite_draw_random_sprites_scaled(pal, mdinfo->Height, drwtype, p_sprlist, 200, tot_sprites);

        sprintf(loc_fname, "referenc/tst_sprdrwsn%lu_rf.png", picno);
        if (LbPngLoad(loc_fname, ref_buffer, &ref_width, &ref_height, ref_pal) != Lb_SUCCESS) {
            LOGERR("%s: unable to load reference PNG", loc_fname);
            return false;
        }
        if ((ref_width != mdinfo->Width) || (ref_height != mdinfo->Height)) {
            LOGERR("%s: unexpected reference image size", loc_fname);
            return false;
        }

        sprintf(loc_fname, "tst_sprdrwsn%lu.png", picno);
        LbPngSaveScreen(loc_fname, lbDisplay.WScreen, pal, true);

        // compare image with reference
        maxpos = 0;
        maxdiff = LbImageBuffersMaxDifference(lbDisplay.WScreen, pal, ref_buffer,
          ref_pal, mdinfo->Width * mdinfo->Height, &maxpos);
       if (maxdiff > 12) {
            LOGERR("%s: high pixel difference to reference (%ld) at (%lu,%lu)",
              loc_fname, maxdiff, maxpos % mdinfo->Width, maxpos / mdinfo->Width);
            return false;
        }
        LOGSYNC("%s: acceptable pixel difference to reference (%ld) at (%lu,%lu)",
            loc_fname, maxdiff, maxpos % mdinfo->Width, maxpos / mdinfo->Width);
    }

    LbMemoryFree(p_sprdata);
    free(ref_buffer);

    MockScreenUnlock();
    MockScreenReset();
    LbMemoryReset();
    LOGSYNC("passed");
    return true;
}

int main(int argc, char *argv[])
{
    if (!test_spritedraw())
        exit(51);
    exit(0);
}

/******************************************************************************/
