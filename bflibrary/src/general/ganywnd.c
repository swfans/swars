/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file ganywnd.c
 *     AnyWindow related functions.
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
#include "bfanywnd.h"
#include "bfscreen.h"

struct TbAnyWindow lbTextJustifyWindow;
struct TbAnyWindow lbTextClipWindow;

TbResult LbScreenStoreGraphicsWindow(struct TbAnyWindow *wnd)
{
  wnd->x = lbDisplay.GraphicsWindowX;
  wnd->y = lbDisplay.GraphicsWindowY;
  wnd->width = lbDisplay.GraphicsWindowWidth;
  wnd->height = lbDisplay.GraphicsWindowHeight;
  wnd->ptr = NULL;
  return Lb_SUCCESS;
}

TbResult LbScreenLoadGraphicsWindow(struct TbAnyWindow *wnd)
{
  lbDisplay.GraphicsWindowX = wnd->x;
  lbDisplay.GraphicsWindowY = wnd->y;
  lbDisplay.GraphicsWindowWidth = wnd->width;
  lbDisplay.GraphicsWindowHeight = wnd->height;
  if (lbDisplay.WScreen != NULL)
  {
      lbDisplay.GraphicsWindowPtr = lbDisplay.WScreen
        + lbDisplay.GraphicsScreenWidth*lbDisplay.GraphicsWindowY + lbDisplay.GraphicsWindowX;
  } else
  {
      lbDisplay.GraphicsWindowPtr = NULL;
  }
  return Lb_SUCCESS;
}

TbResult LbTextSetJustifyWindow(long x, long y, ulong width)
{
    lbTextJustifyWindow.x = x;
    lbTextJustifyWindow.y = y;
    lbTextJustifyWindow.width = width;
    lbTextJustifyWindow.ptr = &lbDisplay.WScreen[x + y * lbDisplay.GraphicsScreenWidth];
    return Lb_SUCCESS;
}

TbResult LbTextSetClipWindow(long x, long y, ulong width, ulong height)
{
    long start_x,start_y;
    long end_x,end_y;
    long i;
    start_x = x;
    start_y = y;
    end_x = x + width;
    end_y = y + height;
    if (x > end_x)
    {
      i = x ^ end_x;
      start_x = i ^ x;
      end_x = i ^ x ^ i;
    }
    if ( y > end_y )
    {
      i = y ^ end_y;
      start_y = i ^ y;
      end_y = i ^ y ^ i;
    }
    if ( start_x < 0 )
        start_x = 0;
    if ( end_x < 0 )
      end_x = 0;
    if ( start_y < 0 )
        start_y = 0;
    if ( end_y < 0 )
      end_y = 0;
    if (start_x > lbDisplay.GraphicsScreenWidth)
        start_x = lbDisplay.GraphicsScreenWidth;
    if (end_x > lbDisplay.GraphicsScreenWidth)
      end_x = lbDisplay.GraphicsScreenWidth;
    if (start_y > lbDisplay.GraphicsScreenHeight)
        start_y = lbDisplay.GraphicsScreenHeight;
    if (end_y > lbDisplay.GraphicsScreenHeight)
      end_y = lbDisplay.GraphicsScreenHeight;
    lbTextClipWindow.x = start_x;
    lbTextClipWindow.y = start_y;
    lbTextClipWindow.width = end_x - start_x;
    lbTextClipWindow.height = end_y - start_y;
    lbTextClipWindow.ptr = &lbDisplay.WScreen[x + lbDisplay.GraphicsScreenWidth * y];
    return Lb_SUCCESS;
}

TbResult LbTextSetWindow(long x, long y, ulong width, ulong height)
{
    LbTextSetJustifyWindow(x, y, width);
    LbTextSetClipWindow(x, y, width, height);
    return Lb_SUCCESS;
}

/******************************************************************************/
