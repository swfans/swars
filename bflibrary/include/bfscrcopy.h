/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfscrcopy.h
 *     Header file for scr_copy.c.
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
#ifndef BFLIBRARY_SCR_COPY_H_
#define BFLIBRARY_SCR_COPY_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Copies a rectangle between buffers of size equal to GraphicsScreen.
 *
 * Given two buffers with sizes equal to GraphicsScreen, and a rectangle,
 * definition, it blits that rectangle between these buffers.
 * If a single buffer is used for both source and dest, the rectanlges
 * should not overlap.
 */
void LbScreenCopyBox(TbPixel *sourceBuf, TbPixel *destBuf,
  long sourceX, long sourceY, long destX, long destY,
  ulong width, ulong height);

/** Copies buffer of size equal to GraphicsWindow to one of GraphicsScreen.
 *
 * Given source buffer with width equal to GraphicsWindow, and destination
 * buffer with width equal to GraphicsScreen, and the height,
 * it copies given amount of lines of one buffer to the other.
 *
 * @param sourceBuf The GraphicsWindow sized buffer to copy from
 * @param destBuf The GraphicsScreen sized buffer to copy into
 * @param height Amount of lines to copy (the GraphicsWindow height is ignored
 *     in favour of this value)
 */
void LbScreenCopy(TbPixel *sourceBuf, TbPixel *destBuf, ushort height);

/** Saves part of buffer of size equal to GraphicsScreen to one of GraphicsWindow.
 *
 * This function does a similar but opposite copy operation to LbScreenCopy().
 * Given source buffer with width equal to GraphicsScreen, and destination
 * buffer with width equal to GraphicsWindow, and the height,
 * it copies given amount of lines of one buffer to the other.
 *
 * @param sourceBuf The GraphicsScreen sized buffer to copy from
 * @param destBuf The GraphicsWindow sized buffer to copy into
 * @param height Amount of lines to copy (the GraphicsWindow height is ignored
 *     in favour of this value)
 */
void LbScreenSave(TbPixel *sourceBuf, TbPixel *destBuf, ushort height);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_SCR_COPY_H_
/******************************************************************************/
