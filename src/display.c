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
#include "bflib_basics.h"

extern ushort data_1aa330;
extern ushort data_1aa332;
extern ubyte *vec_tmap;
extern unsigned char *display_palette;
extern struct TbSprite *pointer_sprites;

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

void
display_set_lowres_stretch (bool stretch)
{
  if (stretch)
      LbScreenSetMinScreenSurfaceDimension(400);
  else
      LbScreenSetMinScreenSurfaceDimension(1);
}

void
display_lock (void)
{
  LbScreenLock();
}

void
display_unlock (void)
{
  LbScreenUnlock();
}

void setup_screen_mode(TbScreenMode mode)
{
    TbBool was_locked;
    TbScreenModeInfo *mdinfo;

    printf("%s %d\n", __func__, (int)mode);
    mdinfo = LbScreenGetModeInfo(mode);
    if (mdinfo->Width == 0) {
        mode = 1;
        mdinfo = LbScreenGetModeInfo(mode);
    }
    data_1aa330 = mdinfo->Width;
    data_1aa332 = mdinfo->Height;
    was_locked = LbScreenIsLocked();
    if (was_locked)
        LbScreenUnlock();
    if (LbScreenSetupAnyMode(mode, data_1aa330, data_1aa332, display_palette) != 1)
        exit(1);
    if (was_locked) {
        while (LbScreenLock() != Lb_SUCCESS)
            ;
    }

    LbMouseSetup(&pointer_sprites[1], 2, 2);
    setup_vecs(lbDisplay.WScreen, vec_tmap, lbDisplay.PhysicalScreenWidth,
        lbDisplay.PhysicalScreenWidth, lbDisplay.PhysicalScreenHeight);
}

void show_black_screen(void)
{
    TbBool was_locked;

    was_locked = LbScreenIsLocked();
    if (!was_locked) {
        while (LbScreenLock() != Lb_SUCCESS)
            ;
    }
    memset(lbDisplay.WScreen, 0, lbDisplay.PhysicalScreenHeight * lbDisplay.PhysicalScreenWidth);
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
