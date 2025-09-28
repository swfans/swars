/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_test_tringl.c
 *     Test application for LbDrawTriangle() function.
 * @par Purpose:
 *     Testing implementation of bflibrary routines.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     25 May 2022 - 25 Aug 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bftringl.h"
#include "bfplanar.h"
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

/** Test drawing random triangles using the LbDrawTriangle() function.
 */
void test_tringl_draw_random_triangles(const ubyte *pal, short res_h)
{
    int i, scale;
    TbPixel colour;

    scale = 16 * res_h / 480;

    for (i = 0; i < 7*40; i++)
    {
        struct TbPoint point_a, point_b, point_c;
        ushort rnd;

        rnd = LbRandomAnyShort();
        // Random colour
        colour = LbPaletteFindColour(pal, (rnd >> 0) & 0x3f,
          (rnd >> 5) & 0x3f, (rnd >> 10) & 0x3f);

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

        // Random positions - few big, more small
        if (i < 7*2)
        {
            rnd = LbRandomAnyShort();
            point_a.x = ((rnd >> 0) & 1023) - (1023 - 640) / 2;
            point_a.y = ((rnd >> 6) &  511) - (511 - 480) / 2;
            rnd = LbRandomAnyShort();
            point_b.x = ((rnd >> 0) & 1023) - (1023 - 640) / 2;
            point_b.y = ((rnd >> 6) &  511) - (511 - 480) / 2;
            rnd = LbRandomAnyShort();
            point_c.x = ((rnd >> 0) & 1023) - (1023 - 640) / 2;
            point_c.y = ((rnd >> 6) &  511) - (511 - 480) / 2;
            if ((point_a.x < 0) && (point_b.x < 0) && (point_c.x < 0))
                point_a.x = 640 - point_a.x;
            if ((point_a.x > 640) && (point_b.x > 640) && (point_c.x > 640))
                point_a.x = -point_a.x + 640;
            if ((point_a.y < 0) && (point_b.y < 0) && (point_c.y < 0))
                point_a.y = 480 - point_a.y;
            if ((point_a.y > 480) && (point_b.y > 480) && (point_c.y > 480))
                point_a.y = -point_a.y + 480;
        } else
        {
            rnd = LbRandomAnyShort();
            point_a.x = ((rnd >> 0) & 1023) - (1023 - 640) / 2;
            point_a.y = ((rnd >> 6) &  511) - (511 - 480) / 2;
            rnd = LbRandomAnyShort();
            point_b.x = point_a.x + ((rnd >> 0) &  63) - 31;
            point_b.y = point_a.y + ((rnd >> 6) &  63) - 31;
            rnd = LbRandomAnyShort();
            point_c.x = point_a.x + ((rnd >> 0) &  63) - 31;
            point_c.y = point_a.y + ((rnd >> 6) &  63) - 31;
            if ((point_a.x < 0) && (point_b.x < 0) && (point_c.x < 0))
                point_a.x = ((-point_a.x) & 0x3f);
            if ((point_a.x > 640) && (point_b.x > 640) && (point_c.x > 640))
                point_a.x = 640 - (point_a.x & 0x3f);
            if ((point_a.y < 0) && (point_b.y < 0) && (point_c.y < 0))
                point_a.y = ((-point_a.y) & 0x3f);
            if ((point_a.y > 480) && (point_b.y > 480) && (point_c.y > 480))
                point_a.y = 480 - (point_a.y & 0x3f);
        }

        point_a.x = (scale * point_a.x) >> 4;
        point_a.y = (scale * point_a.y) >> 4;
        point_b.x = (scale * point_b.x) >> 4;
        point_b.y = (scale * point_b.y) >> 4;
        point_c.x = (scale * point_c.x) >> 4;
        point_c.y = (scale * point_c.y) >> 4;

        if ((point_c.y - point_b.y) * (point_b.x - point_a.x) -
            (point_b.y - point_a.y) * (point_c.x - point_b.x) > 0)
            LbDrawTriangle(point_a.x, point_a.y, point_b.x, point_b.y, point_c.x, point_c.y, colour);
        else
            LbDrawTriangle(point_a.x, point_a.y, point_c.x, point_c.y, point_b.x, point_b.y, colour);
    }
}

TbBool test_tringl(void)
{
    static ulong seeds[] = {0x0, 0xD15C1234, 0xD15C0000, 0xD15C0005, 0xD15C000F,
      0xD15C03DC, 0xD15C07DF, 0xD15CE896, 0xB00710FA, };
    ubyte pal[PALETTE_8b_SIZE];
    ubyte ref_pal[PALETTE_8b_SIZE];
    TbPixel unaffected_colours[] = {0,};
    TbScreenModeInfo *mdinfo;
    TbScreenMode mode = Lb_SCREEN_MODE_640_480_8;
    TbPixel *ref_buffer;
    ulong picno;

    if (LbErrorLogSetup(NULL, "tst_tringl.log", Lb_ERROR_LOG_NEW) != Lb_SUCCESS) {
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
        test_tringl_draw_random_triangles(pal, mdinfo->Height);

        sprintf(loc_fname, "referenc/tst_tringl%lu_rf.png", picno);
        LbPngLoad(loc_fname, ref_buffer, &ref_width, &ref_height, ref_pal);
        if ((ref_width != mdinfo->Width) || (ref_height != mdinfo->Height)) {
            LOGERR("%s: unexpected reference image size", loc_fname);
            return false;
        }

        sprintf(loc_fname, "tst_tringl%lu.png", picno);
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

    free(ref_buffer);

    MockScreenUnlock();
    MockScreenReset();
    LbMemoryReset();
    LOGSYNC("passed");
    return true;
}

int main(int argc, char *argv[])
{
    if (!test_tringl())
        exit(51);
    exit(0);
}

/******************************************************************************/
