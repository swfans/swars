/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file svesa.hpp
 *     Header file for svesa.cpp.
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
#ifndef BFLIBRARY_SVESA_H_
#define BFLIBRARY_SVESA_H_

#include "bftypes.h"
#include "bfscreen.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LB_VESA_DATA_SIZE 0x100

extern u8 *lbVesaData;

TbResult LbVesaGetGran(i32 mode);
TbResult LbVesaSetMode(i32 mode);
TbResult LbVesaSetPage(u16 pageNo);
TbResult LbVesaGetInfo(void);

/** Checks if a VESA mode with given flags is available.
 */
TbBool LbVesaIsModeAvailable(i32 mode);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_SVESA_H_
/******************************************************************************/
