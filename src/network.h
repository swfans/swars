/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file network.h
 *     Header file for network.c.
 * @par Purpose:
 *     Network support.
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
#ifndef NETWORK_H
#define NETWORK_H

#include "bftypes.h"
#include "cybmod.h"
#include "weapon.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

typedef void *NSERV_HANDLE;

enum NetworkServiceType {
    NetSvc_NONE = 0x0,
    NetSvc_IPX,
    NetSvc_COM1,
    NetSvc_COM2,
    NetSvc_COM3,
    NetSvc_COM4,
    NetSvc_Unkn6,
};

struct NetworkServiceFunction { // sizeof=36
    NSERV_HANDLE SessionCreate; // offset=0
    NSERV_HANDLE SessionJoin;
    NSERV_HANDLE SessionExchange;
    NSERV_HANDLE SessionInit;
    NSERV_HANDLE SessionDial; // offset=16
    NSERV_HANDLE SessionAnswer;
    NSERV_HANDLE SessionHangUp;
    NSERV_HANDLE UsedSessionInit;
    NSERV_HANDLE netsvcfunc_unkn20; // offset=32
};

struct TbNetworkService { // sizeof=10
    NSERV_HANDLE Id; // offset=0
    ushort GameId; // offset=4
    ushort Type; // offset=6
    ushort Flags; // offset=8
    struct NetworkServiceFunction F;
};

struct NetworkPlayerUFourPacks {
  ubyte FourPacks[4][5];
};

struct NetworkPlayerUProgress {
  long Credits;
  ubyte val_181189;
  ubyte val_181183;
  ubyte TechLevel;
  ubyte SelectedCity;
  ushort npfield_8;
  ushort npfield_A;
  ubyte ControlMode[4];
  ubyte DoubleMode;
  ubyte val_flags_08;
  ubyte npfield_12;
  ubyte val_15516D;
  long Expenditure;
};

struct NetworkPlayerUWepMod {
  ulong Weapons[4];
  union Mod Mods[4];
};

struct NetworkPlayerURandInit {
  ulong Seed;
};

struct NetworkPlayerUUnkn {
    long npfield_1;
    sbyte npfield_5;
    sbyte npfield_6;
    sbyte npfield_7;
    sbyte SelectedCity;
    sbyte npfield_9[2];
    sbyte npfield_b;
    sbyte npfield_c;
    ushort npfield_d;
    ushort npfield_e;
    sbyte DoubleMode;
    sbyte npfield_12;
    sbyte npfield_13;
    sbyte npfield_14;
    long npfield_15;
};

struct NetworkPlayer { // sizeof=26
    ubyte Type;
    union {
      struct NetworkPlayerUFourPacks FourPacks;
      struct NetworkPlayerUProgress Progress;
      struct NetworkPlayerUWepMod WepMod;
      struct NetworkPlayerURandInit RandInit;
      char Text[24];
      struct NetworkPlayerUUnkn Unkn;
    } U;
    sbyte npfield_19;
};

// TODO maybe this only ocntains one string?
struct NetPlayer2 {
  char field_0[13];
  char field_D;
  char field_E[10];
  char field_18;
};

struct ModemCommand {
  char cmd[80];
};

struct TbSerialDev { // sizeof=4301
  ubyte field_0[2198];
  ubyte inbuf[2048];
  ushort field_1096;
  ushort field_1098;
  ushort field_109A;
  ushort inbuf_pos;
  ushort field_109E;
  ushort field_10A0;
  ushort field_10A2;
  int baudrate;
  ubyte comdev_id;
  ubyte field_10A9;
  ubyte field_10AA;
  ubyte field_10AB;
  ubyte num_players;
  ubyte field_10AD[31];
  ubyte field_10CC;
};

struct ComHandlerInfo {
  struct TbSerialDev *serdev;
  ushort field_4;
  ushort field_6;
  ushort field_8;
  ushort field_A[4];
  ulong field_12;
  ushort field_16;
  ushort field_18;
  ushort field_1A;
};

struct TbIPXPlayerHeader {
  ubyte field_0;
  ubyte field_1;
  short field_2;
  ubyte field_4[8];
  ubyte field_C[16];
  ubyte field_1C[4];
  short field_20;
  ubyte field_22[4];
  long field_26;
  ubyte field_2A;
  ubyte field_2B;
  ubyte field_2C;
  ubyte field_2D[26];
  short field_47[99];
  ubyte num_players;
  ubyte field_10E;
};

struct TbIPXPlayer {
  struct TbIPXPlayerHeader Header;
  ubyte field_10F[268];
  ubyte field_21B;
};

struct TbIPXHandler {
  short field_0;
  short field_2;
  short PlayerDataSize;
  short field_6;
  ushort field_8;
  ubyte field_A;
  ubyte field_B;
  ubyte field_C;
  ubyte field_D;
  ubyte field_E[4];
  ubyte field_12[24];
  long field_2A;
  ubyte field_2E[4];
  short field_32;
  ubyte field_34[16];
  short field_44;
  ubyte field_46[30];
  struct TbIPXPlayer PlayerData[30];
};

#pragma pack()
/******************************************************************************/
extern ubyte data_1c4a70;
extern ubyte net_unkn_pos_01b;
extern ubyte net_players_num;
extern struct TbNetworkService NetworkServicePtr;
extern ulong NetTimeoutTicks;
extern struct NetworkPlayer network_players[8];
extern struct NetPlayer2 net_players[5];

extern struct WeaponsFourPack net_agents__FourPacks[8][4]; // maybe a part of larger struct, maybe not

TbResult LbNetworkReadConfig(const char *fname);
TbResult LbNetworkSetSessionCreateFunction(void *func);
TbResult LbNetworkSetSessionJoinFunction(void *func);
TbResult LbNetworkSetSessionExchangeFunction(void *func);
TbResult LbNetworkSetSessionUnk2CFunction(void *func);
TbResult LbNetworkSetSessionInitFunction(void *func);
TbResult LbNetworkSetSessionDialFunction(void *func);
TbResult LbNetworkSetSessionAnswerFunction(void *func);
TbResult LbNetworkSetSessionHangUpFunction(void *func);
TbResult LbNetworkSetTimeoutSec(ulong tmsec);

int LbNetworkSessionNumberPlayers(void);
TbBool LbNetworkSessionActive(void);
TbResult LbNetworkHostPlayerNumber(void);
TbResult LbNetworkSetupIPXAddress(ulong addr);
TbResult LbNetworkPlayerNumber(void);
TbResult LbNetworkExchange(void *a1, int a2);
TbResult LbNetworkReset(void);
TbResult LbNetworkHangUp(void);
TbResult LbNetworkSessionStop(void);
TbResult LbNetworkShutDownListeners(void);

TbResult LbModemReadConfig(const char *fname);

int my_net_session_callback();
void net_system_init0(void);
void net_system_init2(void);
void net_system_reset(void);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
