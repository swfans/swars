/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file wrcities.c
 *     Routines for handling a list of cities for world map.
 * @par Purpose:
 *     Implement functions for parsing cities file and handling cities array.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 11 Sep 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "wrcities.h"

#include <string.h>
#include <stdlib.h>
#include "bffile.h"
#include "bfmemory.h"
#include "bfmemut.h"
#include "bfini.h"
#include "game.h"
#include "swlog.h"
/******************************************************************************/

void load_city_txt(void)
{
    asm volatile ("call ASM_load_city_txt\n"
        :  :  : "eax" );
}

void load_city_data(ubyte type)
{
    TbFileHandle fh;

    fh = LbFileOpen("data/cities.dat", Lb_FILE_MODE_READ_ONLY);
    if (fh == INVALID_FILE) {
        return;
    }
    LbFileRead(fh, &num_cities, 1);
    LbFileRead(fh, cities, sizeof(struct City) * num_cities);
    LbFileClose(fh);
}

/******************************************************************************/
