/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file lvfiles.h
 *     Header file for lvfiles.c.
 * @par Purpose:
 *     Routines for level and map files handling.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 May 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef LVFILES_H
#define LVFILES_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct QuickLoad {
    ushort *Numb;
    void **Ptr;
    ushort Size;
    ushort Extra;
};

#pragma pack()
/******************************************************************************/

TbResult load_mad_pc(ushort mapno);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
