/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file thing.h
 *     Header file for thing.c.
 * @par Purpose:
 *     Thing structure support.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 May 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef THING_H
#define THING_H

#include "bftypes.h"
#include "cybmod.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

#define THINGS_LIMIT 1000
#define STHINGS_LIMIT 1500

enum ThingType {
    TT_NONE = 0x0,
    TT_UNKN1 = 0x1,
    TT_VEHICLE = 0x2,
    TT_PERSON = 0x3,
    TT_UNKN4 = 0x4,
    SmTT_STATIC = 0x5,
    TT_ROCKET = 0x6,
    TT_UNKN7 = 0x7,
    TT_UNKN8 = 0x8,
    TT_BUILDING = 0x9,
    TT_UNKN10 = 0xA,
    TT_LASER11 = 0xB,
    TT_LASER_GUIDED = 0xC,
    TT_UNKN13 = 0xD,
    TT_UNKN14 = 0xE,
    TT_MINE = 0xF,
    TT_GRENADE = 0x10,
    SmTT_SPARK = 0x11,
    TT_UNKN18 = 0x12,
    TT_LASER_ELEC = 0x13,
    SmTT_INTELLIG_DOOR = 0x14,
    SmTT_SCALE_EFFECT = 0x15,
    SmTT_NUCLEAR_BOMB = 0x16,
    TT_UNKN23 = 0x17,
    SmTT_SMOKE_GENERATOR = 0x18,
    SmTT_DROPPED_ITEM = 0x19,
    SmTT_CARRIED_ITEM = 0x1A,
    SmTT_ELECTRIC_STRAND = 0x1B,
    TT_RAZOR_WIRE = 0x1C,
    TT_LASER29 = 0x1D,
    SmTT_TIME_POD = 0x1E,
    TT_AIR_STRIKE = 0x1F,
    SmTT_CANISTER = 0x20,
    TT_UNKN33 = 0x21,
    TT_UNKN34 = 0x22,
    TT_UNKN35 = 0x23,
    SmTT_STASIS_POD = 0x24,
    SmTT_SOUL = 0x25,
    TT_LASER38 = 0x26,
    TT_UNKN39 = 0x27,
    TT_UNKN40 = 0x28,
    TT_UNKN41 = 0x29,
    TT_UNKN42 = 0x2A,
    TT_UNKN43 = 0x2B,
    TT_UNKN44 = 0x2C,
    TT_UNKN45 = 0x2D,
    TT_UNKN46 = 0x2E,
    TT_UNKN47 = 0x2F,
    TT_UNKN48 = 0x30,
    TT_UNKN49 = 0x31,
    SmTT_BANG = 0x32,
    TT_UNKN51 = 0x33,
    SmTT_FIRE = 0x34,
    SmTT_SFX = 0x35,
    SmTT_TEMP_LIGHT = 0x36,
    TT_UNKN55 = 0x37,
    TT_UNKN56 = 0x38,
};

/** State of Thing of type Object/Building.
 */
struct TngUObject
{
    short NextThing;
    short PrevThing;
    ubyte Group;
    ubyte EffectiveGroup;
    short Object;
    short MatrixIndex;
    ubyte NumbObjects;
    ubyte Angle;
    ubyte Token;
    char TokenDir;
    char OffX;
    char OffZ;
    short TargetDX; // within Thing, pos=92
    short TargetDY;
    short TargetDZ;
    ushort BuildStartVect;
    ushort BuildNumbVect;
    ushort ZZ_unused_but_pads_to_long_ObjectNo;
    ushort ComHead;
    short ComCur;
    short Mood;
    short RaiseDY[2];
    short RaiseY[2];
    short MinY[2];
    short Timer[2];
    short MaxY[2];
    short TNode;
    short Cost;
    short Shite;
    int BHeight;
    ushort Turn;
    ushort TurnPadOnPS;
    short tnode[4];
    ubyte player_in_me;
    ubyte unkn_4D;
    ulong DrawTurn; // set within draw_thing_object() for a building
};

