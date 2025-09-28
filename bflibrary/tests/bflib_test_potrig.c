/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_test_potrig.c
 *     Test application for trig() function.
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
#include "poly.h"
#include "helpers_screen.h"
#include "mock_bfmouse.h"
#include "mock_bfpalette.h"
#include "mock_bfscreen.h"
#include "mock_bfwindows.h"
#include "bfmemory.h"
#include "bfpalette.h"
#include "bfpalcrss.h"
#include "bffile.h"
#include "bfpng.h"
#include "bfgentab.h"
#include "bfutility.h"
#include "bftstlog.h"

#include <SDL.h>

#if defined WIN32 && defined main
// Anti SDL
# undef main
#endif

/******************************************************************************/

/** Test drawing random triangles using the trig() function.
 */
void test_trig_draw_random_triangles(const ubyte *pal, short res_h)
{
    int i, scale;

    scale = 16 * res_h / 480;

    for (i = 0; i < 27*40; i++)
    {
        struct PolyPoint point_a, point_b, point_c;
        ushort rnd;

        rnd = LbRandomAnyShort();
        vec_mode = i % 27;
        // Random colour
        vec_colour = LbPaletteFindColour(pal, (rnd >> 0) & 0x3f, (rnd >> 5) & 0x3f, (rnd >> 10) & 0x3f);
        // Random texture coords, but show one of 32x32 textures from start
        point_a.U = ((rnd >> 0) & 0x7) * 32;
        point_a.V = ((rnd >> 3) & 0x7) * 32;
        switch ((rnd >> 6) & 3)
        {
        case 0:
            point_b.U = point_a.U + 31;
            point_b.V = point_a.V + 31;
            point_c.U = point_a.U + 31;
            point_c.V = point_a.V;
            break;
        case 1:
            point_b.U = point_a.U + 31;
            point_b.V = point_a.V + 31;
            point_c.U = point_a.U;
            point_c.V = point_a.V + 31;
            break;
        case 2:
            point_b.U = point_a.U;
            point_b.V = point_a.V + 31;
            point_c.U = point_a.U + 31;
            point_c.V = point_a.V + 31;
            break;
        case 3:
            point_b.U = point_a.U + 31;
            point_b.V = point_a.V;
            point_c.U = point_a.U + 31;
            point_c.V = point_a.V + 31;
            break;
        }
        // Random lightness
        point_a.S = ((rnd >> 9) & 0x7F) << 15;
        point_b.S = ((rnd >> 6) & 0x7F) << 15;
        point_c.S = ((rnd >> 3) & 0x7F) << 15;
        // Random positions - few big, more small
        if (i < 27*2)
        {
            rnd = LbRandomAnyShort();
            point_a.X = ((rnd >> 0) & 1023) - (1023 - 640) / 2;
            point_a.Y = ((rnd >> 6) &  511) - (511 - 480) / 2;
            rnd = LbRandomAnyShort();
            point_b.X = ((rnd >> 0) & 1023) - (1023 - 640) / 2;
            point_b.Y = ((rnd >> 6) &  511) - (511 - 480) / 2;
            rnd = LbRandomAnyShort();
            point_c.X = ((rnd >> 0) & 1023) - (1023 - 640) / 2;
            point_c.Y = ((rnd >> 6) &  511) - (511 - 480) / 2;
            if ((point_a.X < 0) && (point_b.X < 0) && (point_c.X < 0))
                point_a.X = 640 - point_a.X;
            if ((point_a.X > 640) && (point_b.X > 640) && (point_c.X > 640))
                point_a.X = -point_a.X + 640;
            if ((point_a.Y < 0) && (point_b.Y < 0) && (point_c.Y < 0))
                point_a.Y = 480 - point_a.Y;
            if ((point_a.Y > 480) && (point_b.Y > 480) && (point_c.Y > 480))
                point_a.Y = -point_a.Y + 480;
        } else
        {
            rnd = LbRandomAnyShort();
            point_a.X = ((rnd >> 0) & 1023) - (1023 - 640) / 2;
            point_a.Y = ((rnd >> 6) &  511) - (511 - 480) / 2;
            rnd = LbRandomAnyShort();
            point_b.X = point_a.X + ((rnd >> 0) &  63) - 31;
            point_b.Y = point_a.Y + ((rnd >> 6) &  63) - 31;
            rnd = LbRandomAnyShort();
            point_c.X = point_a.X + ((rnd >> 0) &  63) - 31;
            point_c.Y = point_a.Y + ((rnd >> 6) &  63) - 31;
            if ((point_a.X < 0) && (point_b.X < 0) && (point_c.X < 0))
                point_a.X = ((-point_a.X) & 0x3f);
            if ((point_a.X > 640) && (point_b.X > 640) && (point_c.X > 640))
                point_a.X = 640 - (point_a.X & 0x3f);
            if ((point_a.Y < 0) && (point_b.Y < 0) && (point_c.Y < 0))
                point_a.Y = ((-point_a.Y) & 0x3f);
            if ((point_a.Y > 480) && (point_b.Y > 480) && (point_c.Y > 480))
                point_a.Y = 480 - (point_a.Y & 0x3f);
        }

        point_a.X = (scale * point_a.X) >> 4;
        point_a.Y = (scale * point_a.Y) >> 4;
        point_b.X = (scale * point_b.X) >> 4;
        point_b.Y = (scale * point_b.Y) >> 4;
        point_c.X = (scale * point_c.X) >> 4;
        point_c.Y = (scale * point_c.Y) >> 4;

        if ((point_c.Y - point_b.Y) * (point_b.X - point_a.X) -
            (point_b.Y - point_a.Y) * (point_c.X - point_b.X) > 0)
            trig(&point_a, &point_b, &point_c);
        else
            trig(&point_a, &point_c, &point_b);
    }
}

