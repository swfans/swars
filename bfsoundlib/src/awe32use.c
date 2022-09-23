/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file awe32use.c
 *     Reimplementation of Bullfrog Sound Library.
 * @par Purpose:
 *     Routines use of AWE32 specific features.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Jun 2022 - 05 Sep 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "awe32.h"
#include "dpmi.h"
#include "aildebug.h"
/******************************************************************************/
extern SF_DATA *awe_data;
extern uint16_t awe_data_seg;

/******************************************************************************/

int AWEFreeMem(MDI_DRIVER *mdidrv, short bank_no)
{
  VDI_CALL regs;

  FreeDOSmem(awe_data, awe_data_seg);
  awe_data = NULL;
  awe_data_seg = 0;

  regs.CX = AWESF_FREEBANK;
  regs.DX = bank_no;
  AIL_call_driver(mdidrv->drvr, MDI_VSE, &regs, &regs);
  return regs.AX;
}

/******************************************************************************/
