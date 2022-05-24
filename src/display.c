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
#include "util.h"
#include "bflib_basics.h"

#pragma pack(1)

#define to_SDLSurf(h) ((SDL_Surface  *)h)

extern TbScreenModeInfo lbScreenModeInfo[];

#pragma pack()

extern char lbDrawAreaTitle[128];
extern ushort data_1aa330;
extern ushort data_1aa332;
extern ubyte *vec_tmap;
extern unsigned char *display_palette;
extern struct TbSprite *pointer_sprites;

static bool         display_lowres_stretch = false;

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

static inline void
lock_screen (void)
{
    if (SDL_MUSTLOCK (to_SDLSurf(lbScreenSurface))) {
        if (SDL_LockSurface (to_SDLSurf(lbScreenSurface)) != 0) {
            fprintf (stderr, "SDL_LockSurface: %s\n", SDL_GetError ());
            exit(1);
        }
    }
    // set vga buffer address
    lbDisplay.PhysicalScreen = to_SDLSurf(lbDrawSurface)->pixels;
}

static inline void
unlock_screen (void)
{
    if (SDL_MUSTLOCK (to_SDLSurf(lbScreenSurface))) {
        SDL_UnlockSurface (to_SDLSurf(lbScreenSurface));
    }
    lbDisplay.PhysicalScreen = NULL;
}

int LbScreenSetupAnyModeTweaked(unsigned short mode, unsigned long width,
    unsigned long height, TbPixel *palette)
{
    ubyte *wscreen_bak;

    wscreen_bak = lbDisplay.WScreen;

    uint32_t flags;
    TbScreenModeInfo *mdinfo;

    // OLD: should we really suspend/resume the mouse here?
    long hot_x, hot_y;
    const struct TbSprite *msspr;

    msspr = NULL;
    if (lbDisplay.MouseSprite != NULL)
    {
        msspr = lbDisplay.MouseSprite;
        LbMouseGetSpriteOffset(&hot_x, &hot_y);
    }
    LbMouseChangeSprite(NULL);

    if (lbDrawSurface != NULL)
        unlock_screen ();

    if (lbHasSecondSurface)
        SDL_UnlockSurface (to_SDLSurf(lbDrawSurface));

  // lbDisplay.OldVideoMode which is DWORD 1E2EB6 is used in
  // 000ED764 sub_ED764 to probably get back to text mode
  // I'm setting it to 0xFF for now
  if (!lbDisplay.OldVideoMode)
    lbDisplay.OldVideoMode = 0xFF;

  // We are using only lbDisplay.VesaIsSetUp = false

  /*  if(lbScreenModeInfo[mode].mode < 256)
  {
    lbDisplay.VesaIsSetUp = false;
  }
  else
  {
    // lbDisplay.VesaIsSetUp = true;
    lbDisplay.VesaIsSetUp = false; // Defaulting to false
  }*/
  lbDisplay.VesaIsSetUp = false;

  // Setting mode
    if (lbHasSecondSurface) {
        SDL_FreeSurface (to_SDLSurf(lbDrawSurface));
        lbHasSecondSurface = false;
    }

  flags = SDL_SWSURFACE;

    mdinfo = LbScreenGetModeInfo(mode);
    if ((mdinfo->VideoMode & Lb_VF_WINDOWED) == 0)
        flags |= SDL_FULLSCREEN;


    // Stretch lowres ?
    if (width == 320 && height == 200 && display_lowres_stretch)
      mdinfo = LbScreenGetModeInfo(13);

    // Init mode
    lbScreenSurface = lbDrawSurface =
          SDL_SetVideoMode(mdinfo->Width, mdinfo->Height,
 		    mdinfo->BitsPerPixel, flags);

    mdinfo = LbScreenGetModeInfo(mode);

#ifdef DEBUG
  printf ("SDL_SetVideoMode(%ld, %ld, %d, SDL_SWSURFACE) - %s\n",
          width, height, (int)lbScreenModeInfo[mode].BitsPerPixel,
          lbScreenModeInfo[mode].Desc);
#endif

  if (lbScreenSurface == NULL)
    {
      fprintf (stderr, "SDL_SetVideoMode: %s\n", SDL_GetError ());
      goto err;
    }

    SDL_WM_SetCaption(lbDrawAreaTitle, lbDrawAreaTitle);
    LbScreenUpdateIcon();

    // Create secondary surface if necessary, that is if BPP != lbEngineBPP.
    if (width == 320 && height == 200 && display_lowres_stretch)
    {
        lbDrawSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 8, 0, 0, 0, 0);
        if (lbDrawSurface == NULL) {
            printf("Cannot create secondary surface: %s\n", SDL_GetError());
            LbScreenReset();
            return Lb_FAIL;
        }
        lbHasSecondSurface = true;

         SDL_LockSurface(to_SDLSurf(lbDrawSurface));
    }

    lbDisplay.DrawFlags = 0;
    lbDisplay.DrawColour = 0;
    lbDisplay.PhysicalScreenWidth = mdinfo->Width;
    lbDisplay.PhysicalScreenHeight = mdinfo->Height;
    lbDisplay.ScreenMode = mode;

    lbDisplay.PhysicalScreen = NULL;
    //lbDisplay.WScreen = NULL;
    lbDisplay.GraphicsWindowPtr = NULL;

  // Is that better? chack in disassembly
  lbDisplay.GraphicsScreenWidth  = width;
  lbDisplay.GraphicsScreenHeight = height;

    lbScreenInitialised = true;
    printf("Mode %dx%dx%d setup succeeded", (int)to_SDLSurf(lbScreenSurface)->w, (int)to_SDLSurf(lbScreenSurface)->h,
      (int)to_SDLSurf(lbScreenSurface)->format->BitsPerPixel);

  // Setup palette
  if (palette != NULL)
    {
        if (LbPaletteSet(palette) != 1)
            goto err;
    }

  // Call funcitons that recalculate some buffers
  // They can be switched to C++ later, but it's not needed
  LbScreenSetGraphicsWindow(0, 0, lbDisplay.GraphicsScreenWidth, lbDisplay.GraphicsScreenHeight);
  LbTextSetWindow(0, 0, lbDisplay.GraphicsScreenWidth, lbDisplay.GraphicsScreenHeight);

  lbScreenInitialised = true;

    lbDisplay.WScreen = wscreen_bak;
    lock_screen ();

  return 1;

