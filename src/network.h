/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
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
typedef void *NSESS_HANDLE;
typedef short NPLYR_HANDLE;
typedef int (*NSVC_SESSIONCB) ();

enum NetworkServiceType {
    NetSvc_NONE = 0x0,
    NetSvc_IPX,
    NetSvc_COM1,
    NetSvc_COM2,
    NetSvc_COM3,
    NetSvc_COM4,
    NetSvc_RADICA,
};

struct NetworkServiceInfo { // sizeof=12
    NSERV_HANDLE Id; // offset=0
    ushort GameId; // offset=4
    ushort Type; // offset=6
    ushort Flags; // offset=8
    short Param; // offset=10
};

struct NetworkServiceFunction { // sizeof=36
    NSVC_SESSIONCB SessionCreate; // offset=0
    NSVC_SESSIONCB SessionJoin;
    NSVC_SESSIONCB SessionExchange;
    NSVC_SESSIONCB SessionInit;
    NSVC_SESSIONCB SessionDial; // offset=16
    NSVC_SESSIONCB SessionAnswer;
    NSVC_SESSIONCB SessionHangUp;
    NSVC_SESSIONCB UsedSessionInit;
    NSVC_SESSIONCB netsvcfunc_unkn20; // offset=32
};

struct TbNetworkSession { // sizeof=40
    NSESS_HANDLE Id; // offset=0
    ulong GameId; // offset=2
    char Name[8]; // offset=6
    short HostPlayerNumber; // offset=14
    short MaxPlayers; // offset=16
    short Flags; // offset=18
    ubyte Reserved[20]; // offset=20
};

struct TbNetworkService { // sizeof=10
    struct NetworkServiceInfo I; // offset=0
    union {
    struct NetworkServiceFunction F; // offset=12
    struct TbNetworkSession S;
    };
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
  char field_10AD[31];
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
  char Magic[2];
  short field_2;
  char field_4[8];
  char field_C[16];
  ubyte field_1C[4];
  short field_20;
  ubyte field_22[4];
  long field_26;
  ubyte field_2A;
  ubyte field_2B;
  ubyte field_2C;
};

struct TbIPXPlayerData1Sub {
    short field_47;
    short field_49[13];
};

struct TbIPXPlayerData1 {
    ubyte field_2D[26]; // offset=45
    struct TbIPXPlayerData1Sub Sub1[2]; // offset=71
    short field_7F[71];
};

struct TbIPXPlayerData2Sub {
    ubyte field_2D[14];
};

struct TbIPXPlayerData2 {
    struct TbIPXPlayerData2Sub Sub1[2]; // offset=45
    short field_49[98];
};

struct TbIPXPlayerData3Sub {
    ubyte field_2D[28];
};

struct TbIPXPlayerData3 {
    struct TbIPXPlayerData3Sub Sub1[2]; // offset=45
};

struct TbIPXPlayerData { // sizeof=226
    union {
        struct TbIPXPlayerData1 Data1;
        struct TbIPXPlayerData2 Data2;
        struct TbIPXPlayerData3 Data3;
    };
    ubyte num_players;
    ubyte field_10E; // offset=225 (within TbIPXPlayer offset=270)
};

struct TbIPXPlayer { // sizeof=271
    struct TbIPXPlayerHeader Header; // offset=0
    struct TbIPXPlayerData Data; // offset=45
};

// This was either untyped buffer which is casted on types,
// or an union; not sure
struct TbIPXPlayerEx { // sizeof=540
    struct TbIPXPlayer P; // offset=0
    ubyte field_10F[268]; // offset=272
    ubyte field_21B;
};

struct TbIPXHandler {
  short InterruptNo;
  short field_2;
  short PlayerDataSize;
  short field_6;
  ushort field_8;
  /** Network session state; 0 - not ready, 1 - active, 2 - active but down
   */
  ubyte SessionActive;
  ubyte field_B;
  ubyte field_C;
  ubyte field_D;
  ubyte field_E[4];
  ubyte field_12[24];
  long field_2A;
  ubyte field_2E[4];
  short field_32;
  char field_34[16];
  short field_44;
  ubyte field_46[30];
  struct TbIPXPlayerEx PlayerData[30];
};

struct IPXDatagramBackup {
  long field_0;
  long field_4;
  short field_8;
  ubyte field_A[538];
  short field_224;
};

struct TbNetworkPlayer { // sizeof=22
    NPLYR_HANDLE Id; // offset=0
    ulong PlayerNumber; // offset=2
    char Name[16]; // offset=6
};

struct TbNetworkSessionList { // sizeof=218
    struct TbNetworkSession Session; // offset=0
    struct TbNetworkPlayer Player[8];
    short NumberOfPlayers; // offset=216
};

#pragma pack()
/******************************************************************************/
extern ubyte data_1c4a70;
extern ubyte net_players_num;
extern struct TbNetworkService NetworkServicePtr;
extern ulong NetTimeoutTicks;
extern struct NetworkPlayer network_players[8];
extern struct NetPlayer2 net_players[5];
extern struct TbNetworkSessionList unkstruct04_arr[20];
// Application-accessible copy of the service structure?
extern struct TbNetworkService nsvc;

extern struct WeaponsFourPack net_agents__FourPacks[8][4]; // maybe a part of larger struct, maybe not

TbResult LbNetworkReadConfig(const char *fname);
TbResult LbNetworkSetBaud(int rate);
TbResult LbNetworkSessionCreate(struct TbNetworkSession *session, char *a2);
TbResult LbNetworkSessionJoin(struct TbNetworkSession *session, char *a2);
TbResult LbNetworkSetTimeoutSec(ulong tmsec);

TbResult LbNetworkSetSessionCreateFunction(void *func);
TbResult LbNetworkSetSessionJoinFunction(void *func);
TbResult LbNetworkSetSessionExchangeFunction(void *func);
TbResult LbNetworkSetSessionUnk2CFunction(void *func);
TbResult LbNetworkSetSessionInitFunction(void *func);
TbResult LbNetworkSetSessionDialFunction(void *func);
TbResult LbNetworkSetSessionAnswerFunction(void *func);
TbResult LbNetworkSetSessionHangUpFunction(void *func);

TbResult LbNetworkInit(void);
TbResult LbNetworkServiceStart(struct NetworkServiceInfo *nsvc);

int LbNetworkSessionNumberPlayers(void);
TbBool LbNetworkSessionActive(void);
TbResult LbNetworkHostPlayerNumber(void);
TbResult LbNetworkSetupIPXAddress(ulong addr);
TbResult LbNetworkPlayerNumber(void);
TbResult LbNetworkExchange(void *a1, int a2);
TbResult LbNetworkReset(void);
TbResult LbNetworkDial(const char *distr);
TbResult LbNetworkAnswer(void);
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