/** State of Thing of type Mounted Gun.
 */
struct TngUMGun
{
    short PathIndex;
    ushort UniqueID;
    ubyte Group;
    ubyte EffectiveGroup;
    short Object;
    short MatrixIndex;
    ubyte NumbObjects;
    ubyte ShotTurn;
    short WeaponTurn;
    ushort ObjectNo;
    short PrevThing; // within Thing, pos=92
    short NextThing;
    ubyte Token;
    char TokenDir;
    short AngleDY;
    short AngleX;
    short AngleY;
    short AngleZ;
    short GotoX;
    short GotoY;
    short GotoZ;
    int Dummy11[5];
    short RecoilTimer;
    ushort MaxHealth;
    ubyte CurrentWeapon;
    ubyte Dummy12;
    ubyte Dummy13[6];
};

/** State of Thing of type Vehicle.
 */
struct TngUVehicle
{
  short PathIndex;
  ushort UniqueID;
  ubyte Group;
  ubyte EffectiveGroup;
  short Object;
  short MatrixIndex;
  ubyte NumbObjects;
  ubyte Dummy2;
  short WeaponTurn;
  short ReqdSpeed;
  ushort MaxSpeed; // within Thing, pos=92
  ushort PassengerHead;
  /** Traffic node where the vehicle is moving.
   */
  short TNode;
  short AngleDY;
  short AngleX;
  short AngleY;
  short AngleZ;
  short GotoX;
  short GotoY;
  short GotoZ;
  short VehicleAcceleration;
  short LeisurePlace;
  short TargetDX;
  short TargetDY;
  short TargetDZ;
  short OnFace;
  short WorkPlace;
  ushort ComHead;
  short ComCur;
  short Timer2;
  short RecoilTimer;
  ushort MaxHealth;
  int Dummy[3];
  ushort SubThing;
  short Agok;
  int WobbleZP;
  int WobbleZV;
  ubyte Armour;
  ubyte PissedOffWithWaiting;
  short ZebraOldHealth;
  ushort destx;
  ushort destz;
};

/** State of Thing of type Effect.
 */
struct TngUEffect
{
  ushort MaxSpeed;
  short ReqdSpeed;
  ubyte Group;
  ubyte EffectiveGroup;
  short Object;
  short WeaponTurn;
  ubyte NumbObjects;
  ubyte Angle;
  ushort PassengerHead;
  short TNode;
  short AngleDY; // within Thing, pos=92
  short AngleX;
  short AngleY;
  short AngleZ;
  short GotoX;
  short GotoY;
  short GotoZ;
  short VehicleAcceleration;
  short MatrixIndex;
  short LeisurePlace;
};

/** Person Draw Sprite Frame.
 */
struct DrawFrameId
{
  ubyte Version[5];
};

/** State of Thing of type Person.
 */
struct TngUPerson
{
  short PathIndex;
  ushort UniqueID;
  /** Normal GroupId assignment of a thing.
   * Used to reset `EffectiveGroup` when neccessary.
   */
  ubyte Group;
  /** Effective group is the GroupId of a thing which is actually in effect.
   * It may diverge from normal `Group` ie when a Person is persuaded.
   */
  ubyte EffectiveGroup;
  ushort ComHead;
  ushort ComCur;
  char SpecialTimer;
  ubyte Angle;
  short WeaponTurn;
  ubyte Brightness;
  ubyte ComRange;
  ubyte BumpMode; // within Thing, pos=92
  ubyte BumpCount;
  short Vehicle;
  short LinkPassenger;
  ushort Within;
  ushort LastDist;
  short ComTimer;
  short Timer2;
  short StartTimer2;
  ubyte AnimMode;
  ubyte OldAnimMode;
  short OnFace;
  union Mod UMod;
  short Mood;
  struct DrawFrameId FrameId;
  ubyte Shadows[4];
  ubyte RecoilTimer;
  ushort MaxHealth;
  ubyte Flag3;
  ubyte OldSubType;
  short ShieldEnergy;
  ubyte ShieldGlowTimer;
  ubyte WeaponDir;
  ushort SpecialOwner;
  ushort WorkPlace;
  ushort LeisurePlace;
  short WeaponTimer;
  short Target2;
  short MaxShieldEnergy;
  short PersuadePower;
  short MaxEnergy;
  short Energy;
  ubyte RecoilDir;
  ubyte CurrentWeapon;
  short GotoX;
  short GotoZ;
  short TempWeapon;
  short Stamina;
  short MaxStamina;
  ulong WeaponsCarried;
};

