/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file smouse.cpp
 *     Implementation of related functions.
 * @par Purpose:
 *     Unknown.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <stdbool.h>
#include <SDL/SDL.h>
#include "bfmouse.h"

#include "mshandler.hpp"
#include "bfscreen.h"
#include "bfsprite.h"
#include "bfplanar.h"
#include "privbflog.h"


extern "C" {

#define AUTORESET_MIN_SHIFT 50

};

TbResult LbMousePlace(void)
{
    if (!lbMouseInstalled)
        return Lb_FAIL;

    if (!pointerHandler.PointerBeginPartialUpdate())
        return Lb_FAIL;

    return Lb_SUCCESS;
}

TbResult LbMouseRemove(void)
{
    if (!lbMouseInstalled)
        return Lb_FAIL;

    if (!pointerHandler.PointerEndPartialUpdate())
        return Lb_FAIL;

    return Lb_SUCCESS;
}


TbResult LbMouseOnBeginSwap(void)
{
    if (!pointerHandler.PointerBeginSwap())
        return Lb_FAIL;
    return Lb_SUCCESS;
}

TbResult LbMouseOnEndSwap(void)
{
    if (!pointerHandler.PointerEndSwap())
        return Lb_FAIL;
    return Lb_SUCCESS;
}

TbResult LbMouseChangeSpriteOffset(long hot_x, long hot_y)
{
    if (!lbMouseInstalled)
        return Lb_FAIL;
    LOGDBG("setting hs (%ld,%ld)", hot_x, hot_y);

    if (!pointerHandler.SetPointerOffset(-hot_x, -hot_y))
        return Lb_FAIL;

    return Lb_SUCCESS;
}

TbResult LbMouseGetSpriteOffset(long *hot_x, long *hot_y)
{
    struct TbPoint *hotspot;

    hotspot = pointerHandler.GetPointerOffset();
    if (hotspot == NULL)
        return Lb_FAIL;
    *hot_x = -hotspot->x;
    *hot_y = -hotspot->y;

    return Lb_SUCCESS;
}

TbResult LbMouseChangeSprite(const struct TbSprite *pointer_spr)
{
    if (!lbMouseInstalled)
        return Lb_FAIL;

    if (pointer_spr == NULL)
        LOGDBG("setting to %s", "NONE");
    else
        LOGDBG("setting to %dx%d, data at %p", (int)pointer_spr->SWidth,
          (int)pointer_spr->SHeight, pointer_spr);

    if (!pointerHandler.SetMousePointer(pointer_spr))
        return Lb_FAIL;

    return Lb_SUCCESS;
}

TbResult LbMouseChangeMoveRatio(long ratio_x, long ratio_y)
{
    if ((ratio_x < -8192) || (ratio_x > 8192) || (ratio_x == 0))
        return Lb_FAIL;
    if ((ratio_y < -8192) || (ratio_y > 8192) || (ratio_y == 0))
        return Lb_FAIL;

    LOGSYNC("new ratio %ldx%ld", ratio_x, ratio_y);

#if defined(ENABLE_MOUSE_MOVE_RATIO)
    lbDisplay.MouseMoveRatioX = ratio_x;
    lbDisplay.MouseMoveRatioY = ratio_y;
#endif

    return Lb_SUCCESS;
}

TbBool LbMouseIsInstalled(void)
{
    if (!lbMouseInstalled)
        return false;

    if (!pointerHandler.IsInstalled())
        return false;

    return true;
}

