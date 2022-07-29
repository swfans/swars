/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_render.c
 *     Rendering the 3D view elements functions.
 * @par Purpose:
 *     Some functions to be moved to bflibrary.
 * @par Comment:
 *     To be moved to bflibrary tests.
 * @author   Tomasz Lis
 * @date     20 Mar 2009 - 30 Mar 2009
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "bflib_render.h"

#include "bfscreen.h"
#include "bfwindows.h"
#include "bfpalette.h"
#include "bfutility.h"
#include "bfmemory.h"
#include "bfplanar.h"
#include "bfanywnd.h"
#include "bfmouse.h"
#include "bffile.h"
#include "bfpng.h"
#include "bfscrsurf.h"
#include "bfgentab.h"
#include "bfpalcrss.h"
#include "poly.h"
#include "swlog.h"


/******************************************************************************/
extern ulong seed;
/******************************************************************************/

void LbRegisterStandardVideoModes(void);
ulong LbRandomAnyShort(void);

/** Initialize for tests which do not use real video or input.
 */
TbResult MockBaseInitialise(void)
{
    // Clear global variables
    lbScreenInitialised = false;
    lbAppActive = true;
    LbMouseChangeMoveRatio(256, 256);
    // Register default video modes
    if (lbScreenModeInfoNum == 0) {
        LbRegisterStandardVideoModes();
    }
    lbLibInitialised = true;
    return Lb_SUCCESS;
}

struct TbPoint mock_hotspot;

TbBool MockMouseIsInstalled(void)
{
    if (!lbMouseInstalled)
        return false;
    return true;
}

TbResult MockMouseChangeSprite(const struct TbSprite *pointer_spr)
{
    if (!lbMouseInstalled)
        return Lb_FAIL;
    lbDisplay.MouseSprite = pointer_spr;
    return Lb_SUCCESS;
}

TbResult MockMouseChangeSpriteOffset(long hot_x, long hot_y)
{
    if (!lbMouseInstalled)
        return Lb_FAIL;
    mock_hotspot.x = -hot_x;
    mock_hotspot.y = -hot_y;
    return Lb_SUCCESS;
}

TbResult MockMouseGetSpriteOffset(long *hot_x, long *hot_y)
{
    *hot_x = -mock_hotspot.x;
    *hot_y = -mock_hotspot.y;
    return Lb_SUCCESS;
}

extern ubyte lbPalette[PALETTE_8b_SIZE];

TbResult MockPaletteSet(const ubyte *palette)
{
    if (!lbScreenInitialised)
      return Lb_FAIL;

    memcpy(lbPalette, palette, PALETTE_8b_SIZE);
    lbDisplay.Palette = lbPalette;
    return Lb_SUCCESS;
}

extern long lbPhysicalResolutionMul;
extern long lbMinPhysicalScreenResolutionDim;

TbResult MockScreenFindVideoModes(void)
{
    int i;
    for (i = 0; i < lbScreenModeInfoNum; i++)
        lbScreenModeInfo[i].Available = true;
    return Lb_SUCCESS;
}

TbBool MockScreenIsModeAvailable(TbScreenMode mode)
{
    TbScreenModeInfo *mdinfo;
    static TbBool setup = false;
    if (!setup) {
        MockScreenFindVideoModes();
        setup = true;
    }
    mdinfo = LbScreenGetModeInfo(mode);
    return mdinfo->Available;
}

