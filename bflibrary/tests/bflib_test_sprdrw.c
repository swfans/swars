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
#include "bffile.h"
#include "bfpng.h"
#include "bfscrcopy.h"
#include "bfgentab.h"
#include "bfutility.h"
#include "bftstlog.h"

#include <SDL.h>

#if defined WIN32 && defined main
// Anti SDL
# undef main
#endif

/******************************************************************************/

TbBool test_spritedraw(void)
{
    static ulong seeds[] = {0x0, 0xD15C1234, 0xD15C0000, 0xD15C0005, 0xD15C000F, 0xD15C03DC,
      0xD15C07DF, 0xD15CE896, 0xB00710FA, };
    ubyte pal[PALETTE_8b_SIZE];
    ubyte ref_pal[PALETTE_8b_SIZE];
    TbPixel unaffected_colours[] = {0,};
    TbScreenModeInfo *mdinfo;
    TbScreenMode mode;
    ubyte *p_sprdata;
    TbSprite *p_sprlist;
    TbPixel *ref_buffer;
    ulong picno;
    int sprfile_no = 1;
    int tot_sprites;
    int len;

    if (LbErrorLogSetup(NULL, "tst_sprdrw.log", Lb_ERROR_LOG_NEW) != Lb_SUCCESS) {
        LOGERR("execution log setup failed");
        return false;
    }

    if (MockBaseInitialise() != Lb_SUCCESS) {
        LOGERR("bullfrog library initialization failed");
        return false;
    }
    LbMemorySetup();

    // Prepare a palette, and colour tables for it
    make_general_palette(pal);
    LbFileSaveAt("tst_gp.pal", &pal, sizeof(pal));
    LbColourTablesGenerate(pal, unaffected_colours, "tst_gptbl.dat");

    mode = get_example_sprites_screen_mode(sprfile_no);
    mdinfo = LbScreenGetModeInfo(mode);
    if (MockScreenSetupAnyMode(mode, mdinfo->Width, mdinfo->Height, pal) != Lb_SUCCESS) {
        LOGERR("mock screen initialization failed");
        return false;
    }

    MockScreenLock();

    { // Read image file containing sprites
        char loc_fname[64];
        ubyte colour_remap[PALETTE_8b_COLORS];
        ulong ref_width, ref_height;

        ref_buffer = malloc(mdinfo->Width * (mdinfo->Height + 1) * 1);
        get_example_sprites_file_name(sprfile_no, loc_fname);
        memset(ref_pal, 0, PALETTE_8b_SIZE);
        LbPngLoad(loc_fname, ref_buffer, &ref_width, &ref_height, ref_pal);
        if ((ref_width != mdinfo->Width) || (ref_height != mdinfo->Height)) {
            LOGERR("%s: unexpected sprites image size", loc_fname);
            return false;
        }
        palette_remap_to_screen(colour_remap, ref_pal);
        LbScreenCopyRemap(ref_buffer, lbDisplay.WScreen, lbDisplay.GraphicsWindowHeight,
          colour_remap);
        free(ref_buffer);
    }

    tot_sprites = get_example_sprites_total_count(sprfile_no);
    len = (tot_sprites+1) * 8192;
    p_sprdata = LbMemoryAlloc(len);
    LbMemorySet(p_sprdata, 0, len);

    len = (tot_sprites+1) * sizeof(TbSprite);
    p_sprlist = (TbSprite *)LbMemoryAlloc(len);
    LbMemorySet(p_sprlist, 0, len);

    len = generate_example_sprites_from_screen(sprfile_no, pal, p_sprdata, p_sprlist);

    MockScreenUnlock();
    MockScreenReset();

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

        LbScreenClear(0);
        lbSeed = seeds[picno];

        LbSpriteDraw(88, 88, &p_sprlist[0]);
#if 0
        test_sprite_draw_random_sprites(pal, mdinfo->Height);

        sprintf(loc_fname, "referenc/tst_sprdrw%lu_rf.png", picno);
        LbPngLoad(loc_fname, ref_buffer, &ref_width, &ref_height, ref_pal);
        if ((ref_width != mdinfo->Width) || (ref_height != mdinfo->Height)) {
            LOGERR("%s: unexpected reference image size", loc_fname);
            return false;
        }
#endif
        sprintf(loc_fname, "tst_sprdrw%lu.png", picno);
        LbPngSaveScreen(loc_fname, lbDisplay.WScreen, pal, true);
#if 0
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
#endif
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