TbResult LbMouseSetup(const struct TbSprite *pointer_spr, int ratio_x, int ratio_y)
{
    long x,y;

    if (lbMouseInstalled)
        LbMouseSuspend();
    // Make sure the pointer sprite gets updated, even if address stays unchanged
    lbDisplay.MouseSprite = NULL;

    pointerHandler.Install();

#if 0
    minfo.XSpriteOffset = 0;
    minfo.YSpriteOffset = 0;
    minfo.XMoveRatio = 1;
    minfo.YMoveRatio = 1;
    memset(minfo.Sprite, 254, 0x1000u);
    redraw_active_lock = 0;
    memset(&mbuffer, 0, 0x1020u);
#endif

    lbMouseOffline = true;
    lbMouseInstalled = true;

    if ( LbMouseSetWindow(0, 0, lbDisplay.GraphicsScreenWidth, lbDisplay.GraphicsScreenHeight) != Lb_SUCCESS )
    {
        LOGERR("could not set mouse window, size (%d,%d)",
          (int)lbDisplay.GraphicsScreenWidth, (int)lbDisplay.GraphicsScreenHeight);
        lbMouseInstalled = false;
        return Lb_FAIL;
    }
    y = lbDisplay.MouseWindowY + lbDisplay.MouseWindowHeight / 2;
    x = lbDisplay.MouseWindowX + lbDisplay.MouseWindowWidth / 2;
    if ( LbMouseChangeMoveRatio(ratio_x, ratio_y) != Lb_SUCCESS )
    {
        LOGERR("could not change move ratio to (%d,%d)", ratio_x, ratio_y);
        lbMouseInstalled = false;
        return Lb_FAIL;
    }
    if ( LbMouseSetPosition(x, y) != Lb_SUCCESS )
    {
        LOGERR("could not set position to (%d,%d)", x, y);
        lbMouseInstalled = false;
        return Lb_FAIL;
    }
    if ( LbMouseChangeSprite(pointer_spr) != Lb_SUCCESS )
    {
        LOGERR("could not change sprite");
        lbMouseInstalled = false;
        return Lb_FAIL;
    }
    lbMouseOffline = false;
    return Lb_SUCCESS;
}

TbResult LbMouseReset(void)
{
    return LbMouseSuspend();
}

TbResult LbMouseSuspend(void)
{
    if (!lbMouseInstalled)
        return Lb_FAIL;

    if (!pointerHandler.Release())
        return Lb_FAIL;

    return Lb_SUCCESS;
}

TbResult LbMouseSetWindow(long x, long y, long width, long height)
{
    if (!lbMouseInstalled)
        return Lb_FAIL;

    if (!pointerHandler.SetMouseWindow(x, y, width, height))
        return Lb_FAIL;

    return Lb_SUCCESS;
}

TbResult LbMouseSetPosition(long x, long y)
{
    if (!lbMouseInstalled)
        return Lb_FAIL;

    if (!pointerHandler.SetMousePosition(x, y))
        return Lb_FAIL;

    return Lb_SUCCESS;
}

extern "C" {
TbResult LbMouseOnMove(struct TbPoint pos);
}

TbResult LbMouseOnMove(struct TbPoint pos)
{
    if ((!lbMouseInstalled) || (lbMouseOffline))
        return Lb_FAIL;

    if (!pointerHandler.SetMousePosition(pos.x, pos.y))
        return Lb_FAIL;

    return Lb_SUCCESS;
}

