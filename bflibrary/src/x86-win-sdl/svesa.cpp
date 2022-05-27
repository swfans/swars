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

ubyte *lbVesaData;

TbResult LbVesaGetGran(long mode)
{
    return Lb_FAIL;
}

TbResult LbVesaSetMode(long mode)
{
    return Lb_FAIL;
}

TbResult LbVesaSetPage(ushort pageNo)
{
    return Lb_FAIL;
}

TbResult LbVesaGetInfo(void)
{
    return Lb_FAIL;
}

TbBool LbVesaIsModeAvailable(long mode)
{
    /* low level access to VESA modes not available with SDL */
    return false;
}

/******************************************************************************/
