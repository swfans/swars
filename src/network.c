/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file network.c
 *     Network support.
 * @par Purpose:
 *     Implement functions for handling multiplayer exchange.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 May 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "network.h"

#include "bfkeybd.h"
#include "display.h"
/******************************************************************************/
extern ubyte lbICommSessionActive;


TbResult LbNetworkReadConfig(const char *fname)
{
    return LbModemReadConfig(fname);
}

TbResult LbNetworkSetSessionCreateFunction(void *func)
{
    NetworkServicePtr.F.SessionCreate = func;
    return Lb_SUCCESS;
}

TbResult LbNetworkSetSessionJoinFunction(void *func)
{
    NetworkServicePtr.F.SessionJoin = func;
    return Lb_SUCCESS;
}

TbResult LbNetworkSetSessionExchangeFunction(void *func)
{
    NetworkServicePtr.F.SessionExchange = func;
    return Lb_SUCCESS;
}

TbResult LbNetworkSetSessionUnk2CFunction(void *func)
{
    NetworkServicePtr.F.netsvcfunc_unkn20 = func;
    return Lb_SUCCESS;
}

TbResult LbNetworkSetSessionInitFunction(void *func)
{
    NetworkServicePtr.F.SessionInit = func;
    return Lb_SUCCESS;
}

TbResult LbNetworkSetSessionDialFunction(void *func)
{
    NetworkServicePtr.F.SessionDial = func;
    return Lb_SUCCESS;
}

TbResult LbNetworkSetSessionAnswerFunction(void *func)
{
    NetworkServicePtr.F.SessionAnswer = func;
    return Lb_SUCCESS;
}

TbResult LbNetworkSetSessionHangUpFunction(void *func)
{
    NetworkServicePtr.F.SessionHangUp = func;
    return Lb_SUCCESS;
}

TbResult LbNetworkSetTimeoutSec(ulong tmsec)
{
    NetTimeoutTicks = 100 * tmsec;
    return Lb_SUCCESS;
}

int ipx_get_host_player_number(void)
{
    int ret;
    asm volatile ("call ASM_ipx_get_host_player_number\n"
        : "=r" (ret) : );
    return ret;
}

void ipx_shutdown(ushort a1)
{
    asm volatile ("call ASM_ipx_shutdown\n"
        : : "a" (a1));
}

int ipx_stop_network(void)
{
    int ret;
    asm volatile ("call ASM_ipx_stop_network\n"
        : "=r" (ret) : );
    return ret;
}

int LbCommStopExchange(ubyte a1)
{
    int ret;
    asm volatile ("call ASM_LbCommStopExchange\n"
        : "=r" (ret) : "a" (a1) );
    return ret;
}

int LbCommDeInit(void *a1)
{
    int ret;
    asm volatile ("call ASM_LbCommDeInit\n"
        : "=r" (ret) : "a" (a1) );
    return ret;
}

TbResult LbNetworkSessionNumberPlayers(void)
{
#if 1
    TbResult ret;
    asm volatile ("call ASM_LbNetworkSessionNumberPlayers\n"
        : "=r" (ret) : );
    return ret;
#else
    struct TbSerialDev *serhead;
    switch (NetworkServicePtr.Type)
    {
    case NetSvc_IPX:
        return (ubyte)IPXPlayerHeader.field_10D;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        serhead = NetworkServicePtr.Id;
        return serhead->num_players;
    default:
        break;
    }
    return Lb_FAIL;
#endif
}

TbResult LbNetworkSessionStop(void)
{
    struct TbSerialDev *serhead;
    switch (NetworkServicePtr.Type)
    {
    case NetSvc_IPX:
        ipx_stop_network();
        return Lb_SUCCESS;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        serhead = NetworkServicePtr.Id;
        LbCommStopExchange(serhead->comdev_id);
        lbICommSessionActive = 0;
        return Lb_SUCCESS;
    default:
        break;
    }
    return Lb_FAIL;
}

TbResult LbNetworkHostPlayerNumber(void)
{
    struct TbSerialDev *serhead;

    switch (NetworkServicePtr.Type)
    {
    case NetSvc_IPX:
        return ipx_get_host_player_number();
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        serhead = NetworkServicePtr.Id;
        if (!serhead->field_10A9)
            return 0;
        return 1;
    default:
        break;
    }
    return Lb_FAIL;
}