/** Structure for storing State of any Thing.
 * Everything in game besides ground terrain, are Things.
 */
struct Thing { // sizeof=168
    /** Previous object in the `mapwho` linked list.
     */
    short Parent;
    /** Next object in the `mapwho` linked list.
     */
    short Next;
    short LinkParent;
    short LinkChild;
    ubyte SubType;
    ubyte Type;
    short State;
    ulong Flag;
    short LinkSame;
    short LinkSameGroup;
    short Radius;
    ushort ThingOffset;
    long X;
    long Y;
    long Z;
    short Frame;
    ushort StartFrame;
    short Timer1;
    short StartTimer1;
    long VX;
    long VY;
    long VZ;
    short Speed;
    short Health;
    short Owner;
    ubyte PathOffset;
    ubyte SubState;
    struct Thing *PTarget;
    long Flag2;
    short GotoThingIndex;
    short OldTarget;
    union { // pos=76
        struct TngUObject UObject;
        struct TngUMGun UMGun;
        struct TngUVehicle UVehicle;
        struct TngUEffect UEffect;
        struct TngUPerson UPerson;
    } U;
};

struct STngUEffect {
    short VX;
    short VY;
    short VZ;
    short OX;
    short OY;
    short OZ;
};

struct STngUScenery {
    ushort Frame;
    ushort StartFrame;
    short LightHead;
    short LightDie;
    short LightAnim;
    short Health;
};

struct STngUWeapon {
    short WeaponType;
    short LastFired;
    short Ammo;
    short Owner;
    short OnFace;
};

struct STngUTraffic {
    short Link[4];
    ubyte Flags[4];
};

struct STngULight {
    short MinBright;
    short RangeBright;
};

struct STngUBang {
    short shrapnel;
    short phwoar;
};

struct STngUFire {
    short flame;
};

struct SimpleThing
{
    short Parent;
    short Next;
    short LinkParent;
    short LinkChild;
    ubyte SubType;
    ubyte Type;
    short State;
    ulong Flag;
    short LinkSame;
    short Object;
    short Radius;
    short ThingOffset;
    long X;
    long Y;
    long Z;
    short Frame;
    short StartFrame;
    short Timer1;
    short StartTimer1;
    union {
        struct STngUEffect UEffect;
        struct STngUScenery UScenery;
        struct STngUWeapon UWeapon;
        struct STngUTraffic UTraffic;
        struct STngULight ULight;
        struct STngUBang UBang;
        struct STngUFire UFire;
    } U;
    short field_38;
    ushort UniqueID;
};

typedef struct {
    short Arg1;
    short Arg2;
    short Arg3;
    short Arg4;
    long Arg5;
} ThingFilterParams;

/** Definition of a simple callback type which can only return true/false and has no memory of previous checks. */
typedef TbBool (*ThingBoolFilter)(short thing, ThingFilterParams *params);

/** Old structure for storing State of any Thing.
 * Used only to allow reading old, pre-release levels.
 */