void MouseToScreen(struct TbPoint *pos)
{
    // Static variables for storing last mouse coordinated; needed
    // because lbDisplay.MMouseX/MMouseY coords are scaled
    static long mx = 0;
    static long my = 0;
    struct TbRect clip;
    struct TbPoint orig;
    if (lbMouseAutoReset)
    {
      if (!pointerHandler.GetMouseWindow(&clip))
          return;
      orig.x = pos->x;
      orig.y = pos->y;
#if defined(ENABLE_MOUSE_MOVE_RATIO)
      pos->x = mx + ((pos->x - mx) * (long)lbDisplay.MouseMoveRatioX)/256;
      pos->y = mx + ((pos->y - my) * (long)lbDisplay.MouseMoveRatioY)/256;
#endif
      mx = orig.x;
      my = orig.y;
      if ((mx < clip.left + AUTORESET_MIN_SHIFT)
       || (mx > clip.right - AUTORESET_MIN_SHIFT)
       || (my < clip.top + AUTORESET_MIN_SHIFT)
       || (my > clip.bottom - AUTORESET_MIN_SHIFT))
      {
          mx = (clip.right - clip.left) / 2 + clip.left;
          my = (clip.bottom - clip.top) / 2 + clip.top;
          SDL_WarpMouse(mx, my);
      }
    } else
    {
      orig.x = pos->x;
      orig.y = pos->y;
#if defined(ENABLE_MOUSE_MOVE_RATIO)
      pos->x = mx + ((pos->x - mx) * (long)lbDisplay.MouseMoveRatioX)/256;
      pos->y = mx + ((pos->y - my) * (long)lbDisplay.MouseMoveRatioY)/256;
#endif
      mx = orig.x;
      my = orig.y;
    }

    if (lbScreenSurfaceDimensions.Width != lbDisplay.GraphicsScreenWidth)
        pos->x = (pos->x * lbDisplay.GraphicsScreenWidth) /
            lbScreenSurfaceDimensions.Width;
    if (lbScreenSurfaceDimensions.Height != lbDisplay.GraphicsScreenHeight)
        pos->y = (pos->y * lbDisplay.GraphicsScreenHeight) /
            lbScreenSurfaceDimensions.Height;
    LOGNO("before (%ld,%ld) after (%ld,%ld)", orig.x, orig.y, pos->x, pos->y);
}

/**
 * Converts an SDL mouse button event and button state to platform-independent action.
 * @param eventType SDL event type.
 * @param button SDL button definition.
 * @return
 */
static TbMouseAction MouseButtonActionsMapping(int eventType, const SDL_MouseButtonEvent * button)
{
    if (eventType == SDL_MOUSEBUTTONDOWN) {
        switch (button->button)  {
        case SDL_BUTTON_LEFT: return MActn_LBUTTONDOWN;
        case SDL_BUTTON_MIDDLE: return MActn_MBUTTONDOWN;
        case SDL_BUTTON_RIGHT: return MActn_RBUTTONDOWN;
        case SDL_BUTTON_WHEELUP: return MActn_WHEELMOVEUP;
        case SDL_BUTTON_WHEELDOWN: return MActn_WHEELMOVEDOWN;
        }
    }
    else if (eventType == SDL_MOUSEBUTTONUP) {
        switch (button->button) {
        case SDL_BUTTON_LEFT: return MActn_LBUTTONUP;
        case SDL_BUTTON_MIDDLE: return MActn_MBUTTONUP;
        case SDL_BUTTON_RIGHT: return MActn_RBUTTONUP;
        case SDL_BUTTON_WHEELUP: return MActn_NONE;
        case SDL_BUTTON_WHEELDOWN: return MActn_NONE;
        }
    }
    LOGWARN("unidentified event, type %d button %d", eventType, (int)button->button);
    return MActn_NONE;
}

extern "C" {
TbResult MEvent(const SDL_Event *ev);
};

/** @internal
 * Triggers mouse control function for given SDL mouse event.
 * @return SUCCESS if the event was processed, FAIL if key isn't supported, OK if no mouse event.
 */
TbResult MEvent(const SDL_Event *ev)
{
    TbMouseAction action;
    struct TbPoint pos;
    TbResult ret;

    switch (ev->type)
    {
    case SDL_MOUSEMOTION:
        action = MActn_MOUSEMOVE;
        pos.x = ev->motion.x;
        pos.y = ev->motion.y;
        ret = mouseControl(action, &pos);
        return ret;

    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEBUTTONDOWN:
        action = MouseButtonActionsMapping(ev->type, &ev->button);
        pos.x = ev->button.x;
        pos.y = ev->button.y;
        ret = mouseControl(action, &pos);
        return ret;
    }
    return Lb_OK;
}

/******************************************************************************/