err:
  if (lbDrawSurface != NULL)
    {
      unlock_screen ();
      SDL_FreeSurface (to_SDLSurf(lbDrawSurface));
      lbDrawSurface = NULL;
    }

  lbScreenInitialised = false;

  return -1;
}

TbResult LbScreenSetup(TbScreenMode mode, TbScreenCoord width, TbScreenCoord height,
    unsigned char *palette)
{
    return LbScreenSetupAnyModeTweaked(mode, width, height, palette);
}

void swap_wscreen(void)
{
    TbBool has_wscreeen;
    has_wscreeen = (lbDisplay.WScreen != 0);
    if ( has_wscreeen )
        LbScreenUnlock();
    LbScreenSwap();
    if ( has_wscreeen )
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
  if (lbHasSecondSurface)
    {
      // Stretch lowres
      int i, j;
      unsigned char *poutput = (unsigned char*) to_SDLSurf(lbScreenSurface)->pixels;
      unsigned char *pinput  = to_SDLSurf(lbDrawSurface)->pixels;

      for (j = 0; j < 480; j++)
        {
          for (i = 0; i < 640; i+=2)
            {
              // Do not touch this formula
              int input_xy = ((j * 200) / 480) * 320 + i / 2;
              int output_xy = j * 640 + i;

              poutput[output_xy]     = pinput[input_xy];
              poutput[output_xy + 1] = pinput[input_xy];
            }
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
display_get_size (size_t *width, size_t *height)
{
  if (lbScreenSurface == NULL)
    {
      if (width != NULL)
        *width  = 0;

      if (height != NULL)
        *height = 0;

      return;
    }

  if (width != NULL)
    *width  = lbDisplay.GraphicsScreenWidth;

  if (height != NULL)
    *height = lbDisplay.GraphicsScreenHeight;
}

void
display_get_physical_size (size_t *width, size_t *height)
{
  if (lbScreenSurface == NULL)
    {
      if (width != NULL)
        *width  = 0;

      if (height != NULL)
        *height = 0;

      return;
    }

  if (width != NULL)
    *width  = to_SDLSurf(lbScreenSurface)->w;

  if (height != NULL)
    *height = to_SDLSurf(lbScreenSurface)->h;
}

void
display_set_lowres_stretch (bool stretch)
{
  display_lowres_stretch = stretch;
}

bool
display_is_stretching_enabled (void)
{
  return lbHasSecondSurface;
}

void
display_lock (void)
{
  lock_screen ();
}

void
display_unlock (void)
{
  unlock_screen ();
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
    was_locked = lbDisplay.WScreen != NULL;
    if (was_locked)
        LbScreenUnlock();
    if (LbScreenSetupAnyModeTweaked(mode, data_1aa330, data_1aa332, display_palette) != 1)
        exit(1);
    if (was_locked)
    {
        while (LbScreenLock() != 1)
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