struct ThingOldV9 { // sizeof=216
    short Parent;
    short Next;
    short LinkParent;
    short LinkChild;
    ubyte SubType;
    ubyte Type;
    short State; // pos=10
    ulong Flag;
    /* Since fmtver=4, this and all previous fields confirmed to match
     * file layout in final release (from Pre-Alpha Demo code analysis
     * and from comparative analysis of binary data in level files).
     */
    short LinkSame;
    /** Stores index of first item within game_objects[].
     * Since fmtver=4, most types (Object,Vehicle,MGun,Effect) have `Object`
     * and it is at the same place for all types (from Pre-Alpha Demo code
     * analysis). The Vehicle `Object` confirmed in fmtver=8-11 files (from
     * comparative analysis of binary data in level files).
     */
    union {
        short ObjectObject;
        short VehicleObject;
        short MGunObject;
        short EffectObject;
    };
    short Radius; // pos=20
    /* Since fmtver=4, most types (Object,Vehicle,MGun,Effect) have `NumbObjects`
     * and it is at the same place (from Pre-Alpha Demo code analysis).
     */
    union {
        ubyte ObjectNumbObjects;
        ubyte VehicleNumbObjects;
        ubyte MGunNumbObjects;
        ubyte EffectNumbObjects;
    };
    ubyte TngUnkn23;
    long X;
    long Y;
    long Z;
    /* Since fmtver=4, most types have `Frame`, only Object type GATE reuses it
     * for `MinY[0]` (from Pre-Alpha Demo code analysis).
     */
    union {
        short Frame;
        short ObjectMinY0;
    };
    /* Since fmtver=4, most types have `StartFrame`, only Object type GATE
     *  reuses it for `RaiseY[1]` (from Pre-Alpha Demo code analysis).
     */
    union {
        ushort StartFrame;
        short ObjectRaiseY1;
    };
    short Timer1; // pos=40
    short StartTimer1;
    /* Since fmtver=4, `Timer2` for Person and Vehicle, but Objects have
     * `Timer[0]` (from Pre-Alpha Demo code analysis).
     */
    union {
        short Timer2;
        short ObjectTimer0;
    };
    /* Since fmtver=4, `StartTimer2` for Person, but Objects have `Timer[1]`
     * (from Pre-Alpha Demo code analysis).
     */
    union {
        short StartTimer2;
        short ObjectTimer1;
    };
    /* Since fmtver=4, Person `AnimMode` (from Pre-Alpha Demo code analysis).
     * Confirmed to be low value used against Person in fmtver=8-11 files
     * (from comparative analysis of binary data in level files).
     */
    ubyte PersonAnimMode; // pos=48
    /* Since fmtver=4, Person `OldAnimMode` (from Pre-Alpha Demo code analysis).
     */
    ubyte PersonOldAnimMode;
    ushort ThingOffset; // pos=50
    /* Since fmtver=4, usually `VX`, but Object type GATE reuses it for
     * `RaiseDY[]` (from Pre-Alpha Demo code analysis).
     * The `VX`/`VY`/`VZ` confirmed in fmtver=8-11 (from comparative analysis
     * of binary data in level files).
     */
    union {
        long VX;
        short ObjectRaiseDY[2];
    };
    long VY;
    long VZ;
    /* Since fmtver=4, `AngleX` for Vehicle and MGun (from Pre-Alpha Demo code
     * analysis). Stamina is only here in fmtver=12 files - does not seem to
     * exist in 9-11 (from comparative analysis of binary data in level files).
     */
    union {
        short VehicleAngleX;
        short MGunAngleX;
        short PersonStamina;
    };
    /* Since fmtver=4, `AngleY` for Vehicle and MGun (from Pre-Alpha Demo code
     * analysis). MaxStamina is only here in fmtver=12 files (from comparative
     * analysis of binary data in level files).
     */
    union {
        short VehicleAngleY;
        short MGunAngleY;
        short PersonMaxStamina;
    };
    /* Since fmtver=4, `AngleZ` for Vehicle and MGun (from Pre-Alpha Demo code
     * analysis).
     */
    union { // pos=68
        short VehicleAngleZ;
        short MGunAngleZ;
    };
    /* The `LinkSameGroup` confirmed in fmtver=8-11 (from comparative analysis
     * of binary data in level files).
     */
    short LinkSameGroup; // pos=70
    short TngUnkn72;
    /* Since fmtver=4, `Angle` for Person and Object (from Pre-Alpha Demo code
     * analysis). Confirmed to be some Person stat in fmtver=8-11 (from
     * comparative analysis of binary data in level files).
     */
    union {
        ubyte PersonAngle;
        ubyte ObjectAngle;
    };
    /** Effective group is the GroupId of a thing which is actually in effect.
     * Configrmed to be  Person `EffectiveGroup` since fmtver=4 (from Pre-Alpha
     * Demo code analysis). Also xonfirmed in fmtver=8-11 (from comparative
     * analysis of binary data in level files). These sources do not provide a
     * definite proof about which group field is effective and which the default.
     */
    union { // pos=75
        ubyte PersonEffectiveGroup;
        ubyte VehicleEffectiveGroup;
        ubyte ObjectEffectiveGroup;
        ubyte MGunEffectiveGroup;
    };
    /* Since fmtver=4, `Speed` for Vehicle (from Pre-Alpha Demo code analysis).
     */
    short Speed; // pos=76
    short Health; // pos=78
    /* Since fmtver=4, `GotoX` for Person (from Pre-Alpha Demo code analysis).
     * Assumed to be `GotoX` for most types (MGun,Vehicle,Effect).
     */
    union { // pos=80
        short PersonGotoX;
        short MGunGotoX;
        short VehicleGotoX;
        short EffectGotoX;
    };
    /* Since fmtver=4, `GotoY` for Person (from Pre-Alpha Demo code analysis).
     * Though there is no Parson `GotoY` in final, this is assumed to be `GotoY`
     * for most types (MGun,Vehicle,Effect).
     */
    union { // pos=82
        short PersonGotoY;
        short MGunGotoY;
        short VehicleGotoY;
        short EffectGotoY;
    };
    /* Since fmtver=4, `GotoZ` for Person (from Pre-Alpha Demo code analysis).
     * Assumed to be `GotoZ` for most types (MGun,Vehicle,Effect).
     */
    union { // pos=84
        short PersonGotoZ;
        short MGunGotoZ;
        short VehicleGotoZ;
        short EffectGotoZ;
    };
    /* Since fmtver=4, `Group` for Person (from Pre-Alpha Demo code analysis).
     * Confirmed the same in fmtver=8-11 (from comparative analysis of
     * binary data in level files).
     */
    union {  // pos=86
        short VehicleUnknTng86;
        struct {
            ubyte PersonGroup;
            ubyte TngUnkn87;
        };
    };
    ulong PersonWeaponsCarried; // pos=88
    /** Next command assigned to the Person.
     * Confirmed since fmtver=4 (from Pre-Alpha Demo code analysis).
     */
    ushort PersonComHead; // pos=92
    ushort PersonComCur; // pos=94
    short PersonComTimer; // pos=96
    short TngUnkn98; // pos=98
    /** Index of a thing which owns this one.
     * Confirmed since fmtver=4 (from Pre-Alpha Demo code analysis). Also
     * confirmed  in fmtver=8-11 (from comparative analysis of binary data
     * in level files).
     */
    ushort Owner; // pos=100
    /* Confirmed to be Person `CurrentWeapon` since fmtver=4 (from
     * Pre-Alpha Demo code analysis). Uses low values, for Person only
     * (from comparative analysis of binary data in level files).
     */
    short PersonCurrentWeapon; // pos=102
    short TngUnkn104; // pos=104
    /* Confirmed to be Person `WeaponTurn` since fmtver=4 (from
     * Pre-Alpha Demo code analysis).
     */
    ushort PersonWeaponTurn; // pos=106
    /** Rotation matrix index for a Vehicle.
     * Confirmed since fmtver=4 (from Pre-Alpha Demo code analysis). Also
     * confirmed in fmtver=8-11 (from comparative analysis of binary data
     * in level files).
     */
    ushort VehicleMatrixIndex; // pos=108
    /* Confirmed to be Person `Brightness` since fmtver=4 (from Pre-Alpha
     * Demo code analysis).
     */
    ubyte PersonBrightness; // pos=110
    /* Confirmed to be Person `PathOffset` since fmtver=4 (from
     * Pre-Alpha Demo code analysis).
     */
    ubyte PathOffset;
    /* Since fmtver=4, Person sprite frame of some kind (from Pre-Alpha
     * Demo code analysis).
     */
    ushort PersonFrameUnkn112; // pos=112
    /** Index of the path a person is walking.
     * Confirmed since fmtver=4 (from Pre-Alpha Demo code analysis).
     */
    ushort PersonPathIndex; // pos=114
    /* Person `MaxShieldEnergy` recognized in fmtver=8-11 (from comparative
     * analysis of binary data in level files).
     */
    short PersonMaxShieldEnergy;
    short TngUnkn118;
    long  TngUnkn120;
    short TngUnkn124; // pos=124
    ushort TngUnkn126;
    union { // pos=128
        ushort PersonUniqueID;
        ushort MGunUniqueID;
    };
    ushort TngUnkn130; // pos=130
    short PersonShieldEnergy; // pos=132
    char PersonSpecialTimer;
    ubyte TngUnkn135;
    short TngUnkn136;
    short TngUnkn138; // pos=138
    ubyte PersonBumpMode;
    ubyte PersonBumpCount;
    short PersonVehicle;
    short PersonLinkPassenger;
    /* Since fmtver=4, `TargetDX` for Object type GATE; Person sets it
     * as well, though purpose is uncertain (from Pre-Alpha Demo code
     * analysis). Assumed to be `Within` for Person, no proof.
     */
    union { // pos=146
        short ObjectTargetDX;
        ushort PersonWithin;
    };
    /* Since fmtver=4, `TargetDY` for Object type GATE (from Pre-Alpha Demo
     * code analysis). Assumed to be `LastDist` for Person, no proof.
     */
    union { // pos=148
        short ObjectTargetDY;
        ushort PersonLastDist;
    };
    /* Since fmtver=4, `TargetDZ` for Object type GATE (from Pre-Alpha Demo
     * code analysis).
     */
    union { // pos=150
        short ObjectTargetDZ;
        short TngUnkn150;
    };
    /** Pointer to a Thing being targeted by this one; cleared during load.
     * Confirmed since fmtver=4 (from Pre-Alpha Demo code analysis).
     */
    ulong PTarget; // pos=152
    short TngUnkn156; // pos=156
    /* Assumed to be `OnFace` for Person, no proof.
     */
    short PersonOnFace;
    ushort PersonUnkn160; // pos=160
    ubyte SubState;
    ubyte PersonComRange;
    /* Vehicle `ReqdSpeed`, confirmed since fmtver=4 (from Pre-Alpha Demo
     * code analysis). In that version there was no MaxSpeed, so this counted
     * for both.
     */
    ushort VehicleReqdSpeed; // pos=164
    /* Is Vehicle `UniqueID`, Person `WeaponTimer`, and also used in Objects
     * since fmtver=4 (from Pre-Alpha Demo code analysis).
     */
    union { // pos=166
        ushort VehicleUniqueID;
        short PersonWeaponTimer;
    };
    /* Vehicle `PassengerHead`, confirmed since fmtver=4 (from Pre-Alpha Demo
     * code analysis).
     */
    short VehiclePassengerHead; // pos=168
    /** Traffic node where the vehicle is moving.
     * Vehicle `TNode`, confirmed since fmtver=4 (from Pre-Alpha Demo code
     * analysis). Also confirmed in fmtver=8-11 (from comparative analysis
     * of binary data in level files).
     */
    short VehicleTNode;
    /* Vehicle `AngleDY`, confirmed since fmtver=4 (from Pre-Alpha Demo code
     * analysis).
     */
    short VehicleAngleDY; // pos=172
    ushort TngUnkn174;
    /* Person weapon `MaxEnergy`, confirmed in fmtver=8-11 (from comparative analysis
     * of binary data in level files).
     */
    ushort PersonMaxEnergy; // pos=176
    /* Person weapon `Energy`, confirmed since fmtver=4 (from Pre-Alpha Demo
     * code analysis).
     */
    ushort PersonEnergy;
    ushort TngUnkn180; // pos=180
    ushort TngUnkn182;
    ubyte TngUnkn184; // pos=184
    /** Used by lights processing routine.
     * Confirmed since fmtver=4 (from Pre-Alpha Demo code analysis).
     */
    ubyte PersonShadows[4]; // pos=185
    /* `RecoilTimer` at the same position for People and Vehicle.
     * Confirmed since fmtver=4 (from Pre-Alpha Demo code analysis).
     */
    union { // pos=189
        ubyte VehicleRecoilTimer;
        ubyte PersonRecoilTimer;
    };
    ushort PersonMaxHealth; // pos=190
    ubyte PersonRecoilDir; // pos=192
    ubyte TngUnkn193; // pos=193
    short GotoThingIndex; // pos=194
    short TngUnkn196;
    short TngUnkn198;
    short TngUnkn200;
    short TngUnkn202;
    /* Contains vehicle data in fmtver=8-11, specifics unknown
     * (from comparative analysis of binary data in level files).
     */
    short TngUnkn204;
    /* In fmtver=8-11, these map to values in the third and fourth bytes
     * of the "Dummy" vehicle value of the final level structure; vehicle
     * stats, similar to VehiclePassengerHead, specifics unknown
     * (from comparative analysis of binary data in level files).
     */
    short TngUnkn206;
    short TngUnkn208;
    short TngUnkn210;
    short TngUnkn212; // pos=212
    short TngUnkn214;
};

