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

#include <assert.h>
#include "bfmath.h"
#include "bfutility.h"
#include "ssampply.h"
#include "bigmap.h"
#include "bmbang.h"
#include "game.h"
#include "pathtrig.h"
#include "sound.h"
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

int check_for_a_vehicle_here(int x, int z, struct Thing *p_vehicle)
{
    int ret;
    asm volatile (
      "call ASM_check_for_a_vehicle_here\n"
        : "=r" (ret) : "a" (x), "d" (z), "b" (p_vehicle));
    return ret;
}

void start_crashing(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_start_crashing\n"
        : : "a" (p_vehicle));
}

void process_shuttle_pod(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_shuttle_pod\n"
        : : "a" (p_vehicle));
}

void init_vehicle_explode(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_init_vehicle_explode\n"
        : : "a" (p_vehicle));
}

void process_tank_stationary(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_tank_stationary\n"
        : : "a" (p_vehicle));
}

void process_tank(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_tank\n"
        : : "a" (p_vehicle));
}

void set_passengers_location(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_set_passengers_location\n"
        : : "a" (p_vehicle));
}

void move_flying_vehicle(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_move_flying_vehicle\n"
        : : "a" (p_vehicle));
}

short angle_between_points(int x1, int z1, int x2, int z2)
{
  return LbArcTanAngle(x2 - x1, z1 - z2);
}

void process_tank_turret(struct Thing *p_tank)
{
    struct Thing *p_turret;
    int target_x, target_y;
    int turret;
    short angle;
    int dt_angle;

    turret = p_tank->U.UVehicle.SubThing;
    if (turret == 0) {
        p_tank->OldTarget = LbFPMath_PI+1;
        return;
    }
    p_turret = &things[turret];
    if ((p_tank->Flag & TngF_Unkn20000000) != 0)
    {
        target_x = p_tank->U.UVehicle.TargetDX;
        target_y = p_tank->U.UVehicle.TargetDZ;
    }
    else
    {
        struct Thing *p_target;
        p_target = p_tank->PTarget;
        if (p_target == NULL)
        {
            p_tank->OldTarget = 20000;
            return;
        }
        target_x = PRCCOORD_TO_MAPCOORD(p_target->X);
        target_y = PRCCOORD_TO_MAPCOORD(p_target->Z);
    }
    angle = p_turret->U.UMGun.AngleY
        - angle_between_points(target_x, target_y, PRCCOORD_TO_MAPCOORD(p_tank->X), PRCCOORD_TO_MAPCOORD(p_tank->Z));
    if (angle < -LbFPMath_PI)
        angle += 2*LbFPMath_PI;
    else if (angle > LbFPMath_PI)
        angle -= 2*LbFPMath_PI;

    //TODO CONFIG how fast the tank can target could be a difficulty-related setting
    // Travel 1/8 of the distance in each game turn
    dt_angle = angle / 8;
    if (dt_angle > LbFPMath_PI/17)
        dt_angle = LbFPMath_PI/17;
    if (dt_angle < -LbFPMath_PI/17)
        dt_angle = -LbFPMath_PI/17;
    if (dt_angle == 0)
    {
        if (angle > 0)
            dt_angle = 1;
        if (angle < 0)
            dt_angle = -1;
    }

    // Despite being caused by the turret, we bind the sound samples to the vehicle part
    // of the tank. This is because turrets do not contain full position on map, so the
    // sound update would misplace the sound source if it was bound to the turret
    if ((p_turret->Flag2 & 0x0200) != 0)
    {
        if (dt_angle <= 1) {
            // Play rotation stop sample
            if (!IsSamplePlaying(p_tank->ThingOffset, 47, 0))
                play_dist_sample(p_tank, 47, 127, 0x40u, 100, 0, 1);
            p_turret->Flag2 &= ~0x0200;
        }
    }
    else
    {
        // Play rotation sample if moving over 1.2 degree per turn and the angle is not getting smaller.
        // Huge values of OldTarget (beyond pi) indicate that previously we had no target.
        if ((abs(dt_angle) >= LbFPMath_PI/75) && (p_tank->OldTarget < abs(angle) || p_tank->OldTarget > LbFPMath_PI)) {
            if (!IsSamplePlaying(p_tank->ThingOffset, 48, 0))
                play_dist_sample(p_tank, 48, 127, 0x40u, 100, 0, 1);
            p_turret->Flag2 |= 0x0200;
        }
    }

    p_turret->U.UMGun.AngleY -= dt_angle;
    p_tank->OldTarget = abs(angle);
    p_turret->U.UMGun.AngleY = (p_turret->U.UMGun.AngleY + 2*LbFPMath_PI) & LbFPMath_AngleMask;
}

