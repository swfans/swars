/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file mshandler.cpp
 *     Mouse handling master class.
 * @par Purpose:
 *     Provides mosuse state abstraction, including cursor support.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     16 Nov 2008 - 21 Nov 2009
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "mshandler.hpp"

#include "mspointer.hpp"
#include "bfscreen.h"
#include "bfsprite.h"
#include "bfmouse.h"
#include "privbflog.h"
/******************************************************************************/
// Global variables
class MouseStateHandler pointerHandler;

/******************************************************************************/
/**
 * Adjusts point coordinates.
 * @param x,y Coorditates to adjust.
 * @return Returns true if the coordinates have changed.
 */
int adjust_point(long *x, long *y)
{
    return false;
}

MouseStateHandler::MouseStateHandler(void)
{
    Release();
}

MouseStateHandler::~MouseStateHandler(void)
{
}

bool MouseStateHandler::Install(void)
{
    this->installed = true;
    mspos.x = lbDisplay.MMouseX;
    mspos.y = lbDisplay.MMouseY;
    return true;
}

bool MouseStateHandler::IsInstalled(void)
{
    LbSemaLock semlock(&semaphore,0);
    if (!semlock.Lock(true))
      return false;
    return true;
}

bool MouseStateHandler::Release(void)
{
    LbSemaLock semlock(&semaphore,0);
    if (!semlock.Lock(true))
      return false;
    lbMouseInstalled = false;
    lbDisplay.MouseSprite = NULL;
    this->installed = false;
    mssprite = NULL;
    pointer.Release();
    this->mspos.x = 0;
    this->mspos.y = 0;
    this->hotspot.x = 0;
    this->hotspot.y = 0;
    return true;
}

struct TbPoint *MouseStateHandler::GetPosition(void)
{
    if (!this->installed)
      return NULL;
    return &mspos;
}

bool MouseStateHandler::SetMousePosition(long x, long y)
{
    long mx,my;
    LbSemaLock semlock(&semaphore,0);
    if (!semlock.Lock(true))
      return false;
    if (!this->SetPosition(x, y))
      return false;
    if ( this->installed )
    {
      mx = mspos.x;
      my = mspos.y;
    } else
    {
      mx = x;
      my = y;
    }
    lbDisplay.MMouseX = mx;
    lbDisplay.MMouseY = my;
    return true;
}

bool MouseStateHandler::SetPosition(long x, long y)
{
    long prev_x,prev_y;
    long mx,my;
    if (!this->installed)
      return false;
    // Clip coordinates to our mouse window
    mx = x;
    if (x < lbDisplay.MouseWindowX)
    {
      mx = lbDisplay.MouseWindowX;
    } else
    if (x >= lbDisplay.MouseWindowX + lbDisplay.MouseWindowWidth)
    {
      mx = lbDisplay.MouseWindowWidth + lbDisplay.MouseWindowX - 1;
    }
    my = y;
    if (y < lbDisplay.MouseWindowY)
    {
      my = lbDisplay.MouseWindowY;
    } else
    if ( y >= lbDisplay.MouseWindowHeight + lbDisplay.MouseWindowY)
    {
      my = lbDisplay.MouseWindowHeight + lbDisplay.MouseWindowY - 1;
    }
    // If the coords are unchanged
    if ((mx == lbDisplay.MMouseX) && (my == lbDisplay.MMouseY))
      return true;
    //Change the position
    prev_x = mspos.x;
    mspos.x = mx;
    prev_y = mspos.y;
    mspos.y = my;
    if ((mssprite != NULL) && (this->installed))
    {
      if (!pointer.OnMove())
      {
        mspos.x = prev_x;
        mspos.y = prev_y;
        return false;
      }
    }
    return true;
}

bool MouseStateHandler::SetMouseWindow(long x, long y,long width, long height)
{
    LbSemaLock semlock(&semaphore,0);
    if (!semlock.Lock(true))
      return false;
    lbDisplay.MouseWindowX = x;
    lbDisplay.MouseWindowY = y;
    lbDisplay.MouseWindowWidth = width;
    lbDisplay.MouseWindowHeight = height;
    adjust_point(&lbDisplay.MMouseX, &lbDisplay.MMouseY);
    adjust_point(&lbDisplay.MouseX, &lbDisplay.MouseY);
    return true;
}