#pragma pack()
/******************************************************************************/
extern struct Thing *things;
extern ushort things_used_head;
extern ushort same_type_head[256+32];

extern struct SimpleThing *sthings;
extern ushort sthings_used_head;

void init_things(void);
void refresh_old_thing_format(struct Thing *p_thing, struct ThingOldV9 *p_oldthing, ulong fmtver);
void process_things(void);

/** Delete the thing from `mapwho` chain.
 */
TbResult delete_node(struct Thing *p_thing);

void add_node_thing(ushort new_thing);
short get_new_thing(void);
void remove_thing(short tngno);

void add_node_sthing(ushort new_thing);
short get_new_sthing(void);
void remove_sthing(short tngno);

TbBool thing_is_within_circle(short thing, short X, short Z, ushort R);

/** Unified function to find a thing of given type within given circle and matching filter.
 *
 * Tries to use mapwho and same type list, and if cannot then just searches all used things.
 *
 * @param X Map coordinate in map units.
 * @param Z Map coordinate in map units.
 * @param R Circle radius in map units.
 * @param ttype Thing Type; need to be specific, no -1 allowed.
 * @param subtype Thing SubType; to catch all, use -1.
 * @param filter Filter callback function.
 * @param param Parameters for filter callback function.
 */
short find_thing_type_within_circle_with_filter(short X, short Z, ushort R,
  short ttype, short subtype, ThingBoolFilter filter, ThingFilterParams *params);

short find_dropped_weapon_within_circle(short X, short Z, ushort R, short weapon);
short find_person_carrying_weapon_within_circle(short X, short Z, ushort R, short weapon);
short find_person_carrying_weapon(short weapon);

short find_nearest_from_group(struct Thing *p_person, ushort group, ubyte no_persuaded);
short search_things_for_index(short index);
short find_nearest_object2(short mx, short mz, ushort sub_type);
short search_object_for_qface(ushort object, ubyte gflag, ubyte flag, ushort after);
short search_for_station(short x, short z);
short search_things_for_uniqueid(short index, ubyte flag);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
