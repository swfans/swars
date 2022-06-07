/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file mspointer.hpp
 *     Internal header file for mspointer.cpp.
 * @par Purpose:
 *     Graphics drawing support sdk class.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     16 Nov 2008 - 21 Nov 2009
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIBRARY_MSPOINTER_H_
#define BFLIBRARY_MSPOINTER_H_

#include "bfsemphr.hpp"
#include "bfscrsurf.h"
#include "bfplanar.h"
/******************************************************************************/

struct TbSprite;

class LbI_PointerHandler {
 public:
    LbI_PointerHandler(void);
    ~LbI_PointerHandler(void);

    /** Sets place within the pointer sprite with is actually pointed.
     *
     * Stores coords of the pointing spot within the sprite.
     * Allows only values up to one pixel around dimensions of current sprite.
     * This means you should set the sprite first, then the hotspot.
     * Coordinates (0,0) are one pixel before top left corner of the sprite;
     * increasing values go through dimensions of the sprite, to one pixel after.
     * If the cursor is placed during this call, it will be properly redrawn.
     */
    void SetHotspot(long x, long y);

    /** Initialize the pointer state with given structures.
     *
     * Stores pointers received in parameters as internal state
     * in regard to current pointer, mouse position, and pointer hotspot.
     * None of the structs are copied - the pointers are set directly as
     * internal state.
     */
    void Initialise(const struct TbSprite *spr, struct TbPoint *,
      struct TbPoint *);

    void Release(void);
    void NewMousePos(void);
    bool OnMove(void);
    void OnBeginPartialUpdate(void);
    void OnEndPartialUpdate(void);
    void OnBeginSwap(void);
    void OnEndSwap(void);
    void OnBeginFlip(void);
    void OnEndFlip(void);
 protected:
    void ClipHotspot(void);
    void Draw(bool);
    void Undraw(bool);
    void Backup(bool);
    // Properties
    struct SSurface surf1;
    struct SSurface surf2;
    struct TbPoint *position;
    struct TbPoint *spr_offset;
    struct TbRect spr_clip_rect;
    long draw_pos_x;
    long draw_pos_y;
    bool initialised;
    bool field_1054;
    const struct TbSprite *sprite;
    LbSemaphore sema_rel;
    };

extern long cursor_xsteps_array[];
extern long cursor_ysteps_array[];

#endif // BFLIBRARY_MSPOINTER_H_
/******************************************************************************/