bool MouseStateHandler::GetMouseWindow(struct TbRect *windowRect)
{
    LbSemaLock semlock(&semaphore,0);
    if (!semlock.Lock(true))
      return false;
    windowRect->left = lbDisplay.MouseWindowX;
    windowRect->top = lbDisplay.MouseWindowY;
    windowRect->right = lbDisplay.MouseWindowX + lbDisplay.MouseWindowWidth;
    windowRect->bottom = lbDisplay.MouseWindowY + lbDisplay.MouseWindowHeight;
    return true;
}

const struct TbSprite *MouseStateHandler::GetPointer(void)
{
    if (!this->installed)
      return NULL;
    return mssprite;
}

bool MouseStateHandler::SetPointer(const struct TbSprite *spr, struct TbPoint *point)
{
    if (!this->installed)
      return false;
    mssprite = spr;
    this->hotspot.y = 0;
    this->hotspot.x = 0;
    if ((spr != NULL) && (spr->SWidth != 0) && (spr->SHeight != 0))
    {
      if (point != NULL)
      {
        this->hotspot.x = point->x;
        this->hotspot.y = point->y;
      }
      pointer.Initialise(spr, &this->mspos, &this->hotspot);
      if ((mssprite != NULL) && (this->installed))
      {
        pointer.OnMove();
      }
    } else
    {
      pointer.Release();
      mssprite = NULL;
    }
    return true;
}

bool MouseStateHandler::SetMousePointerAndOffset(const struct TbSprite *mouseSprite, long x, long y)
{
    struct TbPoint point;
    LbSemaLock semlock(&semaphore,0);
    if (!semlock.Lock(true))
      return false;
    if (mouseSprite == lbDisplay.MouseSprite)
      return true;
    if (mouseSprite != NULL)
      if ( (mouseSprite->SWidth > 64) || (mouseSprite->SHeight > 64) )
      {
        LOGERR("Mouse pointer too large");
        return false;
      }
    lbDisplay.MouseSprite = mouseSprite;
    point.x = x;
    point.y = y;
    return this->SetPointer(mouseSprite, &point);
}

bool MouseStateHandler::SetMousePointer(const struct TbSprite *mouseSprite)
{
    LbSemaLock semlock(&semaphore,0);
    if (!semlock.Lock(true))
      return false;
    if (mouseSprite == lbDisplay.MouseSprite)
      return true;
    if (mouseSprite != NULL)
      if ( (mouseSprite->SWidth > 64) || (mouseSprite->SHeight > 64) )
      {
        LOGERR("Mouse pointer too large");
        return false;
      }
    lbDisplay.MouseSprite = mouseSprite;
    this->SetPointer(mouseSprite, NULL);
    return true;
}

bool MouseStateHandler::SetPointerOffset(long x, long y)
{
    LbSemaLock semlock(&semaphore,0);
    if (!semlock.Lock(true))
      return false;
    // `pointer` object has reference to our `hotspot` attribute,
    // so we are asking pointer object to update it
    if (this->installed)
      pointer.SetHotspot(x, y);
    return true;
}

struct TbPoint *MouseStateHandler::GetPointerOffset(void)
{
    return &hotspot;
}

bool MouseStateHandler::PointerBeginSwap(void)
{
    LbSemaLock semlock(&semaphore,0);
    if (!semlock.Lock(true))
      return false;
    if ((!lbMouseInstalled) || (lbMouseOffline))
      return true;
    if ((mssprite != NULL) && (this->installed))
    {
      swap = 1;
      pointer.OnBeginSwap();
    }
    return true;
}

bool MouseStateHandler::PointerEndSwap(void)
{
    LbSemaLock semlock(&semaphore,1);
    if (!lbMouseInstalled)
      return true;
    if ((mssprite != NULL) && (this->installed))
    {
      if (swap)
      {
        swap = false;
        pointer.OnEndSwap();
      }
    }
    semlock.Release();
    return true;
}

bool MouseStateHandler::PointerBeginPartialUpdate(void)
{
    LbSemaLock semlock(&semaphore,0);
    if (!semlock.Lock(true))
      return false;
    if ((!lbMouseInstalled) || (lbMouseOffline))
      return true;
    if ((mssprite != NULL) && (this->installed))
    {
      swap = 1;
      pointer.OnBeginPartialUpdate();
    }
    return true;
}

bool MouseStateHandler::PointerEndPartialUpdate(void)
{
    LbSemaLock semlock(&semaphore,1);
    if (!lbMouseInstalled)
      return true;
    if ((mssprite != NULL) && (this->installed))
    {
      if (swap)
      {
        swap = false;
        pointer.OnEndPartialUpdate();
      }
    }
    semlock.Release();
    return true;
}

/******************************************************************************/
