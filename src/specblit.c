/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file specblit.c
 *     Special graphics data copy (blit) routines.
 * @par Purpose:
 *     Implement functions for copying memory buffers with quirks used
 *     to perform some special blits of graphics data.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     22 Apr 2023 - 22 Oct 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "specblit.h"

#include "bfmemut.h"
#include "swlog.h"
/******************************************************************************/

void * memory_copy_with_skip(void *in_dst, const void *in_src, TbMemSize size, ubyte bskip)
{
    const ubyte *s;
    ubyte *d;
    d = in_dst;
    s = in_src;
    for (; size > 0; size--)
    {
        if (*s != bskip)
            *d = *s;
        s++;
        d++;
    }
    return in_dst;
}

/** Copy given rect buffer to the same position at two larger buffers.
 */
void copy_buffer_to_double_bufs(ubyte *ibuf, ushort iwidth, ushort iheight,
    ubyte *obufs[2], short x, short y, ushort owidth, ushort oheight)
{
    long pos;
    short h;

    pos = y * owidth + x;
    for (h = 0; h < iheight; h++)
    {
        LbMemoryCopy(&obufs[0][pos], ibuf, iwidth);
        LbMemoryCopy(&obufs[1][pos], ibuf, iwidth);
        ibuf += iwidth;
        pos += owidth;
    }
}

/** Copy given rect buffer to the same position at two larger buffers, skipping
 * pixels with transparency color.
 */
void copy_buffer_to_double_bufs_with_trans(ubyte *ibuf, ushort iwidth, ushort iheight,
    ubyte *obufs[2], short x, short y, ushort owidth, ushort oheight, ubyte trans_col)
{
    long pos;
    short h;

    pos = y * owidth + x;
    for (h = 0; h < iheight; h++)
    {
        memory_copy_with_skip(&obufs[0][pos], ibuf, iwidth, trans_col);
        memory_copy_with_skip(&obufs[1][pos], ibuf, iwidth, trans_col);
        ibuf += iwidth;
        pos += owidth;
    }
}


/******************************************************************************/
