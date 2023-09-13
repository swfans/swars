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

void save_city_single_conf(TbFileHandle fh, struct City *p_city, char *buf)
{
    {
        sprintf(buf, "Name = %s\n", memload + p_city->TextIndex[0]);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if (p_city->TextIndex[0] != 0) {
        sprintf(buf, "TextId = %hu\n", p_city->TextIndex[0]);
        LbFileWrite(fh, buf, strlen(buf));
    }
    {
        sprintf(buf, "PropTextId = %hu %hu %hu %hu %hu\n",
          (ushort)p_city->TextIndex[1], (ushort)p_city->TextIndex[2],
          (ushort)p_city->TextIndex[3], (ushort)p_city->TextIndex[4], (ushort)p_city->TextIndex[5]);
        LbFileWrite(fh, buf, strlen(buf));
    }
    {
        sprintf(buf, "Coord = %hd %hd\n", p_city->X, p_city->Y);
        LbFileWrite(fh, buf, strlen(buf));
    }
    {
        sprintf(buf, "MapNo = %hu\n", (ushort)p_city->MapID);
        LbFileWrite(fh, buf, strlen(buf));
    }
    {
        sprintf(buf, "LevelNo = %hu\n", (ushort)p_city->Level);
        LbFileWrite(fh, buf, strlen(buf));
    }
    {
        sprintf(buf, "Flags = %hu\n", (ushort)p_city->Flags);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if (p_city->Info) {
        sprintf(buf, "Info = %hu\n", (ushort)p_city->Info);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if ((p_city->Dummy2[0]|p_city->Dummy2[1]|p_city->Dummy2[2]) != 0) {
        sprintf(buf, "Dummy2 = %d %d %d %d %d\n",
          (int)p_city->Dummy2[0], (int)p_city->Dummy2[1], (int)p_city->Dummy2[2],
          (int)p_city->Dummy2[3], (int)p_city->Dummy2[4]);
        LbFileWrite(fh, buf, strlen(buf));
    }
}

void save_cities_conf_file(void)
{
    TbFileHandle fh;
    char locbuf[320];
    char conf_fname[80];
    int i;

    sprintf(conf_fname, "%s" FS_SEP_STR "cities.ini", "conf");
    fh = LbFileOpen(conf_fname, Lb_FILE_MODE_NEW);
    if (fh == INVALID_FILE) {
        LOGERR("Could not create '%s' file.", conf_fname);
        return;
    }

    {
        sprintf(locbuf, "[common]\n");
        LbFileWrite(fh, locbuf, strlen(locbuf));

        sprintf(locbuf, "CityCount = %hu\n", num_cities);
        LbFileWrite(fh, locbuf, strlen(locbuf));

        sprintf(locbuf, "\n");
        LbFileWrite(fh, locbuf, strlen(locbuf));
    }

    for (i = 0; i < num_cities; i++) {
        struct City *p_city;

        p_city = &cities[i];

        sprintf(locbuf, "[city%d]\n", i);
        LbFileWrite(fh, locbuf, strlen(locbuf));

        save_city_single_conf(fh, p_city, locbuf);

        sprintf(locbuf, "\n");
        LbFileWrite(fh, locbuf, strlen(locbuf));
    }
    LbFileClose(fh);
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