TbResult MockScreenSetupAnyMode(TbScreenMode mode, TbScreenCoord width,
    TbScreenCoord height, ubyte *palette)
{
    long hot_x, hot_y;
    long mdWidth, mdHeight;
    const struct TbSprite *msspr;
    TbScreenModeInfo *mdinfo;

    msspr = NULL;
    if (!lbLibInitialised) {
        LOGERR("set screen mode on ununitialized bflibrary");
        return Lb_FAIL;
    }

    if (lbDisplay.MouseSprite != NULL) {
        msspr = lbDisplay.MouseSprite;
        MockMouseGetSpriteOffset(&hot_x, &hot_y);
    }
    MockMouseChangeSprite(NULL);

    free((void *)lbScreenSurface);
    if (lbHasSecondSurface) {
        free((void *)lbDrawSurface);
        lbHasSecondSurface = false;
    }
    lbScreenInitialised = false;

    if (lbDisplay.OldVideoMode == 0)
        lbDisplay.OldVideoMode = 0xFF;

    mdinfo = LbScreenGetModeInfo(mode);
    if (!MockScreenIsModeAvailable(mode)) {
        LOGERR("%s resolution %dx%d (mode %d) not available",
            (mdinfo->VideoMode & Lb_VF_WINDOWED) ? "windowed" : "full screen",
            (int)mdinfo->Width, (int)mdinfo->Height, (int)mode);
        return Lb_FAIL;
    }

    {
        long minDim = min(mdinfo->Width,mdinfo->Height);
        if ((minDim != 0) && (minDim < lbMinPhysicalScreenResolutionDim)) {
            lbPhysicalResolutionMul = (lbMinPhysicalScreenResolutionDim + minDim - 1) / minDim;
        } else {
            lbPhysicalResolutionMul = 1;
        }
        mdWidth = mdinfo->Width * lbPhysicalResolutionMul;
        mdHeight = mdinfo->Height * lbPhysicalResolutionMul;
        LOGDBG("physical resolution multiplier %ld", lbPhysicalResolutionMul);
    }
    lbDisplay.VesaIsSetUp = false;

    lbScreenSurface = lbDrawSurface = (OSSurfaceHandle)malloc(mdWidth *
      mdHeight * (mdinfo->BitsPerPixel+7) / 8);

    if ((mdinfo->BitsPerPixel != lbEngineBPP) ||
        (mdWidth != width) || (mdHeight != height))
    {
        lbDrawSurface = (OSSurfaceHandle)malloc(width * height * (lbEngineBPP+7) / 8);
        lbHasSecondSurface = true;
    }

    lbDisplay.DrawFlags = 0;
    lbDisplay.DrawColour = 0;
#if defined(ENABLE_SHADOW_COLOUR)
    lbDisplay.ShadowColour = 0;
#endif
    lbDisplay.PhysicalScreenWidth = mdinfo->Width;
    lbDisplay.PhysicalScreenHeight = mdinfo->Height;
    lbDisplay.ScreenMode = mode;
    lbDisplay.PhysicalScreen = NULL;
    lbDisplay.GraphicsScreenWidth  = width;
    lbDisplay.GraphicsScreenHeight = height;

    lbDisplay.WScreen = NULL;
    lbDisplay.GraphicsWindowPtr = NULL;

    lbScreenInitialised = true;
    if (palette != NULL)
    {
        if (MockPaletteSet(palette) != Lb_SUCCESS) {
            LOGERR("palette setting failed");
            return Lb_FAIL;
        }
    }
    LbScreenSetGraphicsWindow(0, 0, mdinfo->Width, mdinfo->Height);
    LbTextSetWindow(0, 0, mdinfo->Width, mdinfo->Height);
    if ( MockMouseIsInstalled() )
    {
        MockMouseChangeSpriteOffset(hot_x, hot_y);
        if (msspr != NULL)
          MockMouseChangeSprite(msspr);
    }
    return Lb_SUCCESS;
}

TbResult MockScreenReset(void)
{
    if (!lbScreenInitialised)
      return Lb_FAIL;

    free((void *)lbScreenSurface);
    if (lbHasSecondSurface) {
        free((void *)lbDrawSurface);
        lbHasSecondSurface = false;
    }
    lbDrawSurface = NULL;
    lbScreenSurface = NULL;
    lbScreenInitialised = false;
    return Lb_SUCCESS;
}

TbBool MockScreenIsLocked(void)
{
    return (lbDisplay.WScreen != NULL) && (lbScreenInitialised)
      && (lbDrawSurface != NULL);
}

TbResult MockScreenLock(void)
{
    if (!lbScreenInitialised)
        return Lb_FAIL;

    lbDisplay.WScreen = (ubyte *) lbDrawSurface;
    lbDisplay.GraphicsWindowPtr = &lbDisplay.WScreen[lbDisplay.GraphicsWindowX +
        lbDisplay.GraphicsScreenWidth * lbDisplay.GraphicsWindowY];

    return Lb_SUCCESS;
}

TbResult MockScreenUnlock(void)
{
    if (!lbScreenInitialised)
        return Lb_FAIL;

    lbDisplay.WScreen = NULL;
    lbDisplay.GraphicsWindowPtr = NULL;
    return Lb_SUCCESS;
}

/** Make simple and fairly universal palette.
 */
