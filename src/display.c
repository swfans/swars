#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "display.h"
#include "bfscreen.h"
#include "bfscrsurf.h"
#include "bfpalette.h"
#include "bfsprite.h"
#include "bftext.h"
#include "bfmouse.h"
#include "bfutility.h"
#include "poly.h"
#include "util.h"
#include "swlog.h"

TbScreenMode screen_mode_game_hi = Lb_SCREEN_MODE_640_480_8;
TbScreenMode screen_mode_game_lo = Lb_SCREEN_MODE_320_200_8;
TbScreenMode screen_mode_menu = Lb_SCREEN_MODE_640_480_8;
TbScreenMode screen_mode_fmvid_lo = Lb_SCREEN_MODE_320_200_8;
TbScreenMode screen_mode_fmvid_hi = Lb_SCREEN_MODE_640_480_8;

extern ushort data_1aa330;
extern ushort data_1aa332;
extern unsigned char *display_palette;

TbPixel fade_unaffected_colours[] = {
  1,2,3,
  128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
  224,225,226,227,228,229,230,231,
  240,241,242,243,244,245,246,
  254,255,
  0};

#if defined(WIN32)

const char * SWResourceMapping(short index)
{
    switch (index)
    {
    case 1:
        return "A";
        //return MAKEINTRESOURCE(110); -- may work for other resource compilers
    default:
        return NULL;
    }
}

#else

const char * SWResourceMapping(short index)
{
    switch (index)
    {
    case 1:
        return "swars_icon.png";
    default:
        return NULL;
    }
}

#endif

void swap_wscreen(void)
{
    TbBool was_locked;
    was_locked = LbScreenIsLocked();
    if ( was_locked )
        LbScreenUnlock();
    LbScreenSwap();
    if ( was_locked )
    {
      while ( LbScreenLock() != Lb_SUCCESS )
        ;
    }
}

void
display_set_full_screen (bool full_screen)
{
    int i;

    if (lbScreenSurface != NULL)
        return;

    for (i = 1; i < LB_MAX_SCREEN_MODES_COUNT; ++i)
    {
        TbScreenModeInfo *mdinfo;

        mdinfo = LbScreenGetModeInfo(i);
        if (mdinfo->Width == 0) break;
        if (full_screen) {
            mdinfo->VideoMode &= ~Lb_VF_WINDOWED;
        } else {
            mdinfo->VideoMode |= Lb_VF_WINDOWED;
        }
    }
}

void display_set_lowres_stretch(bool stretch)
{
    if (stretch)
        LbScreenSetMinScreenSurfaceDimension(400);
    else
        LbScreenSetMinScreenSurfaceDimension(1);
}

void display_lock(void)
{
    if (!LbScreenIsLocked()) {
        while (LbScreenLock() != Lb_SUCCESS)
            ;
    }
}

void display_unlock(void)
{
    LbScreenUnlock();
}

void setup_simple_screen_mode(TbScreenMode mode)
{
    TbScreenModeInfo *mdinfo;
    short ratio;

    printf("%s %d\n", __func__, (int)mode);
    mdinfo = LbScreenGetModeInfo(mode);
    if (mdinfo->Width == 0) {
        LOGERR("Simple video mode %d is invalid", (int)mode);
        return;
    }
    LbScreenSetup(mode, mdinfo->Width, mdinfo->Height, display_palette);

    if (lbDisplay.GraphicsScreenHeight < 400)
        ratio = 2;
    else
        ratio = 1;
    LbMouseSetup(NULL, ratio, ratio);
}

void setup_screen_mode(TbScreenMode mode)
{
    TbBool was_locked;
    TbScreenModeInfo *mdinfo;
    short ratio;

    printf("%s %d\n", __func__, (int)mode);
    mdinfo = LbScreenGetModeInfo(mode);
    if (mdinfo->Width == 0) {
        LOGERR("Game video mode %d is invalid", (int)mode);
        mode = 1;
        mdinfo = LbScreenGetModeInfo(mode);
    }
    was_locked = LbScreenIsLocked();
    if (was_locked)
        LbScreenUnlock();
    if (LbScreenSetup(mode, mdinfo->Width, mdinfo->Height, display_palette) != 1)
        exit(1);
    if (was_locked) {
        while (LbScreenLock() != Lb_SUCCESS)
            ;
    }

    if (lbDisplay.GraphicsScreenHeight < 400)
        ratio = 2;
    else
        ratio = 1;
    LbMouseSetup(&pointer_sprites[1], ratio, ratio);

    setup_vecs(lbDisplay.WScreen, vec_tmap[0], lbDisplay.PhysicalScreenWidth,
        lbDisplay.PhysicalScreenWidth, lbDisplay.PhysicalScreenHeight);
}

void screen_save_backup_buffer(struct ScreenBufBkp *bkp)
{
    LbScreenStoreGraphicsWindow(&bkp->GWindow);
    bkp->WScreen = lbDisplay.WScreen;
    bkp->PhysicalScreenWidth = lbDisplay.GraphicsScreenWidth;
    bkp->PhysicalScreenHeight = lbDisplay.PhysicalScreenHeight;
    bkp->GraphicsScreenWidth = lbDisplay.GraphicsScreenWidth;
    bkp->GraphicsScreenHeight = lbDisplay.GraphicsScreenHeight;
}

void screen_switch_to_custom_buffer(struct ScreenBufBkp *bkp,
  TbPixel *buf, short width, short height)
{
    screen_save_backup_buffer(bkp);
    lbDisplay.WScreen = buf;
    lbDisplay.GraphicsScreenWidth = width;
    lbDisplay.PhysicalScreenHeight = height;
    lbDisplay.GraphicsScreenWidth = width;
    lbDisplay.GraphicsScreenHeight = height;
    LbScreenSetGraphicsWindow(0, 0, width, height);
}

void screen_load_backup_buffer(struct ScreenBufBkp *bkp)
{
    lbDisplay.WScreen = bkp->WScreen;
    lbDisplay.GraphicsScreenWidth = bkp->PhysicalScreenWidth;
    lbDisplay.PhysicalScreenHeight = bkp->PhysicalScreenHeight;
    lbDisplay.GraphicsScreenWidth = bkp->GraphicsScreenWidth;
    lbDisplay.GraphicsScreenHeight = bkp->GraphicsScreenHeight;
    LbScreenLoadGraphicsWindow(&bkp->GWindow);
}

void show_black_screen(void)
{
    TbBool was_locked;

    was_locked = LbScreenIsLocked();
    if (!was_locked) {
        while (LbScreenLock() != Lb_SUCCESS)
            ;
    }
    LbScreenClear(0);
    if (!was_locked)
        LbScreenUnlock();
    swap_wscreen();
}

void my_set_text_window(ushort x1, ushort y1, ushort w, ushort h)
{
    asm volatile (
      "call ASM_my_set_text_window\n"
        : : "a" (x1), "d" (y1), "b" (w), "c" (h));
}

ubyte font_height(uchar c)
{
    int ret;
    asm volatile ("call ASM_font_height\n"
        : "=r" (ret) : "a" (c));
    return ret;
}

ulong my_string_width(const char *text)
{
    int ret;
    asm volatile ("call ASM_my_string_width\n"
        : "=r" (ret) : "a" (text));
    return ret;
}
