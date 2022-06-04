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

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

typedef void *NSERV_HANDLE;

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

struct NetworkPlayer { // sizeof=26
    ubyte npfield_0;
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
    sbyte npfield_19;
};

#pragma pack()
/******************************************************************************/
extern ubyte data_1c4a70;
extern ubyte net_unkn_pos_01b;
extern ubyte net_players_num;
extern struct TbNetworkService NetworkServicePtr;
extern ulong NetTimeoutTicks;
extern struct NetworkPlayer network_players[8];

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

TbResult LbNetworkSessionNumberPlayers(void);
TbResult LbNetworkSetupIPXAddress(ulong addr);
TbResult LbNetworkPlayerNumber(void);
TbResult LbNetworkExchange(void *a1, int a2);

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
