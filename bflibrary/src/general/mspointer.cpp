/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file mspointer.cpp
 *     Graphics drawing of mouse pointer class.
 * @par Purpose:
 *     Allows drawing and removing mouse pointer sprite.
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
#include <cstring>
#include "mspointer.hpp"

#include "bfscreen.h"
#include "bfsprite.h"
#include "bfmouse.h"
#include "bflog.h"
/******************************************************************************/

extern "C" {
extern volatile TbBool lbPointerAdvancedDraw;
long PointerDraw(long x, long y, const struct TbSprite *spr, TbPixel *outbuf,
  unsigned long scanline);
}

// Methods

LbI_PointerHandler::LbI_PointerHandler(void)
{
    LbScreenSurfaceInit(&surf1);
    LbScreenSurfaceInit(&surf2);
    this->field_1050 = false;
    this->field_1054 = false;
    this->sprite = NULL;
    this->position = NULL;
    this->spr_offset = NULL;
    draw_pos_x = 0;
    draw_pos_y = 0;
}

LbI_PointerHandler::~LbI_PointerHandler(void)
{
    Release();
}

void LbI_PointerHandler::SetHotspot(long x, long y)
{
    long prev_x,prev_y;
    LbSemaLock semlock(&sema_rel,0);
    semlock.Lock(true);
    if (this->field_1050)
    {
        // Set new coords, and backup previous ones
        prev_x = spr_offset->x;
        spr_offset->x = x;
        prev_y = spr_offset->y;
        spr_offset->y = y;
        ClipHotspot();
        // If the coords were changed, then update the pointer
        if ((spr_offset->x != prev_x) || (spr_offset->y != prev_y))
        {
            Undraw(true);
            NewMousePos();
            Backup(true);
            Draw(true);
        }
    }
}

void LbI_PointerHandler::ClipHotspot(void)
{
    if (!this->field_1050)
        return;
    if ((sprite != NULL) && (spr_offset != NULL))
    {
        if (spr_offset->x < 0)
        {
          spr_offset->x = 0;
        } else
        if (sprite->SWidth <= spr_offset->x)
        {
          spr_offset->x = sprite->SWidth - 1;
        }
        if (spr_offset->y < 0)
        {
          spr_offset->y = 0;
        } else
        if (spr_offset->y >= sprite->SHeight)
        {
          spr_offset->y = sprite->SHeight - 1;
        }
    }
}

void LbI_PointerHandler::Initialise(const struct TbSprite *spr,
  struct TbPoint *npos, struct TbPoint *noffset)
{
    void *surfbuf;
    TbPixel *buf;
    TbResult ret;
    long i;
    int dstwidth, dstheight;
    unsigned long flags;

    Release();
    LbSemaLock semlock(&sema_rel,0);
    semlock.Lock(true);
    sprite = spr;
    dstwidth = (sprite->SWidth * lbUnitsPerPixel + 16) / 16;
    dstheight = (sprite->SHeight * lbUnitsPerPixel + 16) / 16;
    ret = LbScreenSurfaceCreate(&surf1, dstwidth, dstheight);
    if (ret != Lb_SUCCESS) {
        sprite = NULL;
        return;
    }
    ret = LbScreenSurfaceCreate(&surf2, dstwidth, dstheight);
    if (ret != Lb_SUCCESS) {
        LbScreenSurfaceRelease(&surf1);
        sprite = NULL;
        return;
    }
    surfbuf = LbScreenSurfaceLock(&surf1);
    if (surfbuf == NULL)
    {
        LbScreenSurfaceRelease(&surf1);
        LbScreenSurfaceRelease(&surf2);
        sprite = NULL;
        return;
    }
    buf = (TbPixel *)surfbuf;
    for (i=0; i < dstheight; i++)
    {
        memset(buf, 255, surf1.pitch);
        buf += surf1.pitch;
    }
    PointerDraw(0, 0, this->sprite, (TbPixel *)surfbuf, surf1.pitch);
    LbScreenSurfaceUnlock(&surf1);
    this->position = npos;
    this->spr_offset = noffset;
    ClipHotspot();
    this->field_1050 = true;
    NewMousePos();
    this->field_1054 = false;
    flags = SSBlt_FLAG10 | SSBlt_FLAG2;
    LbScreenSurfaceBlit(&surf2, this->draw_pos_x, this->draw_pos_y, &rect_1038, flags);
}

void LbI_PointerHandler::Draw(bool a1)
{
    unsigned long flags;
    flags = SSBlt_FLAG10 | SSBlt_FLAG8 | SSBlt_FLAG4;
    if ( a1 )
      flags |= SSBlt_FLAG2;
    LbScreenSurfaceBlit(&this->surf1, this->draw_pos_x, this->draw_pos_y, &rect_1038, flags);
}