void process_ship(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_ship\n"
        : : "a" (p_vehicle));
}

void process_mech_stationary(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_mech_stationary\n"
        : : "a" (p_vehicle));
}

void process_mech(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_mech\n"
        : : "a" (p_vehicle));
}

void process_mech_unknown1(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_mech_unknown1\n"
        : : "a" (p_vehicle));
}

int process_my_crashing_vehicle(struct Thing *p_vehicle)
{
    int ret;
    asm volatile ("call ASM_process_my_crashing_vehicle\n"
        : "=r" (ret) : "a" (p_vehicle));
    return ret;
}

int train_unkn_st21_exit_func_1(struct Thing *p_vehicle)
{
    int ret;
    asm volatile ("call ASM_train_unkn_st21_exit_func_1\n"
        : "=r" (ret) : "a" (p_vehicle));
    return ret;
}

void train_unkn_st18_func_1(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_train_unkn_st18_func_1\n"
        : : "a" (p_vehicle));
}

void train_unkn_st18_func_2(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_train_unkn_st18_func_2\n"
        : : "a" (p_vehicle));
}

void move_vehicle(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_move_vehicle\n"
        : : "a" (p_vehicle));
}

void train_unkn_st20_func_1(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_train_unkn_st20_func_1\n"
        : : "a" (p_vehicle));
}

void process_my_flying_vehicle(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_my_flying_vehicle\n"
        : : "a" (p_vehicle));
}

void process_unplacing(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_unplacing\n"
        : : "a" (p_vehicle));
}

void process_stop_as_soon_as_you_can(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_stop_as_soon_as_you_can\n"
        : : "a" (p_vehicle));
}

void process_hovering_vehicle(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_hovering_vehicle\n"
        : : "a" (p_vehicle));
}

void process_my_takeoff(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_my_takeoff\n"
        : : "a" (p_vehicle));
}

void process_my_land_vehicle(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_my_land_vehicle\n"
        : : "a" (p_vehicle));
}

void set_vehicle_alt(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_set_vehicle_alt\n"
        : : "a" (p_vehicle));
}

void process_unstopping(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_unstopping\n"
        : : "a" (p_vehicle));
}

void process_stopping(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_stopping\n"
        : : "a" (p_vehicle));
}

void process_vehicle_goto_point_land(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_vehicle_goto_point_land\n"
        : : "a" (p_vehicle));
}

void process_vehicle_goto_point_fly(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_vehicle_goto_point_fly\n"
        : : "a" (p_vehicle));
}

void process_parked_flyer(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_parked_flyer\n"
        : : "a" (p_vehicle));
}

void process_train(struct Thing *p_vehicle)
{
    int dtvel;

    dtvel = p_vehicle->U.UVehicle.ReqdSpeed - p_vehicle->Speed;
    if (abs(dtvel) >= 4) {
        p_vehicle->Speed += dtvel >> 1;
    } else {
        p_vehicle->Speed = p_vehicle->U.UVehicle.ReqdSpeed;
    }

    if (p_vehicle->State == VehSt_UNKN_45)
    {
          p_vehicle->Y <<= 3;
          if (process_my_crashing_vehicle(p_vehicle))
          {
              move_flying_vehicle(p_vehicle);
              p_vehicle->Y >>= 3;
              set_passengers_location(p_vehicle);
          }
          return;
    }

    if (p_vehicle->U.UVehicle.TNode != 0)
    {
        struct Thing *p_station;

        assert(p_vehicle->SubType == SubTT_VEH_TRAIN); // only trains have stations
        assert(p_vehicle->U.UVehicle.TNode > 0); // trains stations have positive indexes
        p_station = &things[p_vehicle->U.UVehicle.TNode];
        if ((p_station->Type != TT_BUILDING) || (p_station->Flag & TngF_Unkn0002) != 0)
        {
            p_vehicle->Flag |= TngF_Unkn0002;
            start_crashing(p_vehicle);
            p_vehicle->U.UVehicle.ReqdSpeed = 0;
            return;
        }
    }

    switch (p_vehicle->State)
    {
    case VehSt_UNKN_12:
        if (p_vehicle->U.UVehicle.TNode != 0)
        {
            if ((p_vehicle->Flag & 0x8000000) != 0)
                train_unkn_st18_func_1(p_vehicle);
            else
                train_unkn_st18_func_2(p_vehicle);
        }
        set_passengers_location(p_vehicle);
        move_vehicle(p_vehicle);
        break;
    case VehSt_UNKN_13:
        if (!train_unkn_st21_exit_func_1(p_vehicle))
        {
            p_vehicle->State = VehSt_UNKN_14;
            p_vehicle->U.UVehicle.Timer2 = 100;
            play_dist_sample(p_vehicle, 225 + (LbRandomAnyShort() % 4), 0x7Fu, 0x40u, 100, 0, 1);
        }
        break;
    case VehSt_UNKN_14:
        train_unkn_st20_func_1(p_vehicle);
        set_passengers_location(p_vehicle);
        break;
    case VehSt_UNKN_15:
        train_unkn_st21_exit_func_1(p_vehicle);
        if (p_vehicle->U.UVehicle.GotoX != 0 && things[p_vehicle->U.UVehicle.GotoX].State == VehSt_UNKN_12) {
            p_vehicle->State = VehSt_UNKN_12;
        }
        else if (p_vehicle->Owner != 0 && things[p_vehicle->Owner].State == VehSt_UNKN_12) {
            p_vehicle->State = VehSt_UNKN_12;
        }
        set_passengers_location(p_vehicle);
        break;
    default:
        LOGERR("Shagged train state %d", (int)p_vehicle->State);
        break;
    }
}