TbBool test_trig(void)
{
    static ulong seeds[] = {0x0, 0xD15C1234, 0xD15C0000, 0xD15C0005, 0xD15C000F, 0xD15C03DC,
      0xD15C07DF, 0xD15CE896, 0xB00710FA, };
    ubyte pal[PALETTE_8b_SIZE];
    ubyte ref_pal[PALETTE_8b_SIZE];
    TbPixel unaffected_colours[] = {0,};
    TbScreenModeInfo *mdinfo;
    TbScreenMode mode = Lb_SCREEN_MODE_640_480_8;
    ubyte *texmap;
    TbPixel *ref_buffer;
    ulong picno;

    if (LbErrorLogSetup(NULL, "tst_trig.log", Lb_ERROR_LOG_NEW) != Lb_SUCCESS) {
        LOGERR("execution log setup failed");
        return false;
    }

    if (MockBaseInitialise() != Lb_SUCCESS) {
        LOGERR("bullfrog library initialization failed");
        return false;
    }
    LbMemorySetup();

    mdinfo = LbScreenGetModeInfo(mode);

    // Prepare a palette, and colour tables for altering colours from that palette
    make_general_palette(pal);
    LbFileSaveAt("tst_gp.pal", &pal, sizeof(pal));
    LbColourTablesGenerate(pal, unaffected_colours, "tst_gptbl.dat");

    if (MockScreenSetupAnyMode(mode, mdinfo->Width, mdinfo->Height, pal) != Lb_SUCCESS) {
        LOGERR("mock screen initialization failed");
        return false;
    }

    MockScreenLock();

    texmap = LbMemoryAlloc(256*256*1);
    generate_example_texture_map_xor_based(pal, texmap);

    setup_vecs(lbDisplay.WScreen, texmap, lbDisplay.PhysicalScreenWidth,
        lbDisplay.PhysicalScreenWidth, lbDisplay.PhysicalScreenHeight);

#if 0
    LbPaletteDraw(320-256-8, 32, 16, 16, 0x01);
    raw_to_wscreen(320+8, 32, 256, 256, texmap);
#endif

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

        LbScreenClear(0);
        lbSeed = seeds[picno];
        test_trig_draw_random_triangles(pal, mdinfo->Height);

        sprintf(loc_fname, "referenc/tst_trig%lu_rf.png", picno);
        LbPngLoad(loc_fname, ref_buffer, &ref_width, &ref_height, ref_pal);
        if ((ref_width != mdinfo->Width) || (ref_height != mdinfo->Height)) {
            LOGERR("%s: unexpected reference image size", loc_fname);
            return false;
        }

        sprintf(loc_fname, "tst_trig%lu.png", picno);
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

    LbMemoryFree(texmap);
    free(ref_buffer);

    MockScreenUnlock();
    MockScreenReset();
    LbMemoryReset();
    LOGSYNC("passed");
    return true;
}

int main(int argc, char *argv[])
{
    if (!test_trig())
        exit(51);
    exit(0);
}

/******************************************************************************/