void LbI_PointerHandler::Backup(bool a1)
{
    unsigned long flags;
    flags = SSBlt_FLAG10;
    if ( a1 )
      flags |= SSBlt_FLAG2;
    this->field_1054 = false;
    LbScreenSurfaceBlit(&this->surf2, this->draw_pos_x, this->draw_pos_y, &rect_1038, flags);
}

void LbI_PointerHandler::Undraw(bool a1)
{
    unsigned long flags;
    flags = SSBlt_FLAG10 | SSBlt_FLAG8;
    if ( a1 )
      flags |= SSBlt_FLAG2;
    LbScreenSurfaceBlit(&this->surf2, this->draw_pos_x, this->draw_pos_y, &rect_1038, flags);
}

void LbI_PointerHandler::Release(void)
{
    LbSemaLock semlock(&sema_rel,0);
    semlock.Lock(true);
    if ( this->field_1050 )
    {
        if ( lbInteruptMouse )
            Undraw(true);
        this->field_1050 = false;
        this->field_1054 = false;
        position = NULL;
        sprite = NULL;
        spr_offset = NULL;
        LbScreenSurfaceRelease(&surf1);
        LbScreenSurfaceRelease(&surf2);
    }
}

void LbI_PointerHandler::NewMousePos(void)
{
    this->draw_pos_x = position->x - spr_offset->x * lbUnitsPerPixel / 16;
    this->draw_pos_y = position->y - spr_offset->y * lbUnitsPerPixel / 16;
    int dstwidth, dstheight;
    dstwidth = sprite->SWidth * lbUnitsPerPixel / 16;
    dstheight = sprite->SHeight * lbUnitsPerPixel / 16;
    LbSetRect(&rect_1038, 0, 0, dstwidth, dstheight);
    if (this->draw_pos_x < 0)
    {
        rect_1038.left -= this->draw_pos_x;
        this->draw_pos_x = 0;
    } else
    if (this->draw_pos_x + dstwidth > lbDisplay.PhysicalScreenWidth)
    {
        rect_1038.right += lbDisplay.PhysicalScreenWidth - dstwidth-this->draw_pos_x;
    }
    if (this->draw_pos_y < 0)
    {
        rect_1038.top -= this->draw_pos_y;
        this->draw_pos_y = 0;
    } else
    if (this->draw_pos_y+dstheight > lbDisplay.PhysicalScreenHeight)
    {
        rect_1038.bottom += lbDisplay.PhysicalScreenHeight - dstheight - this->draw_pos_y;
    }
}

bool LbI_PointerHandler::OnMove(void)
{
    LbSemaLock semlock(&sema_rel,0);
    if (!semlock.Lock(true))
        return false;
    if (lbPointerAdvancedDraw && lbInteruptMouse)
    {
        Undraw(true);
        NewMousePos();
        Backup(true);
        Draw(true);
    } else
    {
        NewMousePos();
    }
    return true;
}

void LbI_PointerHandler::OnBeginPartialUpdate(void)
{
    LbSemaLock semlock(&sema_rel,0);
    if (!semlock.Lock(true))
        return;
    Backup(false);
    Draw(false);
}

void LbI_PointerHandler::OnEndPartialUpdate(void)
{
    LbSemaLock semlock(&sema_rel,1);
    Undraw(false);
    this->field_1054 = true;
    semlock.Release();
}

void LbI_PointerHandler::OnBeginSwap(void)
{
    LbSemaLock semlock(&sema_rel,0);
    if (!semlock.Lock(true))
      return;
    if ( lbPointerAdvancedDraw )
    {
        Backup(false);
        Draw(false);
    } else
    if (LbScreenLock() == Lb_SUCCESS)
    {
      PointerDraw(position->x - spr_offset->x * lbUnitsPerPixel / 16,
          position->y - spr_offset->y * lbUnitsPerPixel / 16,
          sprite, lbDisplay.WScreen, lbDisplay.GraphicsScreenWidth);
      LbScreenUnlock();
    }
}

void LbI_PointerHandler::OnEndSwap(void)
{
    LbSemaLock semlock(&sema_rel,1);
    if ( lbPointerAdvancedDraw )
    {
        Undraw(false);
        this->field_1054 = true;
    }
    semlock.Release();
}

void LbI_PointerHandler::OnBeginFlip(void)
{
    LbSemaLock semlock(&sema_rel,0);
    if (!semlock.Lock(true))
        return;
    Backup(false);
    Draw(false);
}

void LbI_PointerHandler::OnEndFlip(void)
{
    LbSemaLock semlock(&sema_rel,1);
    semlock.Release();
}

/******************************************************************************/
