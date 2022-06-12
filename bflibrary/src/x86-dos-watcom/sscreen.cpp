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

#define CLIP_START_END_COORDS(cor1, cor2, corlimit) \
    if (cor1 > cor2) \
    { \
        if (cor1 < 0) \
            return Lb_FAIL; \
        if (cor2 > corlimit) \
            return Lb_FAIL; \
        if (cor2 < 0) \
            cor2 = 0; \
        if (cor1 > corlimit) \
            cor1 = corlimit; \
    } else \
    { \
        if (cor2 < 1) \
            return Lb_FAIL; \
        if (corlimit - 1 < cor1) \
            return Lb_FAIL; \
        if (cor1 < 0) \
            cor1 = 0; \
        if (cor2 > corlimit) \
            cor2 = corlimit; \
    }

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
    return dest;
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
    return dest;
}

TbResult LbScreenSetupAnyMode(TbScreenMode mode, TbScreenCoord width,
    TbScreenCoord height, ubyte *palette)
{
// code at 0001:000954e0
}

TbResult LbScreenClearGraphicsWindow(TbPixel colour)
{
    TbPixel *ptr;
    long h;

    ptr = lbDisplay.GraphicsWindowPtr;
    if (ptr == NULL)
        return Lb_FAIL;
    for (h = lbDisplay.GraphicsWindowHeight; h >= 0; h--)
    {
        LbMemorySet(ptr, colour, lbDisplay.GraphicsWindowWidth);
        ptr += lbDisplay.GraphicsScreenWidth;
    }
    return Lb_SUCCESS;
}

int LbScreenClear()
{
// code at 0001:00095728
}

