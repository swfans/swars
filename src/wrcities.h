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
    /** Coordinate of the city on mission brief map. */
    short X;
    /** Coordinate of the city on mission brief map. */
    short Y;
    /** Map assigned to this city by last active mission. */
    u8 MapID;
    /** Level assigned to this city by last active mission. */
    u8 Level;
    u8 Flags;
    /** Amount of additional infos in brief screen (both unlocked by default
     * and by cash).
     */
    u8 Info;
    /** National text IDs: city name, then 5 additional properties. */
    u16 TextIndex[6];
    /** Map numbers which can be assigned to this city by active mission. */
    u8 MapsNo[3];
    /** Total cash reward expected from the active mission in this city.
     * Multiply x100 to get the value in credits.
     */
    u16 CreditReward;
    u8 Dummy2[15];
};

#pragma pack()
/******************************************************************************/
extern u8 num_cities;
extern struct City cities[];
extern i16 city_id;

void save_cities_conf_file(void);
void read_cities_conf_file(void);
void load_city_txt(void);
void load_city_data(u8 type);

void activate_cities(u8 brief);
void recount_city_credit_reward(u8 city);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
