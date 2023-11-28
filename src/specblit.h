/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file specblit.h
 *     Header file for specblit.c.
 * @par Purpose:
 *     Special graphics data copy (blit) routines.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     22 Apr 2023 - 22 Oct 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef SPECBLIT_H
#define SPECBLIT_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

void copy_buffer_to_double_bufs(ubyte *ibuf, ushort iwidth, ushort iheight,
    ubyte *obufs[2], short x, short y, ushort owidth, ushort oheight);

void copy_buffer_to_double_bufs_with_trans(ubyte *ibuf, ushort iwidth, ushort iheight,
    ubyte *obufs[2], short x, short y, ushort owidth, ushort oheight, ubyte trans_col);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
