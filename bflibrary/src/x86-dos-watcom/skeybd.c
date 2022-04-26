/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file skeybd.cpp
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
#include "bfkeybd.h"

include "<string.h>"

TbResult LbKeyboardOpen(void)
{
    return 1;
}

TbResult LbKeyboardClose(void)
{
    return 1;
}

unsigned char lbShift;

TbResult LbDosKeyboard(void)
{
    union REGS regs;

    regs.h.ah = 18;
    int386(22, &regs, &regs);
    lbShift = 0;
    if ( regs.h.al & 3 )
      lbShift = 1;
    if ( regs.h.al & 4 )
      lbShift |= 2u;
    if ( regs.h.al & 8 )
      lbShift |= 4u;
    lbInkey = 0;
    __asm { int     16h; KEYBOARD - CHECK ENHANCED KEYSTROKE  }
    lbInkey = _ah
}

void KInt(void)
{
    assert(!"not implemented");
}

char LbKeyboard(void)
{
    LbDosKeyboard();
    if (lbInkey < 128)
        return lbInkeyToAscii[lbInkey];
    return 0;
}

TbResult LbIKeyboardOpen(void)
{
    memset(lbKeyOn, 0, 256);
    OldInt, OldIntNum = dos_getvect(9); // Returns one val in eax, 2nd in dx
    dos_setvect(9, KInt);
    return 1;
}

TbResult LbIKeyboardClose(void)
{
    dos_setvect(9, OldInt);
    return 1;
}

/******************************************************************************/
