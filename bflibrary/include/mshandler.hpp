/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file mshandler.hpp
 *     Internal header file for mshandler.cpp.
 * @par Purpose:
 *     Mouse handler class for storing internal mouse state.
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
#ifndef BFLIBRARY_MSHANDLER_H_
#define BFLIBRARY_MSHANDLER_H_

#include "mspointer.hpp"
#include "bfsemphr.hpp"
/******************************************************************************/
struct TbSprite;
struct TbRect;

class MouseStateHandler {
 public:
    MouseStateHandler(void);
    virtual ~MouseStateHandler(void);
    bool Install(void);
    bool IsInstalled(void);
    bool Release(void);
    struct TbPoint *GetPosition(void);
    bool SetMousePosition(long x, long y);
    bool SetPosition(long x, long y);
    const struct TbSprite *GetPointer(void);
    bool SetMousePointerAndOffset(const struct TbSprite *mouseSprite, long x, long y);
    bool SetMousePointer(const struct TbSprite *mouseSprite);
    bool SetPointerOffset(long x, long y);
    struct TbPoint *GetPointerOffset(void);
    bool SetMouseWindow(long x, long y, long width, long height);
    bool GetMouseWindow(struct TbRect *windowRect);
    bool PointerBeginSwap(void);
    bool PointerEndSwap(void);
 protected:
    bool SetPointer(const struct TbSprite *spr, struct TbPoint *pt);
    // Properties
    LbSemaphore semaphore;
    bool installed;
    const struct TbSprite *mssprite;
    struct TbPoint mspos;
    struct TbPoint hotspot;
    class LbI_PointerHandler pointer;
    bool swap;
};

/******************************************************************************/
extern class MouseStateHandler pointerHandler;
/******************************************************************************/

#endif // BFLIBRARY_MSHANDLER_H_
