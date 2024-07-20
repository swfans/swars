/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file sscrsurf.c
 *     Graphics surfaces support.
 * @par Purpose:
 *     Surfaces used for drawing on screen or compositing layers.
 * @par Comment:
 *     Depends on the video support library, which is SDL in this implementation.
 * @author   Tomasz Lis
 * @date     10 Feb 2010 - 30 Sep 2010
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <SDL.h>
#include "bfscrsurf.h"

#include "bfplanar.h"
#include "bfscreen.h"
#include "privbflog.h"

/******************************************************************************/
#define to_SDLSurf(h) ((SDL_Surface *)h)

OSSurfaceHandle lbScreenSurface;
OSSurfaceHandle lbDrawSurface;

extern SDL_Color lbPaletteColors[256];

TbResult LbIScreenDrawSurfaceCheck(void);
/******************************************************************************/
void LbScreenSurfaceInit(struct SSurface *surf)
{
    surf->surf_data = NULL;
    surf->pitch = 0;
    surf->locks_count = 0;
}

TbResult LbScreenSurfaceCreate(struct SSurface *surf, ulong w, ulong h)
{
    const SDL_PixelFormat * format;

    if (lbDrawSurface == NULL) {
        LOGERR("DrawSurface pixel format must be known to create further surfaces.");
        return Lb_FAIL;
    }
    format = to_SDLSurf(lbDrawSurface)->format;

    surf->surf_data = (OSSurfaceHandle)SDL_CreateRGBSurface(0,
      w, h, format->BitsPerPixel,
      format->Rmask, format->Gmask, format->Bmask, format->Amask);

    if (surf->surf_data == NULL) {
        LOGERR("Failed to create surface.");
        return Lb_FAIL;
    }
    surf->locks_count = 0;
    surf->pitch = to_SDLSurf(surf->surf_data)->pitch;

    // moved color key control to blt_surface()

    return Lb_SUCCESS;
}

TbResult LbScreenSurfaceRelease(struct SSurface *surf)
{
    if (surf->surf_data == NULL) {
        return Lb_FAIL;
    }

    SDL_FreeSurface(to_SDLSurf(surf->surf_data));
    surf->surf_data = NULL;

    return Lb_SUCCESS;
}

TbResult LbScreenSurfaceBlit(struct SSurface *surf, ulong x, ulong y,
    struct TbRect *rect, ulong blflags)
{
    // Convert TbRect to SDL rectangles
    SDL_Rect srcRect;
    SDL_Rect destRect;
    Uint32 clkey;

    LbIScreenDrawSurfaceCheck();

    if (lbDrawSurface == NULL) {
        LOGERR("DrawSurface pixel format must be known to blit other surfaces.");
        return Lb_FAIL;
    }

    srcRect.x = rect->left;
    srcRect.y = rect->top;
    srcRect.w = rect->right - rect->left;
    srcRect.h = rect->bottom - rect->top;

    destRect.x = x;
    destRect.y = y;
    destRect.w = srcRect.w;
    destRect.h = srcRect.h;

    // Set blit parameters

    if ((blflags & SSBlt_FLAG2) != 0) {
      //TODO: see how/if to handle this, I interpret this as "blit directly to primary rather than back"
      //secSurf = surface3;
      //I think it can simply be deleted as not even the mouse pointer code is using it and there's no way
      //to access front buffer in SDL
    }

    if ((blflags & SSBlt_FLAG4) != 0)
    {
        if (to_SDLSurf(surf->surf_data)->format->BitsPerPixel == 8) {
            // here we know we want to use a specific color index as key, no need for mapping
            clkey = 255;
        } else {
            clkey = SDL_MapRGB(to_SDLSurf(surf->surf_data)->format, 0x0, 0xff, 0xff);
        }
        // enable color key
        SDL_SetColorKey(to_SDLSurf(surf->surf_data), SDL_TRUE, clkey);
        if (SDL_HasColorKey(to_SDLSurf(surf->surf_data)) == -1)
            LOGWARN("DrawSurface refused to enable color key; no transparency.");
    }
    else
    {
        // disable color key
        SDL_SetColorKey(to_SDLSurf(surf->surf_data), SDL_FALSE, 0);
    }

    if ((blflags & SSBlt_FLAG10) != 0) {
        //TODO: see if this can/should be handled
        //probably it can just be deleted
        //dwTrans |= DDBLTFAST_WAIT;
    }

    // SDL has a per-surface palette for 8 bit surfaces. But the engine assumes palette
    // to be required only for screen surface. To make off-screen surface working,
    // we must manually set the palette for it. So temporarily change palette.
    SDL_Palette * paletteBackup = NULL;
    if (to_SDLSurf(surf->surf_data)->format->BitsPerPixel == 8) {
        paletteBackup = to_SDLSurf(surf->surf_data)->format->palette;
        to_SDLSurf(surf->surf_data)->format->palette = to_SDLSurf(lbDrawSurface)->format->palette;
    }

    int blresult;
    // the blit
    if ((blflags & SSBlt_FLAG8) != 0) {
        // surface to screen
        blresult = SDL_BlitSurface(to_SDLSurf(surf->surf_data),
          &srcRect, to_SDLSurf(lbDrawSurface), &destRect);
    }
    else {
        // screen to surface
        blresult = SDL_BlitSurface(to_SDLSurf(lbDrawSurface),
          &destRect, to_SDLSurf(surf->surf_data), &srcRect);
    }

    // restore palette
    if (to_SDLSurf(surf->surf_data)->format->BitsPerPixel == 8) {
        to_SDLSurf(surf->surf_data)->format->palette = paletteBackup;
    }

    if (blresult == -1) {
        // Blitting mouse cursor will occasionally fail, so there's no point in logging this
        LOGERR("Blit failed: %s", SDL_GetError());
        return Lb_FAIL;
    }
    return Lb_SUCCESS;
}

void *LbScreenSurfaceLock(struct SSurface *surf)
{
    if (surf->surf_data == NULL) {
        return NULL;
    }

    if (SDL_LockSurface(to_SDLSurf(surf->surf_data)) < 0) {
        LOGERR("Failed to lock surface: %s", SDL_GetError());
        return NULL;
    }

    surf->locks_count++;
    surf->pitch = to_SDLSurf(surf->surf_data)->pitch;
    return to_SDLSurf(surf->surf_data)->pixels;
}

TbResult LbScreenSurfaceUnlock(struct SSurface *surf)
{
    if (surf->locks_count == 0) {
        return Lb_SUCCESS;
    }
    if (surf->surf_data == NULL) {
        return Lb_FAIL;
    }
    SDL_UnlockSurface(to_SDLSurf(surf->surf_data));
    surf->locks_count--;
    return Lb_SUCCESS;
}
/******************************************************************************/
