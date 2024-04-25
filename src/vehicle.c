/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file vehicle.c
 *     Routines implementing vehicle.
 * @par Purpose:
 *     Implement functions for handling vehicle things.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 12 Jun 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "vehicle.h"

#include "thing.h"
#include "swlog.h"
/******************************************************************************/

#pragma pack(1)

struct CarGlare { // sizeof=7
    short Dix;
    short Diy;
    short Diz;
    ubyte Flag;
};

#pragma pack()

struct CarGlare car_glare[] = {
  {0, 16, 336, 0},
  {-96, -64, 400, 0},
  {128, -64, 400, 0},
  {0, -96, 304, 0},
  {-208, -32, -240, 0},
  {208, -32, -240, 0},
  {128, -144, 240, 0},
  {-112, -144, 240, 0},
  {96, -32, 304, 0},
  {-80, -32, 304, 0},
  {96, -96, 528, 0},
  {-80, -96, 528, 0},
  {-128, -16, 288, 1},
  {144, -16, 288, 2},
  {112, -112, -96, 2},
  {-96, -112, -96, 1},
  {-80, -368, 48, 1},
  {96, -368, 48, 2},
  {-32, -64, 160, 0},
  {112, -16, 304, 0},
  {-96, -16, 304, 0},
  {-96, -224, 224, 1},
  {80, -224, 224, 2},
  {-96, -224, -288, 2},
  {80, -224, -288, 1},
  {-144, 0, 224, 1},
  {176, 0, 224, 2},
  {-112, -32, 448, 0},
  {144, -32, 448, 0},
  {-224, -176, 576, 0},
  {224, -176, 576, 0},
  {0, 16, 336, 0},
  {-96, -64, 400, 0},
  {128, -64, 400, 0},
  {-160, -672, -688, 0},
  {160, -672, -688, 0},
  {-112, -32, 448, 0},
  {144, -32, 448, 0},
  {0, -96, 304, 0},
  {112, -16, 304, 0},
  {-96, -16, 304, 0},
};

void init_mech(void)
{
    asm volatile ("call ASM_init_mech\n"
        :  :  : "eax" );
}

void mech_unkn_func_02(void)
{
    asm volatile ("call ASM_mech_unkn_func_02\n"
        :  :  : "eax" );
}

void mech_unkn_func_09(short thing)
{
    asm volatile ("call ASM_mech_unkn_func_09\n"
        : : "a" (thing));
}

void veh_add(struct Thing *p_thing, short frame)
{
    asm volatile ("call ASM_veh_add\n"
        : : "a" (p_thing), "d" (frame));
}

ushort veh_passenger_count(struct Thing *p_veh)
{
    struct Thing *p_thing;
    short thing;
    ushort c;

    c = 0;

    thing = p_veh->U.UVehicle.PassengerHead;
    while (thing > 0 && c < THINGS_LIMIT)
    {
        c++;
        p_thing = &things[thing];
        thing = p_thing->U.UPerson.LinkPassenger;
    }
    return c;
}

void VNAV_unkn_func_207(struct Thing *p_thing)
{
    asm volatile ("call ASM_VNAV_unkn_func_207\n"
        : : "a" (p_thing));
}

/******************************************************************************/
