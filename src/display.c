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
#include "util.h"
#include "bflib_basics.h"

#pragma pack(1)

#define to_SDLSurf(h) ((SDL_Surface  *)h)

#pragma pack()

extern char lbDrawAreaTitle[128];
extern ushort data_1aa330;
extern ushort data_1aa332;
extern ubyte *vec_tmap;
extern unsigned char *display_palette;
extern struct TbSprite *pointer_sprites;
extern long lbPhysicalResolutionMul;

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
display_update (void)
{
    assert(lbScreenSurface != NULL);
    // Stretched lowres in action?
    if ((lbPhysicalResolutionMul > 1) && lbHasSecondSurface)
    {
        if (SDL_MUSTLOCK (to_SDLSurf(lbScreenSurface))) {
            if (SDL_LockSurface (to_SDLSurf(lbScreenSurface)) != 0) {
                fprintf (stderr, "SDL_LockSurface: %s\n", SDL_GetError());
                exit(1);
            }
        }
        // Stretch lowres
        long i, j;
        long mdWidth, mdHeight;
        ubyte *poutput = (ubyte *)to_SDLSurf(lbScreenSurface)->pixels;
        ubyte *pinput  = (ubyte *)to_SDLSurf(lbDrawSurface)->pixels;

        mdWidth = lbDisplay.PhysicalScreenWidth;
        mdHeight = lbDisplay.PhysicalScreenHeight;

        for (j = 0; j < mdHeight; j++)
        {
            for (i = 0; i < mdWidth; i++)
            {
                long di, dj;
                int input_xy = j * mdWidth + i;

                for (dj = 0; dj < lbPhysicalResolutionMul; dj++)
                {
                    int output_xy = (j*lbPhysicalResolutionMul+dj) *
                      mdWidth*lbPhysicalResolutionMul +
                      i*lbPhysicalResolutionMul;

                    for (di = 0; di < lbPhysicalResolutionMul; di++) {
                        poutput[output_xy++] = pinput[input_xy];
                    }
                }
            }
        }

        if (SDL_MUSTLOCK (to_SDLSurf(lbScreenSurface))) {
            SDL_UnlockSurface (to_SDLSurf(lbScreenSurface));
        }
    }

    SDL_Flip (to_SDLSurf(lbScreenSurface));
}

void
display_set_full_screen (bool full_screen)
{
    TbScreenModeInfo *mdinfo;

    if (lbScreenSurface != NULL)
        return;

    if (full_screen) {
        mdinfo = LbScreenGetModeInfo(1);
        mdinfo->VideoMode &= ~Lb_VF_WINDOWED;
        mdinfo = LbScreenGetModeInfo(13);
        mdinfo->VideoMode &= ~Lb_VF_WINDOWED;
    } else {
        mdinfo = LbScreenGetModeInfo(1);
        mdinfo->VideoMode |= Lb_VF_WINDOWED;
        mdinfo = LbScreenGetModeInfo(13);
        mdinfo->VideoMode |= Lb_VF_WINDOWED;
    }
}

void
display_set_lowres_stretch (bool stretch)
{
  if (stretch)
      LbScreenSetMinPhysicalScreenResolution(400);
  else
      LbScreenSetMinPhysicalScreenResolution(1);
}

void
display_lock (void)
{
}

void
display_unlock (void)
{
}

void setup_vecs(ubyte *screenbuf, ubyte *vec_tmap, ulong width3, ulong width4, long height)
{
    asm volatile (
      "push %4\n"
      "call ASM_setup_vecs_\n"
        : : "a" (screenbuf), "d" (vec_tmap), "b" (width3), "c" (width4), "g" (height));
}


void setup_screen_mode(TbScreenMode mode)
{
    TbBool was_locked;

    printf("setup_screen_mode %d\n", (int)mode);
    switch (mode)
    {
    case 1:
        data_1aa330 = 320;
        data_1aa332 = 200;
        break;
    case 13:
        data_1aa330 = 640;
        data_1aa332 = 480;
        break;
    case 16:
        data_1aa330 = 800;
        data_1aa332 = 600;
        break;
    default:
        mode = 1;
        data_1aa330 = 320;
        data_1aa332 = 200;
        break;
    }
    was_locked = LbScreenIsLocked();
    if (was_locked)
        LbScreenUnlock();
    if (LbScreenSetupAnyMode(mode, data_1aa330, data_1aa332, display_palette) != 1)
        exit(1);
    if (was_locked) {
        while (LbScreenLock() != Lb_SUCCESS)
            ;
    }

    if (mode == 1)
        lbDisplay.ScreenMode = 1;
    if (mode == 13)
        lbDisplay.ScreenMode = 13;

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
