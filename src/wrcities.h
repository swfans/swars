/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file wrcities.h
 *     Header file for wrcities.c.
 * @par Purpose:
 *     Routines for handling a list of cities for world map.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 11 Sep 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef WRCITIES_H
#define WRCITIES_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct City { // sizeof=40
    short X;
    short Y;
    ubyte MapID;
    ubyte Level;
    ubyte Flags;
    char Info;
    ushort TextIndex[6];
    long Dummy2[5];
};

#pragma pack()
/******************************************************************************/
extern ubyte num_cities;
extern struct City cities[];
extern sbyte city_id;

void save_cities_conf_file(void);
void read_cities_conf_file(void);
void load_city_txt(void);
void load_city_data(ubyte type);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