TbResult LbScreenReset(void)
{
    if (!lbScreenInitialised)
      return Lb_FAIL;

    LbMouseSuspend();
    // here we should call proper interrupt to get back into lbDisplay.OldVideoMode
    // Mark as not initialized
    lbScreenInitialised = false;

    return Lb_SUCCESS;
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

TbResult LbScreenFindVideoModes(void)
{
    TbScreenModeInfo *mdinfo;
    int i;

    LbVesaGetInfo();
    for (i = 1; lbScreenModeInfo[i].Width; i++)
    {
        mdinfo = &lbScreenModeInfo[i];
        if ((mdinfo->VideoMode & Lb_VF_VESA) != 0)
            mdinfo->Available = LbVesaIsModeAvailable(mdinfo->VideoMode);
    }
    {
        mdinfo = &lbScreenModeInfo[1];
        mdinfo->Available = 1;
    }
    return Lb_SUCCESS;
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

TbResult LbScreenSwapBox(ubyte *sourceBuf, long sourceX, long sourceY,
  long destX, long destY, ulong width, ulong height)
{
    ushort h;
    ushort page1, page2;
    ubyte *destBuf;
    ulong outPos1, outPos2, pageAddr;

    LbMousePlace();
    outPos1 = lbDisplay.PhysicalScreenWidth * destY + destX;
    outPos2 = lbDisplay.PhysicalScreenWidth * destY + width + destX;
    inLnStart = sourceX + lbDisplay.GraphicsScreenWidth * sourceY;
    pageAddr = outPos1 & 0xFFFF0000;
    page1 = pageAddr >> 16;
    if (lbDisplay.VesaIsSetUp)
        LbVesaSetPage(page1);

    for (h = 0; h < height; h++)
    {
        pageAddr = outPos1 & 0xFFFF0000;
        page2 = pageAddr >> 16;
        if (page2 != page1)
            LbVesaSetPage(page2);
        pageAddr = outPos2 & 0xFFFF0000;
        page1 = pageAddr >> 16;
        if (page2 == page1)
        {
            destBuf = (ubyte *)0xA0000 + (outPos1 & 0xFFFF);
            qmemcpy(destBuf, &sourceBuf[inLnStart], width);
        } else
        {
            destBuf = (ubyte *)0xA0000;
            widthPart = 0x10000 - (outPos1 & 0xFFFF);
            qmemcpy(destBuf + (outPos1 & 0xFFFF), &sourceBuf[inLnStart], widthPart);
            LbVesaSetPage(page1);
            qmemcpy(destBuf, &sourceBuf[inLnStart + widthPart], width - widthPart);
        }
        outPos1 += lbDisplay.GraphicsScreenWidth;
        outPos2 += lbDisplay.PhysicalScreenWidth;
        inLnStart += lbDisplay.GraphicsScreenWidth;
    }
    LbMouseRemove();
    return Lb_SUCCESS;
}

static TbResult LbI_ScreenDrawHLineDirect(long X1, long Y1, long X2, long Y2)
{
    ubyte *ptr;
    ubyte *ptrEnd;
    long xBeg, xEnd;
    long width, shiftX, shiftY;

    if (X1 <= X2) {
        width = X2 - X1;
        shiftY = lbDisplay.GraphicsWindowWidth * Y1;
        shiftX = X1;
    } else {
        width = X1 - X2;
        shiftY = lbDisplay.GraphicsWindowWidth * Y1;
        shiftX = X2;
    }
    xBeg = shiftY + shiftX;
    xEnd = width + shiftY + shiftX;
    if (lbDisplay.VesaIsSetUp)
    {
        ulong page1, page2;

        page1 = xBeg >> 16;
        page2 = xEnd >> 16;
        if (page1 == page2)
        {
            LbVesaSetPage(page1);
            ptrEnd = &lbDisplay.PhysicalScreen[xEnd - (page2 << 16)];
            ptr = &lbDisplay.PhysicalScreen[xBeg - (page1 << 16)];
        }
        else
        {
            LbVesaSetPage(page1);
            ptrEnd = &lbDisplay.PhysicalScreen[0x10000];
            ptr = &lbDisplay.PhysicalScreen[xBeg - (page1 << 16)];
            do {
                *ptr += 128;
                ptr++;
            } while (ptr < ptrEnd);
            LbVesaSetPage(page2);
            ptr = &lbDisplay.PhysicalScreen[0];
            ptrEnd = &lbDisplay.PhysicalScreen[xEnd - (page2 << 16)];
        }
        do {
            *ptr += 128;
            ptr++;
        } while (ptr < ptrEnd);
    } else
    {
        ptrEnd = &lbDisplay.PhysicalScreen[xEnd];
        ptr = &lbDisplay.PhysicalScreen[xBeg];
        do {
            *ptr += 128;
            ptr++;
        } while (ptr < ptrEnd);
    }
}

static TbResult LbI_ScreenDrawVLineDirect(long X1, long Y1, long X2, long Y2)
{
    ubyte *ptr;
    ubyte *ptrEnd;
    long yBeg, yEnd;
    long height, shiftX, shiftY;
    long delta;

    if (Y1 <= Y2) {
        height = Y2 - Y1;
        shiftY = lbDisplay.GraphicsWindowWidth * Y1;
    } else {
        height = Y1 - Y2;
        shiftY = lbDisplay.GraphicsWindowWidth * Y2;
    }
    yBeg = shiftY + X1;
    yEnd = height * lbDisplay.GraphicsScreenWidth + yBeg;
    if (lbDisplay.VesaIsSetUp)
    {
        ulong page1, page2, i;
        ulong begFirstPg, endLastPg, endMidPg;
        ubyte *ptrPrev;

        page1 = yBeg >> 16;
        page2 = yEnd >> 16;
        ptrPrev = NULL;
        begFirstPg = &lbDisplay.PhysicalScreen[yBeg - (page1 << 16)];
        endLastPg = &lbDisplay.PhysicalScreen[yEnd - (page2 << 16)];
        endMidPg = &lbDisplay.PhysicalScreen[0x10000];
        if (page1 == page2)
            endMidPg = &lbDisplay.PhysicalScreen[yEnd - (page2 << 16)];
        for (i = page1; i <= page2; i++)
        {
            LbVesaSetPage(i);
            if (i == page1) {
                ptrEnd = endMidPg;
                ptr = begFirstPg;
            } else if (i != page2) {
                ptrEnd = endMidPg;
                ptr = ptrPrev - 0x10000;
            } else {
                ptrEnd = endLastPg;
                ptr = ptrPrev - 0x10000;
            }
            delta = lbDisplay.GraphicsScreenWidth;
            do {
                *ptr += 128;
                ptr += delta;
            } while (ptr < ptrEnd);
            ptrPrev = ptr;
        }
    } else
    {
        ptrEnd = &lbDisplay.PhysicalScreen[yEnd];
        ptr = &lbDisplay.PhysicalScreen[yBeg];
        delta = lbDisplay.GraphicsScreenWidth;
        do {
            *ptr += 128;
            ptr += delta;
        } while (ptr < ptrEnd);
    }
}

TbResult LbScreenDrawHVLineDirect(long X1, long Y1, long X2, long Y2)
{
    CLIP_START_END_COORDS(X1, X2, lbDisplay.GraphicsWindowWidth);
    CLIP_START_END_COORDS(Y1, Y2, lbDisplay.GraphicsWindowHeight);
    lbScreenDirectAccessActive = 1;
    if (X1 != X2)
    {
        LbI_ScreenDrawHLineDirect(X1, Y1, X2, Y2);
    } else
    {
        LbI_ScreenDrawVLineDirect(X1, Y1, X2, Y2);
    }
    lbScreenDirectAccessActive = 0;
    return Lb_SUCCESS;
}

TbResult LbScreenWaitVbi(void)
{
    ubyte flg;

    do
        flg = __inbyte(0x3DAu);
    while ( !(flg & 0x08) );
    return Lb_SUCCESS;
}


/******************************************************************************/