void process_vehicle_goto_point_liftoff(struct Thing *p_vehicle)
{
    int i;

    set_passengers_location(p_vehicle);
    i = alt_at_point(PRCCOORD_TO_MAPCOORD(p_vehicle->X), PRCCOORD_TO_MAPCOORD(p_vehicle->Z));
    if (p_vehicle->Y < MAPCOORD_TO_PRCCOORD(400, 0) + (i << 3)) {
        p_vehicle->Speed = 0;
        p_vehicle->Y += MAPCOORD_TO_PRCCOORD(8, 0);
    } else {
        p_vehicle->SubState = 6;
    }
}

void process_veh_ground(struct Thing *p_vehicle)
{
    int dtvel;

    dtvel = p_vehicle->U.UVehicle.ReqdSpeed - p_vehicle->Speed;
    if (dtvel != 0)
    {
        if (dtvel > 100)
            p_vehicle->Speed += 100;
        else if (dtvel < -400)
            p_vehicle->Speed -= 400;
        else
            p_vehicle->Speed = p_vehicle->U.UVehicle.ReqdSpeed;
    }

    switch (p_vehicle->State)
    {
    case VehSt_UNKN_3A:
    case VehSt_UNKN_42:
    case VehSt_UNKN_43:
        p_vehicle->Y <<= 3;
        process_my_flying_vehicle(p_vehicle);
        move_flying_vehicle(p_vehicle);
        set_passengers_location(p_vehicle);
        p_vehicle->Y >>= 3;
        break;
    case VehSt_UNKN_3E:
        process_unplacing(p_vehicle);
        move_vehicle(p_vehicle);
        set_passengers_location(p_vehicle);
        break;
    case VehSt_UNKN_41:
        p_vehicle->U.UVehicle.ReqdSpeed = 0;
        p_vehicle->U.UVehicle.AngleDY = 0;
        p_vehicle->Y <<= 3;
        process_hovering_vehicle(p_vehicle);
        move_flying_vehicle(p_vehicle);
        set_passengers_location(p_vehicle);
        p_vehicle->Y >>= 3;
        break;
    case VehSt_UNKN_45:
        p_vehicle->Y <<= 3;
        if (process_my_crashing_vehicle(p_vehicle)) {
            move_flying_vehicle(p_vehicle);
            set_passengers_location(p_vehicle);
        }
        p_vehicle->Y >>= 3;
        break;
    case VehSt_UNKN_39:
    case VehSt_UNKN_44:
        p_vehicle->Y <<= 3;
        process_my_takeoff(p_vehicle);
        move_flying_vehicle(p_vehicle);
        set_passengers_location(p_vehicle);
        p_vehicle->Y >>= 3;
        break;
    case VehSt_FLY_LANDING:
        p_vehicle->Y <<= 3;
        process_my_land_vehicle(p_vehicle);
        move_flying_vehicle(p_vehicle);
        set_passengers_location(p_vehicle);
        p_vehicle->Y >>= 3;
        break;
    case VehSt_UNKN_3C:
        p_vehicle->State = VehSt_UNKN_11;
        if (p_vehicle->U.UVehicle.TNode != 0)
              process_next_tnode(p_vehicle);
        move_vehicle(p_vehicle);
        set_passengers_location(p_vehicle);
        p_vehicle->State = VehSt_UNKN_3C;
        process_stop_as_soon_as_you_can(p_vehicle);
        break;
    case VehSt_UNKN_0:
    case VehSt_UNKN_3D:
    case VehSt_UNKN_21:
        if (p_vehicle->SubType == SubTT_VEH_FLYING)
            process_parked_flyer(p_vehicle);
        break;
    case VehSt_UNKN_27:
        switch (p_vehicle->SubState)
        {
        case 5:
            process_vehicle_goto_point_liftoff(p_vehicle);
            move_vehicle(p_vehicle);
            set_passengers_location(p_vehicle);
            break;
        case 6:
            process_vehicle_goto_point_fly(p_vehicle);
            move_vehicle(p_vehicle);
            set_passengers_location(p_vehicle);
            break;
        case 7:
            process_vehicle_goto_point_land(p_vehicle);
            move_vehicle(p_vehicle);
            set_passengers_location(p_vehicle);
            break;
        default:
            move_vehicle(p_vehicle);
            set_passengers_location(p_vehicle);
            break;
        }
        break;
    case VehSt_UNKN_36:
        process_stopping(p_vehicle);
        move_vehicle(p_vehicle);
        set_passengers_location(p_vehicle);
        break;
    case VehSt_UNKN_38:
        process_unstopping(p_vehicle);
        move_vehicle(p_vehicle);
        set_passengers_location(p_vehicle);
        break;
    case VehSt_UNKN_11:
    case VehSt_UNKN_32:
    case VehSt_UNKN_33:
    case VehSt_UNKN_34:
        if (p_vehicle->U.UVehicle.TNode != 0)
            process_next_tnode(p_vehicle);
        move_vehicle(p_vehicle);
        set_passengers_location(p_vehicle);
        break;
    default:
        LOGERR("Unexpected vehicle state %d", (int)p_vehicle->State);
        break;
    }

    if ((p_vehicle->SubType != SubTT_VEH_FLYING)
     && ((p_vehicle->Flag & 0x0002) == 0)) {
        set_vehicle_alt(p_vehicle);
    }
}