void make_general_palette(ubyte *pal)
{
    short i, k;
    ubyte *p;

    p = pal;
    k = 0;
    // Grey
    for (i = 0; i < 64; i++) {
        p[0] = i;
        p[1] = i;
        p[2] = i;
        p += 3;
        k++;
    }
    // Red
    for (i = 0; i < 24; i++) {
        p[0] = 55*i/23 + 8;
        p[1] = 0;
        p[2] = 0;
        p += 3;
        k++;
    }
    // Light Red
    for (i = 0; i < 8; i++) {
        p[0] = 55*i/7 + 8;
        p[1] = p[0] / 2;
        p[2] = p[0] / 2;
        p += 3;
        k++;
    }
    // Green
    for (i = 0; i < 24; i++) {
        p[0] = 0;
        p[1] = 55*i/23 + 8;
        p[2] = 0;
        p += 3;
        k++;
    }
    // Light Green
    for (i = 0; i < 8; i++) {
        p[1] = 55*i/7 + 8;
        p[0] = p[1] / 2;
        p[2] = p[1] / 2;
        p += 3;
        k++;
    }
    // Blue
    for (i = 0; i < 24; i++) {
        p[0] = 0;
        p[1] = 0;
        p[2] = 55*i/23 + 8;
        p += 3;
        k++;
    }
    // Light Blue
    for (i = 0; i < 8; i++) {
        p[2] = 55*i/7 + 8;
        p[0] = p[2] / 2;
        p[1] = p[2] / 2;
        p += 3;
        k++;
    }
    // Yellow
    for (i = 0; i < 24; i++) {
        p[0] = 55*i/23 + 8;
        p[1] = 55*i/23 + 8;
        p[2] = 0;
        p += 3;
        k++;
    }
    // Light Yellow
    for (i = 0; i < 8; i++) {
        p[0] = 55*i/7 + 8;
        p[1] = 55*i/7 + 8;
        p[2] = p[0] / 2;
        p += 3;
        k++;
    }
    // Magenta
    for (i = 0; i < 24; i++) {
        p[0] = 55*i/23 + 8;
        p[1] = 0;
        p[2] = 55*i/23 + 8;
        p += 3;
        k++;
    }
    // Light Magenta
    for (i = 0; i < 8; i++) {
        p[0] = 55*i/7 + 8;
        p[1] = p[0] / 2;
        p[2] = 55*i/7 + 8;
        p += 3;
        k++;
    }
    // Cyan
    for (i = 0; i < 24; i++) {
        p[0] = 0;
        p[1] = 55*i/23 + 8;
        p[2] = 55*i/23 + 8;
        p += 3;
        k++;
    }
    // Light Cyan
    for (i = 0; i < 8; i++) {
        p[1] = 55*i/7 + 8;
        p[0] = p[1] / 2;
        p[2] = 55*i/7 + 8;
        p += 3;
        k++;
    }
}

void generate_xor_textute(ubyte *Tex, ushort Width, ushort Height, ushort FillW,
  ushort FillH, const ubyte *Pal, short FactorsX[], short FactorsY[])
{
    short x, y;
    ubyte *t;

    t = Tex;
    for (y = 0; y < FillH; y++)
    {
        for (x = 0; x < FillW; x++)
        {
            ubyte r, g, b;
            r = ((x * FactorsX[0]) >> 10) ^ ((y * FactorsY[0]) >> 10);
            g = ((x * FactorsX[1]) >> 10) ^ ((y * FactorsY[1]) >> 10);
            b = ((x * FactorsX[2]) >> 10) ^ ((y * FactorsY[2]) >> 10);
            t[x] = LbPaletteFindColour(Pal, r, g, b);
        }
        t += Width;
    }
}

void raw_to_wscreen(short X, short Y, ushort Width, ushort Height, ubyte *Raw)
{
    short y;
    ushort w, h;
    ubyte *r;
    ubyte *o;

    r = Raw;

    if (X >= 0) {
        w = Width;
    } else if (X > -Width) {
        w = Width - X;
        Raw += (-X);
    } else
        return;
    if (X + w > lbDisplay.GraphicsScreenWidth)
        w = lbDisplay.GraphicsScreenWidth - X;

    if (Y >= 0) {
        h = Height;
    } else if (Y > -Height) {
        h = Height - Y;
        Raw += lbDisplay.GraphicsScreenWidth * (-Y);
    } else
        return;
    if (Y + h > lbDisplay.GraphicsScreenHeight)
        h = lbDisplay.GraphicsScreenHeight - Y;

    o = lbDisplay.WScreen + max(Y,0) * lbDisplay.GraphicsScreenWidth + max(X,0);

    for (y = 0; y < h; y++)
    {
        memcpy(o, r, w);
        r += Width;
        o += lbDisplay.GraphicsScreenWidth;
    }
}

