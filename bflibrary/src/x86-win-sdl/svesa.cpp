/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file svesa.cpp
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
#include "svesa.h"

int LbVesaGetGran_UNUSED()
{
// code at 0001:000b2730
}

int LbVesaSetMode_UNUSED()
{
// code at 0001:000b2838
}

TbResult LbVesaSetPage(ushort pageNo)
{
    return Lb_FAIL;
}

int LbVesaGetInfo_UNUSED()
{
// code at 0001:000b2914
}

int LbVesaIsModeAvailable_UNUSED()
{
// code at 0001:000b29d8
}

int lbVesaBytesPerLine_UNUSED;

int lbVesaHRes_UNUSED;

int lbVesaVRes_UNUSED;

ubyte *lbVesaData;

int lbVesaPage_UNUSED;

int lbVesaGran_UNUSED;


/******************************************************************************/