void vehicle_check_outside_map(struct Thing *p_vehicle)
{
    if ((p_vehicle->X <= 0) || (p_vehicle->X >= 0x800000) ||
        (p_vehicle->Z <= 0) || (p_vehicle->Z >= 0x800000))
    {
        if (p_vehicle->State != VehSt_UNKN_45)
            start_crashing(p_vehicle);
    }
}

void process_vehicle(struct Thing *p_vehicle)
{
#if 0
    asm volatile ("call ASM_process_vehicle\n"
        : : "a" (p_vehicle));
    return;
#endif
    if ((p_vehicle->Flag & 0x02) == 0)
        p_vehicle->OldTarget = 0;
    if (p_vehicle->U.UVehicle.RecoilTimer > 0)
        p_vehicle->U.UVehicle.RecoilTimer--;
    if ((p_vehicle->U.UVehicle.WorkPlace & 0x80) != 0)
    {
        p_vehicle->Health -= 16;
        if (p_vehicle->Health < 0)
            start_crashing(p_vehicle);
        if (((gameturn & 7) == 0) && (LbRandomAnyShort() & 7) == 0)
            p_vehicle->U.UVehicle.WorkPlace &= ~0x80;
    }

    switch (p_vehicle->SubType)
    {
    case SubTT_VEH_TRAIN:
        if (p_vehicle->State == VehSt_UNKN_D)
            break;
        process_train(p_vehicle);
        break;
    case SubTT_VEH_SHUTTLE_POD:
        process_shuttle_pod(p_vehicle);
        break;
    case SubTT_VEH_GROUND:
    case SubTT_VEH_UNKN43:
        process_veh_ground(p_vehicle);
        break;
    case SubTT_VEH_FLYING:
        vehicle_check_outside_map(p_vehicle);
        process_veh_ground(p_vehicle);
        break;
    case SubTT_VEH_TANK:
        if (p_vehicle->State == VehSt_UNKN_45) {
            bang_new4(p_vehicle->X, p_vehicle->Y, p_vehicle->Z, 10);
            init_vehicle_explode(p_vehicle);
        } else if (p_vehicle->State == VehSt_UNKN_21) {
            process_tank_stationary(p_vehicle);
        } else {
            process_tank(p_vehicle);
            set_passengers_location(p_vehicle);
        }
        process_tank_turret(p_vehicle);
        break;
    case SubTT_VEH_SHIP:
        process_ship(p_vehicle);
        set_passengers_location(p_vehicle);
        break;
    case SubTT_VEH_MECH:
        unkn_path_func_001(p_vehicle, 0);
        if (p_vehicle->State == VehSt_UNKN_21)
            process_mech_stationary(p_vehicle);
        else
            process_mech(p_vehicle);
        process_mech_unknown1(p_vehicle);
        set_passengers_location(p_vehicle);
        break;
    default:
        break;
    }
}


/******************************************************************************/