void generate_example_texture_map_xor_based(const ubyte *pal, ubyte *texmap)
{
    ubyte *tmap;
    short factors0[] = {256, 256, 256};
    tmap = texmap;
    generate_xor_textute(tmap, 256, 256, 256,  64, pal, factors0, factors0);
    short factors1[] = {512,   0,   0};
    tmap = texmap + 256 *  64 + 0;
    generate_xor_textute(tmap, 256, 256, 128,  64, pal, factors1, factors1);
    short factors2[] = {  0, 512,   0};
    tmap = texmap + 256 *  64 + 128;
    generate_xor_textute(tmap, 256, 256, 128,  64, pal, factors2, factors2);
    short factors3[] = {  0,   0, 512};
    tmap = texmap + 256 * 128 + 0;
    generate_xor_textute(tmap, 256, 256, 128,  64, pal, factors3, factors3);
    short factors4[] = {512, 512,   0};
    tmap = texmap + 256 * 128 + 128;
    generate_xor_textute(tmap, 256, 256, 128,  64, pal, factors4, factors4);
    short factors5[] = {512,   0, 512};
    tmap = texmap + 256 * 192 + 0;
    generate_xor_textute(tmap, 256, 256, 128,  64, pal, factors5, factors5);
    short factors6[] = {  0, 512, 512};
    tmap = texmap + 256 * 192 + 128;
    generate_xor_textute(tmap, 256, 256, 128,  64, pal, factors6, factors6);
}

void test_trig_draw_random_triangles(const ubyte *pal)
{
    int i;

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

        if ((point_c.Y - point_b.Y) * (point_b.X - point_a.X) -
            (point_b.Y - point_a.Y) * (point_c.X - point_b.X) > 0)
            trig_dbg(&point_a, &point_b, &point_c);
        else
            trig_dbg(&point_a, &point_c, &point_b);
    }
}

TbBool test_trig(void)
{
    static ulong seeds[] = {0x0, 0xD15C1234, 0xD15C0000, 0xD15C0005, 0xD15C000F, 0xD15C03DC,
      0xD15C07DF, 0xD15CE896, };
    ubyte pal[PALETTE_8b_SIZE];
    ubyte ref_pal[PALETTE_8b_SIZE];
    TbPixel unaffected_colours[] = {0,};
    ubyte *texmap;
    TbPixel *ref_buffer;
    ulong picno;

#if 0
    if (LbErrorLogSetup(NULL, "tst_trig.log", Lb_ERROR_LOG_NEW) != Lb_SUCCESS) {
        LOGERR("execution log setup failed");
        return false;
    }
#endif
    if (MockBaseInitialise() != Lb_SUCCESS) {
        LOGERR("bullfrog Library initialization failed");
        return false;
    }
    LbMemorySetup();

    // Prepare a palette, and colour tables for it
    make_general_palette(pal);
    LbFileSaveAt("tst_gp.pal", &pal, sizeof(pal));
    LbColourTablesGenerate(pal, unaffected_colours, "tst_gptbl.dat");

    if (MockScreenSetupAnyMode(Lb_SCREEN_MODE_640_480_8, 640, 480, pal) != Lb_SUCCESS) {
        LOGERR("bullfrog Library initialization failed");
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

    ref_buffer = malloc(640 * 480 * (lbEngineBPP+7) / 8);
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
        seed = seeds[picno];
        test_trig_draw_random_triangles(pal);

        sprintf(loc_fname, "referenc/tst_trig%lu_rf.png", picno);
        LbPngLoad(loc_fname, ref_buffer, &ref_width, &ref_height, ref_pal);
        if ((ref_width != 640) || (ref_height != 480)) {
            LOGERR("%s: unexpected reference image size", loc_fname);
            return false;
        }

        sprintf(loc_fname, "tst_trig%lu.png", picno);
        LbPngSaveScreen(loc_fname, lbDisplay.WScreen, pal, true);

        // compare image with reference
        maxpos = 0;
        maxdiff = LbImageBuffersMaxDifference(lbDisplay.WScreen, pal, ref_buffer,
          ref_pal, 640 * 480, &maxpos);
       if (maxdiff > 12) {
            LOGERR("%s: high pixel difference to reference (%ld) at (%lu,%lu)",
              loc_fname, maxdiff, maxpos%640, maxpos/640);
            return false;
        }
        LOGSYNC("%s: acceptable pixel difference to reference (%ld) at (%lu,%lu)",
            loc_fname, maxdiff, maxpos%640, maxpos/640);
    }

    LbMemoryFree(texmap);
    free(ref_buffer);
    
    MockScreenUnlock();
    MockScreenReset();
    LbMemoryReset();
    LOGSYNC("passed");
    return true;
}

TbBool test_memory(void)
{
    void *p;

    if (MockBaseInitialise() != Lb_SUCCESS) {
        LOGERR("bullfrog Library initialization failed");
        return false;
    }
    LbMemorySetup();

    p = LbMemoryAlloc(2048);
    LbMemoryFree(p);

    LbMemoryReset();
    LOGSYNC("passed");
    return true;
}

/******************************************************************************/
