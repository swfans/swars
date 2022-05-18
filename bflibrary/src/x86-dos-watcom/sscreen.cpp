/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file sscreen.cpp
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
#include "bfscreen.h"

static inline void *LbI_XMemCopy(void *dest, void *source, ulong len)
{
        ulong remain;
        ubyte *s;
        ubyte *d;
        s = (ubyte *)source;
        d = (ubyte *)dest;
        for (remain = len >> 2; remain != 0; remain--)
        {
            *(ulong *)d = *(ulong *)s;
            d += 4;
            s += 4;
        }
}

static inline void *LbI_XMemCopyAndSet(void *dest, void *source, ulong val, ulong len)
{
        ulong remain;
        ubyte *s;
        ubyte *d;
        s = (ubyte *)source;
        d = (ubyte *)dest;
        for (remain = len >> 2; remain != 0; remain--)
        {
            *(ulong *)d = *(ulong *)s;
            *(ulong *)s = val;
            d += 4;
            s += 4;
        }
}

int LbScreenSetupAnyMode()
{
// code at 0001:000954e0
}

int LbScreenClearGraphicsWindow()
{
// code at 0001:000956dc
}

int LbScreenClear()
{
// code at 0001:00095728
}

int LbScreenReset()
{
// code at 0001:00095754
}

int LbScreenLock()
{
// code at 0001:000957a0
}

int LbScreenUnlock()
{
// code at 0001:000957ac
}

int LbScreenSetDoubleBuffering()
{
// code at 0001:000957b8
}

int LbScreenSetWScreenInVideo()
{
// code at 0001:000957d8
}

int lbScreenDirectAccessActive;

int LbScreenFindVideoModes()
{
// code at 0001:000957f8
}

TbResult LbScreenSwap(void)
{
    LbMousePlace();
    if (lbDisplay.VesaIsSetUp)
    {
        ulong block_id, blshift, blremain;
        ubyte *srcbuf;
        blremain = lbDisplay.GraphicsScreenHeight * lbDisplay.GraphicsScreenWidth;
        srcbuf = lbDisplay.WScreen;
        block_id = 0;
        while ( remain )
        {
          if ( remain >= 0x10000 )
            blshift = 0x10000;
          else
            blshift = remain;
          blremain -= blshift;
          LbVesaSetPage(block_id);
          LbI_XMemCopy(lbDisplay.PhysicalScreen, srcbuf, blshift);
          srcbuf += 0x10000;
          block_id++;
        }
    } else
    {
        ulong blremain;
        blremain = lbDisplay.GraphicsScreenHeight * lbDisplay.GraphicsScreenWidth;
        LbI_XMemCopy(lbDisplay.PhysicalScreen, lbDisplay.WScreen, blremain);
    }
    LbMouseRemove();
    return Lb_SUCCESS;
}

int LbScreenSwapBoxClear()
{
// code at 0001:00095964
}

TbResult LbScreenSwapClear(TbPixel colour)
{
    LbMousePlace();
    if (lbDisplay.VesaIsSetUp)
    {
        ulong block_id, pagelen, blremain;
        ubyte *blsrcbuf;
        blremain = lbDisplay.GraphicsScreenHeight * lbDisplay.GraphicsScreenWidth;
        blsrcbuf = lbDisplay.WScreen;
        for (block_id=0; blremain != 0; block_id++)
        {
            if (blremain >= 0x10000)
                pagelen = 0x10000;
            else
                pagelen = blremain;
            LbVesaSetPage(block_id);
            LbI_XMemCopyAndSet(lbDisplay.PhysicalScreen, blsrcbuf, 0x01010101 * colour, pagelen);
            blremain -= pagelen;
            blsrcbuf += 0x10000;
        }
    }
    else
    {
        int blsize;
        ubyte *blsrcbuf;
        blsrcbuf = lbDisplay.WScreen;
        blsize = lbDisplay.GraphicsScreenHeight * lbDisplay.GraphicsScreenWidth;
        LbI_XMemCopyAndSet(lbDisplay.PhysicalScreen, blsrcbuf, 0x01010101 * colour, blsize);
    }
    LbMouseRemove();
    return Lb_SUCCESS;
}

int LbScreenSwapBox()
{
// code at 0001:00095c38
}

int LbScreenDrawHVLineDirect()
{
// code at 0001:00095dc4
}

int LbScreenWaitVbi()
{
// code at 0001:000961b0
}


/******************************************************************************/