TbResult LbNetworkSetupIPXAddress(ulong addr)
{
#if 1
    TbResult ret;
    asm volatile ("call ASM_LbNetworkSetupIPXAddress\n"
        : "=r" (ret) : "a" (addr) );
    return ret;
#else
    if ( !addr )
        return Lb_FAIL;
    IPXHandler = addr;
    return Lb_SUCCESS;
#endif
}

TbResult LbNetworkPlayerNumber(void)
{
    TbResult ret;
    asm volatile ("call ASM_LbNetworkPlayerNumber\n"
        : "=r" (ret) : );
    return ret;
}

TbResult LbNetworkExchange(void *a1, int a2)
{
    TbResult ret;
    asm volatile ("call ASM_LbNetworkExchange\n"
        : "=r" (ret) : "a" (a1), "d" (a2));
    return ret;
}

TbResult LbNetworkReset(void)
{
    TbResult ret;
#if 1
    asm volatile ("call ASM_LbNetworkReset\n"
        : "=r" (ret) : );
    return ret;
#else
    ret = Lb_FAIL;
    switch (NetworkServicePtr.Type)
    {
    case NetSvc_IPX:
        ret = ipx_shutdown(*(ushort *)(IPXHandler + 8));
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        ret = LbCommDeInit(NetworkServicePtr.Id);
        break;
    case NetSvc_Unkn6:
        ret = netsvc6_shutdown();
        break;
    }
    NetworkServicePtr.Type = NetSvc_NONE;
    return ret;
#endif
}

TbResult LbModemHangUp(ushort dev_id)
{
    TbResult ret;
    asm volatile ("call ASM_LbModemHangUp\n"
        : "=r" (ret) : "a" (dev_id));
    return ret;
}

TbResult LbNetworkHangUp(void)
{
    struct TbSerialDev *serhead;
    switch (NetworkServicePtr.Type)
    {
    case NetSvc_IPX:
        return Lb_FAIL;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        serhead = NetworkServicePtr.Id;
        return LbModemHangUp(serhead->comdev_id);
    default:
        break;
    }
    return Lb_FAIL;
}

TbResult LbModemReadConfig(const char *fname)
{
    TbResult ret;
    asm volatile ("call ASM_LbModemReadConfig\n"
        : "=r" (ret) : "a" (fname));
    return ret;
}

int my_net_session_callback()
{
    swap_wscreen();
    if (lbKeyOn[KC_ESCAPE])
        return -7;
    return 0;
}

void net_system_init0(void)
{
    LbNetworkSetSessionCreateFunction(my_net_session_callback);
    LbNetworkSetSessionJoinFunction(my_net_session_callback);
    LbNetworkSetSessionUnk2CFunction(my_net_session_callback);
    LbNetworkSetSessionInitFunction(my_net_session_callback);
    LbNetworkSetSessionDialFunction(my_net_session_callback);
    LbNetworkSetSessionAnswerFunction(my_net_session_callback);
    LbNetworkSetSessionHangUpFunction(my_net_session_callback);
    if (LbNetworkReadConfig("modem.cfg") != Lb_FAIL)
        data_1c4a70 = 1;
}

void net_system_init2(void)
{
    LbNetworkSetSessionCreateFunction(my_net_session_callback);
    LbNetworkSetSessionJoinFunction(my_net_session_callback);
    LbNetworkSetSessionExchangeFunction(my_net_session_callback);
    LbNetworkSetSessionInitFunction(my_net_session_callback);
    LbNetworkSetSessionDialFunction(my_net_session_callback);
    LbNetworkSetSessionAnswerFunction(my_net_session_callback);
    LbNetworkSetSessionHangUpFunction(my_net_session_callback);
    if (LbNetworkReadConfig("modem.cfg") != Lb_FAIL)
        data_1c4a70 = 1;
}

void net_system_reset(void)
{
    LbNetworkSetSessionCreateFunction(NULL);
    LbNetworkSetSessionJoinFunction(NULL);
    LbNetworkSetSessionExchangeFunction(NULL);
    LbNetworkSetSessionInitFunction(NULL);
    LbNetworkSetSessionDialFunction(NULL);
    LbNetworkSetSessionAnswerFunction(NULL);
    LbNetworkSetSessionHangUpFunction(NULL);
}
/******************************************************************************/
