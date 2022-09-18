/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file mssxmidi.c
 *     OpenAL based reimplementation of MSS API.
 * @par Purpose:
 *     SS functions from MSS API.
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

#include "mssxmidi.h"
/******************************************************************************/

MDI_DRIVER *AIL2OAL_API_install_MDI_driver_file(char *filename, SNDCARD_IO_PARMS *iop)
{
    MDI_DRIVER *ret;
    asm volatile (
      "push %2\n"
      "push %1\n"
      "call ASM_AIL_API_install_MDI_driver_file\n"
        : "=r" (ret) : "g" (filename), "g" (iop));
    return ret;
}

/******************************************************************************/
