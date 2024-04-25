/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file vehicle.h
 *     Header file for vehicle.c.
 * @par Purpose:
 *     Routines implementing vehicle.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 12 Jun 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef VEHICLE_H
#define VEHICLE_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

enum SubTypeVehicle {
  SubTT_VEH_GROUND = 0x28,
  SubTT_VEH_SHUTTLE_POD = 0x29,
  SubTT_VEH_UNKN42 = 0x2A,
  SubTT_VEH_UNKN43 = 0x2B,
  SubTT_VEH_UNKN44 = 0x2C,
  SubTT_VEH_UNKN45 = 0x2D,
  SubTT_VEH_UNKN46 = 0x2E,
  SubTT_VEH_UNKN47 = 0x2F,
  SubTT_VEH_UNKN48 = 0x30,
  SubTT_VEH_UNKN49 = 0x31,
  SubTT_VEH_FLYING = 0x32,
  SubTT_VEH_TANK = 0x33,
  SubTT_VEH_UNKN52 = 0x34,
  SubTT_VEH_UNKN53 = 0x35,
  SubTT_VEH_SHIP = 0x36,
  SubTT_VEH_UNKN55 = 0x37,
  SubTT_VEH_UNKN56 = 0x38,
  SubTT_VEH_UNKN57 = 0x39,
  SubTT_VEH_UNKN58 = 0x3A,
  SubTT_VEH_MECH = 0x3B,
  SubTT_VEH_UNKN60 = 0x3C,
};

struct Thing;

#pragma pack()
/******************************************************************************/
void init_mech(void);
void mech_unkn_func_02(void);
void mech_unkn_func_09(short thing);

void veh_add(struct Thing *p_thing, short frame);

ushort veh_passenger_count(struct Thing *p_veh);

void VNAV_unkn_func_207(struct Thing *p_thing);

void process_tank_turret(struct Thing *p_tank);

void process_vehicle(struct Thing *p_vehicle);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
