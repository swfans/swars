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

#pragma pack()
/******************************************************************************/
extern ubyte data_1c4a70;
extern struct TbNetworkService NetworkServicePtr;
extern ulong NetTimeoutTicks;

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

TbResult LbModemReadConfig(const char *fname);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
