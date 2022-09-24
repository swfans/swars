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
#include <string.h>
#include <assert.h>

#include "mssxmidi.h"
/******************************************************************************/
extern char GTL_prefix[128];

void AIL2OAL_API_set_GTL_filename_prefix(char const *prefix)
{
    int i;

    strcpy(GTL_prefix,prefix);

    // Truncate prefix string at final '.' character, if it exists and
    // if it occurs after the last directory separator
    for (i=strlen(GTL_prefix)-1; i > 0; i--)
    {
        if ((GTL_prefix[i] == '\\') || (GTL_prefix[i] == '/'))
        {
            if (i == (int)strlen(GTL_prefix)-1) {
                GTL_prefix[i] = '\0';
            }
            break;
        }

        if (GTL_prefix[i] == '.') {
            GTL_prefix[i] = '\0';
            break;
        }
    }
}

MDI_DRIVER *AIL2OAL_API_install_MDI_driver_file(char *filename, SNDCARD_IO_PARMS *iop)
{
    MDI_DRIVER *mdidrv;
    asm volatile (
      "push %2\n"
      "push %1\n"
      "call ASM_AIL_API_install_MDI_driver_file\n"
        : "=r" (mdidrv) : "g" (filename), "g" (iop));
    return mdidrv;
}

void AIL2OAL_API_uninstall_MDI_driver(MDI_DRIVER *mdidrv)
{
    asm volatile (
      "push %0\n"
      "call ASM_AIL_API_uninstall_MDI_driver\n"
        :  : "g" (mdidrv));
}

/******************************************************************************/
