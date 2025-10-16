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

#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "bfdos.h"
#include "bfmemory.h"
#include "bftime.h"

#include "display.h"
#include "keyboard.h"
#include "swlog.h"
/******************************************************************************/
#pragma pack(1)

struct IPXSession { // sizeof=45
    NSESS_HANDLE Id; // offset=0
    ushort GameId; // offset=2
    char Name[8]; // offset=4
    ubyte Reserved[31]; // offset=12
    ubyte HostPlayerNumber; // offset=43
    ubyte field_44; // offset=44
};

struct IPXPlayer { // sizeof=28
    ulong Res0; // offset=0
    ulong Res4; // offset=4
    ushort Res8; // offset=8
    char Name[16]; // offset=10
    ushort Used; // offset=26
};

struct IPXSessionList { // sizeof=271
    struct IPXSession Session; // offset=0
    struct IPXPlayer Player[8]; // offset=45
    ubyte NumberOfPlayers; // offset=269
    ubyte field_270; // offset=270
};

struct ModemResponse { // sizeof=0x2C
    char msg[40];
    ulong code;
};

#pragma pack()
/******************************************************************************/

ubyte lbICommSessionActive;
extern struct TbIPXHandler *IPXHandler;
extern struct TbIPXPlayer IPXPlayer;

char ModemResponseString[80];
char ModemRequestString[80];
extern ubyte byte_1E81E0[1027];

extern ulong ipx_send_packet_count[8][8];
extern ulong ipx_got_player_send_packet_count[8];
extern struct TbIPXPlayerHeader loggon_header;
extern ubyte player_loggon;

ubyte net_players_num = 1;

extern struct ComHandlerInfo com_dev[4];
extern struct IPXDatagramBackup datagram_backup[8];

struct TbNetworkService NetworkServicePtr;

extern struct TbSerialDev *data_1e85e3;

const struct ModemResponse modem_response[] = {
    {"OK", 1},
    {"CONNECT", 2},
    {"RING", 3},
    {"BUSY", 4},
    {"ERROR", 5},
    {"NO CARRIER", 6},
    {"NO DIALTONE", 7},
    {"", 0},
};

struct ModemCommand modem_cmds[] = {
    {"ATZ"},
    {"ATD"},
    {"ATH"},
    {"ATS0=1"},
};

struct NetworkServiceInfo Network_Service_List[] = {
    {0, 0, NetSvc_IPX, 0x02, 0},
    {0, 0, NetSvc_COM1, 0x01, 0},
    {0, 0, NetSvc_COM2, 0x01, 0},
    {0, 0, NetSvc_COM3, 0x01, 0},
    {0, 0, NetSvc_COM4, 0x01, 0},
    {0, 0, NetSvc_RADICA, 0x02, 0},
    {0, 0, 0, 0, 0},
};

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

TbBool ipx_is_initialized(void)
{
    return (IPXHandler != NULL);
}

#if defined(DOS)||defined(GO32)

int CallIPX(ubyte a1)
{
    struct DPMI_REGS dpmi_regs;
    union REGS regs;
    struct SREGS sregs;

    IPXHandler->field_2 = a1;
    memset(&dpmi_regs, 0, sizeof(struct DPMI_REGS));
    memset(&regs, 0, sizeof(union REGS));
    /* Use DMPI call 300h to issue the DOS interrupt */
    regs.x.eax = 0x300;
    regs.x.ebx = IPXHandler->InterruptNo;
    segread(&sregs);
    regs.x.edi = (unsigned int)&dpmi_regs;
    return int386x(0x31, &regs, &regs, &sregs);
}

static ubyte CallRealModeInterrupt(ubyte intno, struct DPMI_REGS *dpmi_regs)
{
    union REGS regs;
    struct SREGS sregs;

    memset(&regs, 0, sizeof(union REGS));
    /* Use DMPI call 300h to issue the DOS interrupt */
    regs.x.eax = 0x300;
    regs.x.ebx = intno;
    segread(&sregs);
    regs.x.edi = (unsigned int)dpmi_regs;
    int386x(49, &regs, &regs, &sregs);
    return dpmi_regs->eax;
}

#endif

void setup_bullfrog_header(struct TbIPXPlayerHeader *ipxhead, int a2)
{
    struct TbIPXHandler *ipxhndl;

    ipxhndl = IPXHandler;
    ipxhead->Magic[0] = 'B';
    ipxhead->Magic[1] = 'U';
    ipxhead->field_2A = a2;
    memcpy(ipxhead->field_1C, ipxhndl->field_2E, sizeof(ipxhead->field_1C));
    memcpy(&ipxhead->field_20, &ipxhndl->field_32, sizeof(ipxhead->field_20));
    memcpy(ipxhead->field_22, &ipxhndl->field_2A, sizeof(ipxhead->field_22));
}

void ipx_add_new_player(struct TbIPXPlayerHeader *p_ipxhead)
{
    asm volatile ("call ASM_ipx_add_new_player\n"
        : : "a" (p_ipxhead) );
}

void ipx_log_on_new_players(void)
{
    short i;
#if defined(DOS)||defined(GO32)
    CallIPX(1);
#endif
    for (i = 0; i < 30; i++)
    {
        struct TbIPXPlayer *p_ipxplyr;

        if (IPXHandler->field_46[i] == 0)
            continue;

        IPXHandler->field_46[i] = 0;
        p_ipxplyr = (struct TbIPXPlayer *)&IPXHandler->PlayerData[i];

        if (strncasecmp(p_ipxplyr->Header.Magic, "BU", 2) != 0)
            continue;
        if (p_ipxplyr->Header.field_2A == 2)
            continue;

        if (IPXPlayer.Data.num_players < 8) {
            ipx_add_new_player(&p_ipxplyr->Header);
        }
    }
}

void ipx_update(void)
{
    static TbClockMSec start_time = 0;
    TbClockMSec curr_time;

    if (IPXHandler->SessionActive != 1) {
        return;
    }
    if (IPXHandler->field_C != IPXHandler->field_D) {
        return;
    }
    if (start_time == 0)
        start_time = LbTimerClock();
    curr_time = LbTimerClock();
    if (curr_time - start_time > 100)
    {
        start_time = curr_time;
        IPXPlayer.Header.field_2A = 1;
        memcpy(&IPXHandler->PlayerData[0], &IPXPlayer, sizeof(struct TbIPXPlayer));
        IPXHandler->PlayerDataSize = sizeof(struct TbIPXPlayer);
#if defined(DOS)||defined(GO32)
        CallIPX(3);
#endif
    }

    if (IPXPlayer.Data.num_players == 1)
    {
        ipx_log_on_new_players();
    }
    else if (IPXPlayer.Data.num_players > 1)
    {
        if (player_loggon) {
            player_loggon = 0;
            ipx_add_new_player(&loggon_header);
        }
    }
}

void ipx_service_init(ushort a1)
{
    struct TbIPXHandler *ipxhndl;

    LOGDBG("Starting");
    ipxhndl = IPXHandler;
    ipxhndl->field_8 = a1;
#if defined(DOS)||defined(GO32)
    CallIPX(8);
#endif
}


TbResult ipx_create_session(char *a1, const char *a2)
{
    struct TbIPXHandler *ipxhndl;
    struct TbIPXPlayer *p_plyrdt;
    ulong tm_start, tm_curr;
    TbResult ret;
    int i;

    LOGDBG("Starting");
    if (IPXHandler->SessionActive != 0) {
        LOGERR("Already have IPX session");
        return Lb_FAIL;
    }

    tm_start = clock();
    while (1)
    {
        tm_curr = clock();
#if defined(DOS)||defined(GO32)
        CallIPX(1);
#endif
        for (i = 1; i < 30; i++)
        {
            ipxhndl = IPXHandler;
            if (!ipxhndl->field_46[i])
                continue;
            ipxhndl->field_46[i] = 0;

            p_plyrdt = (struct TbIPXPlayer *)&ipxhndl->PlayerData[i];
            if (IPXPlayer.Header.field_2 == p_plyrdt->Header.field_2)
            {
                if (strcasecmp(p_plyrdt->Header.field_4, a1) == 0) {
                    LOGERR("String same as remote");
                    return Lb_FAIL;
                }
            }
        }

        ret = 0;
        if (NetworkServicePtr.F.SessionCreate != NULL)
            ret = NetworkServicePtr.F.SessionCreate();
        if (ret == -7) {
          LOGERR("Service callback error %d", (int)ret);
          return ret;
        }

        if (tm_curr - tm_start >= 300)
            break;
    }

    for (i = 0; i < 16; i++)
    {
        ubyte val;

        val = a2[i];
        if (val == 0)
        {
              IPXHandler->field_34[i] = 0;
              break;
        }
        IPXHandler->field_34[i] = val;
    }

    ipxhndl = IPXHandler;
    IPXHandler->field_34[15] = 0;
    setup_bullfrog_header(&IPXPlayer.Header, 1);
    IPXHandler->field_44 = 1;

    if (strlen(a1) > 7)
            a1[7] = 0;
    strcpy(IPXPlayer.Header.field_4, a1);

    memcpy(&IPXPlayer.Data, &IPXHandler->field_2A, 0x1Cu);
    strcpy(IPXPlayer.Header.field_C, IPXHandler->field_34);

    memset(ipx_send_packet_count, 0, 0x100u);
    memset(ipx_got_player_send_packet_count, 0, 0x20u);
    memset(datagram_backup, 0, sizeof(struct IPXDatagramBackup) * 8);

    ipxhndl = IPXHandler;
    ipxhndl->SessionActive = 1;
    ipxhndl->field_C = 0;
    ipxhndl->field_D = 0;
    IPXPlayer.Data.num_players = 1;
    IPXPlayer.Header.field_26 = 0;

    return Lb_SUCCESS;
}

int ipx_session_list(struct IPXSessionList *sesslist, int listlen)
{
    int ret;
    int i, k;
    ushort n;

    if (IPXHandler->SessionActive != 0) {
        LOGERR("Already have IPX session");
        return Lb_FAIL;
    }

#if defined(DOS)||defined(GO32)
    CallIPX(1);
#endif

    ret = -1;
    n = 0;
    for (i = 1; i < 30; i++)
    {
        struct IPXSessionList *p_ipxsess;
        struct TbIPXPlayer *p_plyrdt;

        if (!IPXHandler->field_46[i])
            continue;
        IPXHandler->field_46[i] = 0;

        p_plyrdt = (struct TbIPXPlayer *)&IPXHandler->PlayerData[i];

        if (strncasecmp(p_plyrdt->Header.Magic, "BU", 2) != 0)
            continue;
        if (p_plyrdt->Header.field_2 != IPXPlayer.Header.field_2)
            continue;
        if (p_plyrdt->Header.field_2A != 1)
            continue;

        for (k = 0; k < n; k++)
        {
            p_ipxsess = &sesslist[k];
            if (memcmp(p_plyrdt->Header.field_1C, &p_ipxsess->Session.Reserved[16], 6) == 0)
            {
                k = -1;
                memcpy(p_ipxsess, p_plyrdt, sizeof(struct IPXSessionList));
                break;
            }
        }
        if (k != -1)
        {
            p_ipxsess = &sesslist[n];
            memcpy(p_ipxsess, p_plyrdt, sizeof(struct IPXSessionList));
            ret = ++n;
            if (n >= listlen)
                break;
        }
    }

    return ret;
}

int ipx_join_session(struct IPXSessionList *p_ipxsess, char *a2)
{
    struct TbIPXPlayerHeader ipxhead;
    ulong tm_start, tm_curr;
    TbResult ret;
    short i, k;

    LOGDBG("Starting");
    if (IPXHandler->SessionActive != 0) {
        LOGERR("Already have IPX session");
        return Lb_FAIL;
    }

    ret = Lb_FAIL;
    memset(&ipxhead, 0, 45u);
    ipxhead.Magic[0] = 'B';
    ipxhead.Magic[1] = 'U';
    ipxhead.field_2A = 2;
    memcpy(ipxhead.field_1C, IPXHandler->field_2E, sizeof(ipxhead.field_1C));
    memcpy(&ipxhead.field_20, &IPXHandler->field_32, sizeof(ipxhead.field_20));
    memcpy(ipxhead.field_22, &IPXHandler->field_2A, sizeof(ipxhead.field_22));
    strcpy(ipxhead.field_C, a2);
    strcpy(ipxhead.field_4, p_ipxsess->Session.Name);
    ipxhead.field_2 = IPXPlayer.Header.field_2;

    tm_start = clock();
    while ( 1 )
    {
        memcpy(IPXHandler->PlayerData, &ipxhead, sizeof(struct TbIPXPlayerHeader));
        IPXHandler->field_B = 45;
        memcpy(IPXHandler->field_12, &p_ipxsess->Session.Reserved[16], 6u);
        memcpy(IPXHandler->field_E, &p_ipxsess->Session.Reserved[22], sizeof(IPXHandler->field_E));
#if defined(DOS)||defined(GO32)
        CallIPX(2);
        CallIPX(1);
#endif

        for (i = 1; i < 30; i++)
        {
            struct TbIPXPlayer *p_plyrdt;

            if (!IPXHandler->field_46[i])
                continue;
            IPXHandler->field_46[i] = 0;

            p_plyrdt = (struct TbIPXPlayer *)&IPXHandler->PlayerData[i];

            if (strncasecmp(p_plyrdt->Header.Magic, "BU", 2) != 0)
                continue;
            if (p_plyrdt->Header.field_2 != IPXPlayer.Header.field_2)
                continue;
            if (p_plyrdt->Header.field_2A != 1)
                continue;
            if (strcasecmp(p_plyrdt->Header.field_4, p_ipxsess->Session.Name) != 0)
                continue;

            for (k = 0; k < 8; k++)
            {
                struct TbIPXPlayerData3Sub *p_pdtsub;
                p_pdtsub = &p_plyrdt->Data.Data3.Sub1[k];
                if (memcmp(&p_pdtsub->field_2D[4], ipxhead.field_1C, 6) == 0)
                {
                    tm_start = 0;
                    ret = 1;
                    i = 31;
                    break;
                }
            }
        }

        if (ret != 1)
        {
            int ret2;
            ret2 = 0;
            if (NetworkServicePtr.F.SessionJoin)
                ret2 = NetworkServicePtr.F.SessionJoin();
            if (ret2 == -7)
                return ret2;
        }
        tm_curr = clock();
        if (tm_curr - tm_start > 500)
        {
            break;
        }
    }
    if (ret != -1)
    {
        struct TbIPXPlayer *p_plyrdt;

        p_plyrdt = (struct TbIPXPlayer *)&IPXHandler->PlayerData[i];

        IPXPlayer.Header.Magic[0] = 'B';
        IPXPlayer.Header.Magic[1] = 'U';
        IPXPlayer.Header.field_2A = 0;
        memcpy(IPXPlayer.Header.field_1C, IPXHandler->field_2E, sizeof(IPXPlayer.Header.field_1C));
        memcpy(&IPXPlayer.Header.field_20, IPXHandler->field_2E + 4, sizeof(IPXPlayer.Header.field_20));
        memcpy(IPXPlayer.Header.field_22, &IPXHandler->field_2A, sizeof(IPXPlayer.Header.field_22));
        IPXHandler->field_C = k;
        IPXHandler->SessionActive = 1;
        IPXHandler->field_D = p_plyrdt->Header.field_2B;
        IPXPlayer.Data.num_players = p_plyrdt->Data.num_players;
        IPXPlayer.Data.field_10E = p_plyrdt->Data.field_10E;
        IPXPlayer.Header.field_26 = p_plyrdt->Header.field_26;
        IPXPlayer.Header.field_2B = k;
        strcpy(IPXPlayer.Header.field_C, a2);
        strcpy(IPXPlayer.Header.field_4, p_plyrdt->Header.field_4);
        memcpy(&IPXPlayer.Data, &p_plyrdt->Data, 0xE0u);
        memset(ipx_send_packet_count, 0, 0x100u);
        memset(ipx_got_player_send_packet_count, 0, 0x20u);
        memset(datagram_backup, 0, 0x1130u);
    }
    return ret;
}

int ipx_get_host_player_number(void)
{
    if (!ipx_is_initialized()) {
        LOGERR("Called before IPX initialization");
        return -1;
    }
    if (IPXHandler->SessionActive == 0) {
        LOGERR("Called without active session");
        return -1;
    }
    return IPXHandler->field_D;
}

int ipx_get_player_number(void)
{
    if (!ipx_is_initialized()) {
        LOGERR("Called before IPX initialization");
        return -1;
    }
    if (IPXHandler->SessionActive == 0) {
        LOGERR("Called without active session");
        return -1;
    }
    return IPXPlayer.Header.field_2B;
}

int ipx_exchange_packets(void *a1, int a2)
{
    int ret;
    asm volatile ("call ASM_ipx_exchange_packets\n"
        : "=r" (ret) : "a" (a1), "d" (a2) );
    return ret;
}

void ipx_shutdown(ushort a1)
{
    struct TbIPXHandler *p_ipxhndl;
    int i;

    LOGDBG("Starting");
    p_ipxhndl = IPXHandler;

    p_ipxhndl->field_8 = a1;
#if defined(DOS)||defined(GO32)
    CallIPX(9);

    for (i = 0; i < 40; i++)
    {
        CallIPX(1);
    }
#else
    (void)i; // unused
#endif
}

void ipx_shutdown_listeners(void)
{
    LOGDBG("Starting");
    if (!ipx_is_initialized()) {
        LOGERR("Called before IPX initialization");
        return;
    }
    if (IPXHandler->SessionActive == 0) {
        LOGERR("Called without active session");
        return;
    }
    IPXHandler->SessionActive = 2;
}

void ipx_openup_listeners(void)
{
    LOGDBG("Starting");
    if (!ipx_is_initialized()) {
        LOGERR("Called before IPX initialization");
        return;
    }
    if (IPXHandler->SessionActive == 0) {
        LOGERR("Called without active session");
        return;
    }
    IPXHandler->SessionActive = 1;
}

int ipx_stop_network(void)
{
    int ret;
    LOGDBG("Starting");
    asm volatile ("call ASM_ipx_stop_network\n"
        : "=r" (ret) : );
    return ret;
}

TbResult ipx_get_player_name(char *name, int plyr)
{
    int ret;
    asm volatile ("call ASM_ipx_get_player_name\n"
        : "=r" (ret) : "a" (name), "d" (plyr) );
    return ret;
}

TbResult ipx_network_send(int plyr, ubyte *data, int dtlen)
{
    assert(!"Not implemented");
    return Lb_FAIL;
}

TbResult ipx_network_receive(int plyr, ubyte *data, int dtlen)
{
    assert(!"Not implemented");
    return Lb_FAIL;
}

TbResult ipx_send_packet_to_player_nowait(int plyr, ubyte *data, int dtlen)
{
    struct TbIPXHandler *p_ipxhndl;
    struct TbIPXPlayer *p_plyrdt;

    LOGDBG("Starting");
    if (plyr >= 8) {
        LOGERR("Target player invalid");
        return Lb_FAIL;
    }
    if (IPXPlayer.Data.Data1.Sub1[plyr].field_47) {
        LOGERR("Cond 1 triggered");
        return Lb_FAIL;
    }

    p_ipxhndl = IPXHandler;
    p_plyrdt = (struct TbIPXPlayer *)&IPXHandler->PlayerData[0];

    IPXPlayer.Header.field_2A = 4;
    memcpy(&p_plyrdt->Header, &IPXPlayer.Header, sizeof(struct TbIPXPlayerHeader));
    memcpy(&p_plyrdt->Data, data, dtlen);
    p_ipxhndl->PlayerDataSize = sizeof(struct TbIPXPlayerHeader) + dtlen;

    memcpy(&p_ipxhndl->field_12[0], &IPXPlayer.Data.Data2.Sub1[plyr].field_2D[4], 6);
    memcpy(p_ipxhndl->field_E, &IPXPlayer.Data.Data2.Sub1[plyr].field_2D[0], sizeof(p_ipxhndl->field_E));
#if defined(DOS)||defined(GO32)
    CallIPX(2);
#endif
    return Lb_SUCCESS;
}

TbResult ipx_receive_packet_from_player_nowait(int plyr, ubyte *data, int dtlen)
{
    TbResult ret;
    int i;
  
    ret = Lb_OK;
    for (i = 1; i < 30; i++)
    {
        struct TbIPXPlayer *p_plyrdt;

        if (!IPXHandler->field_46[i])
            continue;
        IPXHandler->field_46[i] = 0;

        p_plyrdt = (struct TbIPXPlayer *)&IPXHandler->PlayerData[i];

        if (strncasecmp(p_plyrdt->Header.Magic, "BU", 2) != 0)
            continue;
        if (strcasecmp(p_plyrdt->Header.field_4, IPXPlayer.Header.field_4) != 0)
            continue;
        if (p_plyrdt->Header.field_2 != IPXPlayer.Header.field_2)
            continue;
        if ((p_plyrdt->Header.field_2A != 4) || (p_plyrdt->Header.field_2B != plyr))
            continue;

        memcpy(data, &p_plyrdt->Data, dtlen);
        ret = Lb_SUCCESS;
        break;
    }
    return ret;
}

TbResult ipx_send_packet_to_player_wait(int plyr, ubyte *data, int dtlen)
{
    ubyte *dt;
    int i;
    ushort pkt_count;

    LOGDBG("Starting");
    if (!IPXPlayer.Header.field_2C) {
        LOGERR("Cond 1 not met");
        return Lb_OK;
    }
    if ((plyr >= 8 && plyr != 0xFFFF)
      || (plyr == IPXPlayer.Header.field_2B)
      || (plyr != 0xFFFF && !IPXPlayer.Data.Data1.Sub1[plyr].field_47) )
    {
        LOGERR("Target player invalid");
        return Lb_OK;
    }

    dt = data;
    pkt_count = dtlen / 400;
    for (i = 0; i < pkt_count; i++)
    {
        ipx_network_send(plyr, dt, 400);
        dt += 400;
    }
    if (dtlen % 400)
    {
        ipx_network_send(plyr, dt, dtlen % 400);
    }
    return Lb_SUCCESS;
}

TbResult ipx_receive_packet_from_player_wait(int plyr, ubyte *data, int dtlen)
{
    ubyte *dt;
    int i;
    ushort pkt_count;

    LOGDBG("Starting");
    if (!IPXPlayer.Header.field_2C) {
        LOGERR("Cond 1 not met");
        return Lb_OK;
    }
    if ((plyr >= 8)
      || (plyr == IPXPlayer.Header.field_2B)
      || (!IPXPlayer.Data.Data1.Sub1[plyr].field_47) )
    {
        LOGERR("Target player invalid");
        return Lb_OK;
    }

    dt = data;
    pkt_count = dtlen / 400;
    for (i = 0; i < pkt_count; i++)
    {
        ipx_network_receive(plyr, dt, 400);
        dt += 400;
    }
    if (dtlen % 400)
    {
        ipx_network_receive(plyr, dt, dtlen % 400);
    }
    return Lb_SUCCESS;
}

int ipx_session_list_conv(struct TbNetworkSessionList *nslist, int listlen)
{
    struct TbNetworkSessionList *nslent;
    struct IPXSessionList ipxsess;
    int n_ent;
    int i, k;

    memset(&ipxsess, 0, sizeof(struct IPXSessionList));

    nslent = nslist;
    for (i = 0; i < listlen; i++)
    {
        TbResult ret;

        ret = ipx_session_list(&ipxsess, 1);
        memset(nslent, 0, sizeof(struct TbNetworkSessionList));
        if (ret != Lb_SUCCESS)
            continue;
        nslent->NumberOfPlayers = ipxsess.NumberOfPlayers;
        nslent->Session.GameId = ipxsess.Session.GameId;
        nslent->Session.HostPlayerNumber = ipxsess.Session.HostPlayerNumber;
        strcpy(nslent->Session.Name, ipxsess.Session.Name);

        for (k = 0; k < 8; k++)
        {
            if (ipxsess.Player[k].Used)
            {
                nslent->Player[k].Id = 1;
                nslent->Player[k].PlayerNumber = k;
                strcpy(nslent->Player[k].Name, ipxsess.Player[k].Name);

                if (k == ipxsess.Session.HostPlayerNumber) {
                    memcpy(&nslent->Session.Reserved[0], &ipxsess.Player[k].Res0, 4);
                    memcpy(&nslent->Session.Reserved[4], &ipxsess.Player[k].Res4, 4);
                    memcpy(&nslent->Session.Reserved[8], &ipxsess.Player[k].Res8, 2);
                }
            }
        }
        n_ent++;
        nslent++;
    }
    return n_ent;
}

int net_unkn_func_352(void)
{
    int ret;
    LOGDBG("Starting");
    asm volatile ("call ASM_net_unkn_func_352\n"
        : "=r" (ret) :  );
    return ret;
}

int net_unkn_func_338(void *a1)
{
    int ret;
    LOGDBG("Starting");
    asm volatile ("call ASM_net_unkn_func_338\n"
        : "=r" (ret) : "a" (a1) );
    return ret;
}

int radica_create_session(struct TbNetworkSession *session, const char *a2)
{
    int ret;
    LOGDBG("Starting");
    asm volatile ("call ASM_radica_create_session\n"
        : "=r" (ret) : "a" (session), "d" (a2) );
    return ret;
}

int radica_join_session(struct TbNetworkSession *session, char *a2)
{
    int ret;
    LOGDBG("Starting");
    asm volatile ("call ASM_radica_join_session\n"
        : "=r" (ret) : "a" (session), "d" (a2) );
    return ret;
}

int radica_update(void)
{
    assert(!"Not implemented");
    return Lb_SUCCESS;
}

TbResult radica_service_init(struct NetworkServiceInfo *nsvc)
{
    TbResult ret;
    LOGDBG("Starting");
    asm volatile ("call ASM_radica_service_init\n"
        : "=r" (ret) : "a" (nsvc) );
    return ret;
}

int radica_session_list(struct TbNetworkSessionList *nslist, int listlen)
{
    int ret;
    LOGDBG("Starting");
    asm volatile ("call ASM_radica_session_list\n"
        : "=r" (ret) : "a" (nslist), "d" (listlen) );
    return ret;
}

int radica_exchange_packets(void *a1, int a2)
{
    int ret;
    asm volatile ("call ASM_radica_exchange_packets\n"
        : "=r" (ret) : "a" (a1), "d" (a2) );
    return ret;
}

int radica_shutdown(void)
{
    LOGSYNC("Quitting RADICA");
    return net_unkn_func_352();
}

int SetBps(struct TbSerialDev *serdev, int rate)
{
    if (rate < 300)
        rate = 300;
    else if (rate > 115200)
        rate = 115200;

#if defined(DOS)||defined(GO32)
    cli();
    outp(serdev->field_1096 + 3, 131);
    outp(serdev->field_1096 + 1, 115200 / rate >> 8);
    outp(serdev->field_1096 + 0, (115200 / rate));
    outp(serdev->field_1096 + 3, 3);
    sti();
#endif
    return 115200 / (115200 / rate);
}

#if defined(DOS)||defined(GO32)

void write_char_no_buff(struct TbSerialDev *serdev, ubyte c)
{
    while ( !(inp(serdev->field_1096 + 5) & 0x20) )
        ;
    cli();
    outp(serdev->field_1096, c);
    sti();
}

void write_char(struct TbSerialDev *serdev, ubyte c)
{
    write_char_no_buff(serdev, c);
}

#endif

void write_string(struct TbSerialDev *serdev, const char *str)
{
#if defined(DOS)||defined(GO32)
    uint i;
    ubyte c;

    for (i = 0; i < strlen(str); i++)
    {
        c = str[i];
        write_char(serdev, c);
    }
#else
    // On Windows, WriteFile() should be used
    // On Linux, write the device file with standard file ops
    assert(!"not implemented");
#endif
}

void write_buffer(struct TbSerialDev *serdev, const ubyte *buf, uint buflen)
{
#if defined(DOS)||defined(GO32)
    uint i;
    ubyte c;

    for (i = 0; i < buflen; i++)
    {
        c = buf[i];
        write_char(serdev, c);
    }
#else
    // On Windows, WriteFile() should be used
    // On Linux, write the device file with standard file ops
    assert(!"not implemented");
#endif
}

void read_write_clear_flag(struct TbSerialDev *serdev, ushort port, ubyte c)
{
#if defined(DOS)||defined(GO32)
    ubyte val;
    val = inp(port) & ~c;
    outp(port, val);
#else
    assert(!"not implemented");
#endif
}

void read_write_set_flag(struct TbSerialDev *serdev, ushort port, ubyte c)
{
#if defined(DOS)||defined(GO32)
    ubyte val;
    val = inp(port) | c;
    outp(port, val);
#else
    assert(!"not implemented");
#endif
}

void wait(ulong msec)
{
#if defined(DOS)||defined(GO32)
    delay(msec);
#else
    usleep(msec * 1000);
#endif
}

void send_string(struct TbSerialDev *serdev, const char *str)
{
    char locstr[80];

    strcpy(locstr, str);
    strcat(locstr, "\r");
    write_string(serdev, locstr);
    strcpy(ModemRequestString, locstr);
}

uint net_unkn_callback1(ubyte *data, uint datalen)
{
    write_buffer(data_1e85e3, data, datalen);
    return datalen;
}

void inbuf_pos_inc(struct TbSerialDev *serdev)
{
    int pos;
    pos = ++serdev->inbuf_pos;
    if (pos > 2047)
        serdev->inbuf_pos = 0;
}

int read_char(struct TbSerialDev *serdev)
{
    ubyte chr;
    if (serdev->inbuf_pos == serdev->field_109E)
        return -1;
    chr = serdev->inbuf[serdev->inbuf_pos];
    inbuf_pos_inc(serdev);
    return chr;
}

int get_modem_response(struct TbSerialDev *serdev)
{
    const struct ModemResponse *resp;
    char locstr[80];
    TbClockMSec start_time;
    int ret;
    int chr;
    ushort lspos, mrpos;
    TbBool done;

    ret = -1;
    done = false;
    mrpos = 0;
    start_time = LbTimerClock();
    memset(locstr, 0, sizeof(locstr));
    lspos = 0;
    while (!done)
    {
        if (LbTimerClock() - start_time > 4000)
            return -1;
        chr = read_char(serdev);
        if (mrpos >= strlen(ModemRequestString)) {
            mrpos = 0;
        } else if (chr == ModemRequestString[mrpos]) {
            chr = -1;
            mrpos++;
        }
        if (chr == -1)
        {
            if (NetworkServicePtr.F.UsedSessionInit) {
                // TODO not sure if UsedSessionInit gets the chr as parameter
                if (NetworkServicePtr.F.UsedSessionInit() == -7)
                    return -7;
            }
            continue;
        }
        if ((chr == 10 && lspos == 0) || (lspos >= sizeof(locstr)))
        {
            for (resp = modem_response; resp->code != 0; resp++)
            {
                if (strncasecmp(locstr, resp->msg, strlen(resp->msg)) == 0)
                {
                    ret = resp->code;
                    strcpy(ModemResponseString, locstr);
                    done = true;
                    break;
                }
            }
            if (done)
                break; // break the main loop
            lspos = 0;
            memset(locstr, 0, sizeof(locstr));
        }

        if (chr >= 32)
        {
            locstr[lspos] = chr;
            lspos++;
        }
        // Periodically trigger user callback, if set
        if (NetworkServicePtr.F.UsedSessionInit != NULL)
        {
            if (NetworkServicePtr.F.UsedSessionInit() == -7)
                return -7;
        }
    }
    return ret;
}

struct TbSerialDev *LbCommInit(int idx)
{
    struct TbSerialDev *ret;
    LOGDBG("Starting");
    asm volatile ("call ASM_LbCommInit\n"
        : "=r" (ret) : "a" (idx) );
    return ret;
}

TbResult LbCommSetBaud(int rate, ushort dev_id)
{
    struct TbSerialDev *serdev;

    LOGDBG("Starting");
    if (dev_id > 3)
        return Lb_FAIL;

    serdev = com_dev[dev_id].serdev;
    if (serdev == NULL)
        return Lb_FAIL;

    serdev->baudrate = SetBps(serdev, rate);
    return Lb_SUCCESS;
}

int LbCommExchange(int a1, void *a2, int a3)
{
    int ret;
    asm volatile ("call ASM_LbCommExchange\n"
        : "=r" (ret) : "a" (a1), "d" (a2), "b" (a3) );
    return ret;
}

int LbCommStopExchange(ubyte a1)
{
    net_unkn_func_338(byte_1E81E0);
    return Lb_SUCCESS;
}

int LbCommDeInit(struct TbSerialDev *serhead)
{
#if defined(DOS)||defined(GO32)
    struct TbSerialDev *serdev;
    struct ComHandlerInfo *cdev;
    union REGS regs;
    ubyte r, n;

    LOGDBG("Starting");
    cdev = &com_dev[serhead->comdev_id];

    outp(serhead->field_1096 + 1, 0);
    outp(serhead->field_1096 + 4, 0);
    n = 1 << serhead1->field_109A;
    r = inp(0x21);
    outp(0x21, r | n);

    memset(&regs, 0, sizeof(union REGS));
    regs.x.eax = 0x201;
    regs.x.ebx = serhead1->field_1098;
    regs.x.ecx = cdev->field_18;
    regs.x.edx = cdev->field_1A;
    int386(0x31, &regs, &regs);

    memset(&regs, 0, sizeof(union REGS));
    regs.x.eax = 0x205;
    regs.x.ebx = serhead->field_1098;
    regs.x.ecx = cdev->field_16;
    regs.x.edx = cdev->field_12;
    int386(0x31, &regs, &regs);

    memset(&regs, 0, sizeof(union REGS));
    regs.x.eax = 0xF3;
    regs.x.edx = serhead->comdev_id;
    int386(0x14, &regs, &regs);

    return Lb_SUCCESS;
#else
    LOGDBG("Starting");
    return Lb_FAIL;
#endif
}

TbResult LbModemInit(ushort dev_id)
{
    struct TbSerialDev *serdev;
    TbResult ret;

    LOGDBG("Starting");
    if (dev_id > 3)
        return Lb_FAIL;

    serdev = com_dev[dev_id].serdev;

    if (serdev == NULL)
        return Lb_FAIL;

    send_string(serdev, modem_cmds[0].cmd);
    NetworkServicePtr.F.UsedSessionInit = NetworkServicePtr.F.SessionInit;
    ret = get_modem_response(serdev);
    NetworkServicePtr.F.UsedSessionInit = NULL;

    return ret;
}

TbResult LbModemDial(ushort dev_id, const char *distr)
{
    struct TbSerialDev *serdev;
    char locstr[80];
    TbResult ret;

    LOGDBG("Starting");
    if (dev_id > 3)
        return Lb_FAIL;

    serdev = com_dev[dev_id].serdev;

    if (serdev == NULL)
        return Lb_FAIL;

    strcpy(locstr, modem_cmds[1].cmd);
    switch(serdev->field_10AB)
    {
    case 1:
        strcat(locstr, "T");
        break;
    case 2:
        strcat(locstr, "P");
        break;
    }
    strcat(locstr, distr);

    send_string(serdev, locstr);
    NetworkServicePtr.F.UsedSessionInit = NetworkServicePtr.F.SessionDial;
    ret = get_modem_response(serdev);
    NetworkServicePtr.F.UsedSessionInit = NULL;

    return ret;
}

TbResult LbModemAnswer(ushort dev_id)
{
    struct TbSerialDev *serdev;
    TbResult ret;

    LOGDBG("Starting");
    if (dev_id > 3)
        return Lb_FAIL;

    serdev = com_dev[dev_id].serdev;
    if (serdev == NULL)
        return Lb_FAIL;

    NetworkServicePtr.F.UsedSessionInit = NetworkServicePtr.F.SessionAnswer;
    send_string(serdev, modem_cmds[3].cmd);
    ret = 3;
    while (ret == 3 || ret == 1)
        ret = get_modem_response(serdev);
    NetworkServicePtr.F.UsedSessionInit = NULL;
    return ret;
}

TbResult LbModemHangUp(ushort dev_id)
{
    struct TbSerialDev *serdev;
    TbResult ret;

    LOGDBG("Starting");
    if (dev_id > 3)
        return Lb_FAIL;

    serdev = com_dev[dev_id].serdev;
    if (serdev == NULL)
        return Lb_FAIL;

    read_write_clear_flag(serdev, serdev->field_1096 + 4, 0x01);
    wait(1250);

    read_write_set_flag(serdev, serdev->field_1096 + 4, 0x01);
    wait(1300);

    send_string(serdev, "+++");
    wait(1300);

    NetworkServicePtr.F.UsedSessionInit = NetworkServicePtr.F.SessionHangUp;
    ret = get_modem_response(serdev);
    NetworkServicePtr.F.UsedSessionInit = NULL;
    send_string(serdev, modem_cmds[2].cmd);

    return ret;
}

TbResult LbModemRingType(ushort dev_id, ubyte rtyp)
{
    struct TbSerialDev *serdev;

    LOGDBG("Starting");
    if (dev_id > 3)
        return Lb_FAIL;

    serdev = com_dev[dev_id].serdev;
    if (serdev == NULL)
        return Lb_FAIL;

    serdev->field_10AB = rtyp;

    return Lb_SUCCESS;
}

TbResult LbNetworkServiceStart(struct NetworkServiceInfo *p_nsvc)
{
    TbResult ret;
    ulong k;

    ret = Lb_FAIL;
    memcpy(&NetworkServicePtr.I, p_nsvc, sizeof(struct NetworkServiceInfo));
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        if (IPXHandler == NULL) {
            IPXHandler = LbMemoryAlloc(sizeof(struct TbIPXHandler));
        }
        if (IPXHandler == NULL) {
            LOGERR("Allocating IPX handler failed");
            ret = Lb_FAIL;
            break;
        }
        memset(&IPXPlayer.Header, 0, sizeof(struct TbIPXPlayerHeader));
        memset(&IPXPlayer.Data, 0, sizeof(struct TbIPXPlayerData));
        IPXPlayer.Header.field_2 = p_nsvc->GameId;
        k = p_nsvc->Param + 0x4545;
        NetworkServicePtr.I.Id = &IPXPlayer.Header;
        if (k > 0x4FFF)
            k = 0x4FFF;
        ipx_service_init((k << 8) | (k >> 8));
        ret = Lb_SUCCESS;
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        NetworkServicePtr.I.Id = LbCommInit(NetworkServicePtr.I.Type - NetSvc_COM1);
        if (NetworkServicePtr.I.Id == NULL) {
            LOGERR("Serial service init failed");
            ret = Lb_FAIL;
            break;
        }
        ret = Lb_SUCCESS;
        break;
    case NetSvc_RADICA:
        ret = radica_service_init(p_nsvc);
        if (ret != Lb_SUCCESS) {
            LOGERR("RADICA service init failed");
            ret = Lb_FAIL;
            NetworkServicePtr.I.Type = NetSvc_NONE; // Why only this one is reverting the type on fail?
        }
        break;
    }
    return ret;
}

TbResult LbNetworkUpdate(void)
{
    TbResult ret;

    ret = Lb_FAIL;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        ipx_update();
        ret = Lb_SUCCESS;
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        ret = Lb_SUCCESS;
        break;
    case NetSvc_RADICA:
        radica_update();
        ret = Lb_SUCCESS;
        break;
    }
    return ret;
}

int LbNetworkSessionList(struct TbNetworkSessionList *nslist, int listlen)
{
    int ret;

    ret = 0;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        ret = ipx_session_list_conv(nslist, listlen);
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        ret = 0;
        break;
    case NetSvc_RADICA:
        ret = radica_session_list(nslist, listlen);
        break;
    }
    return ret;
}

/** Prepares Network_Service_List and returns its size.
 */
int LbNetworkServiceList(void)
{
    struct NetworkServiceInfo *nsinfo;
    int i;

    nsinfo = Network_Service_List;
    i = 0;
    while (nsinfo->Type != 0)
    {
        nsinfo++;
        i++;
    }
    return i;
}

int LbNetworkSessionNumberPlayers(void)
{
    struct TbSerialDev *serhead;
    struct TbIPXPlayerData *ipxdata;
    int ret;

    ret = Lb_FAIL;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        ipxdata = &IPXPlayer.Data;
        ret = ipxdata->num_players;
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        serhead = NetworkServicePtr.I.Id;
        ret = serhead->num_players;
        break;
    case NetSvc_RADICA:
        ret = Lb_FAIL;
        break;
    }
    return ret;
}

TbResult LbNetworkSend(int plyr, ubyte *data, int dtlen)
{
    TbResult ret;

    ret = Lb_FAIL;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        ipx_send_packet_to_player_wait(plyr, data, dtlen);
        ret = Lb_SUCCESS;
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        ret = Lb_FAIL;
        break;
    case NetSvc_RADICA:
        ret = Lb_FAIL;
        break;
    }
    return ret;
}

TbResult LbNetworkReceive(int plyr, ubyte *data, int dtlen)
{
    TbResult ret;

    ret = Lb_FAIL;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        ipx_receive_packet_from_player_wait(plyr, data, dtlen);
        ret = Lb_SUCCESS;
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        ret = Lb_FAIL;
        break;
    case NetSvc_RADICA:
        ret = Lb_FAIL;
        break;
    }
    return ret;
}

TbResult LbNetworkMessageSend(int plyr, ubyte *data, int dtlen)
{
    TbResult ret;

    ret = Lb_FAIL;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        ipx_send_packet_to_player_wait(plyr, data, dtlen);
        ret = Lb_SUCCESS;
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        ret = Lb_FAIL;
        break;
    case NetSvc_RADICA:
        ret = Lb_FAIL;
        break;
    }
    return ret;
}

TbResult LbNetworkMessageReceive(int plyr, ubyte *data, int dtlen)
{
    TbResult ret;

    ret = Lb_FAIL;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        ipx_receive_packet_from_player_wait(plyr, data, dtlen);
        ret = Lb_SUCCESS;
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        ret = Lb_FAIL;
        break;
    case NetSvc_RADICA:
        ret = Lb_FAIL;
        break;
    }
    return ret;
}

TbResult LbNetworkSendNoWait(int plyr, ubyte *data, int dtlen)
{
    TbResult ret;

    ret = Lb_FAIL;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        ipx_send_packet_to_player_nowait(plyr, data, dtlen);
        ret = Lb_SUCCESS;
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        ret = Lb_FAIL;
        break;
    case NetSvc_RADICA:
        ret = Lb_FAIL;
        break;
    }
    return ret;
}

TbResult LbNetworkReceiveNoWait(int plyr, ubyte *data, int dtlen)
{
    TbResult ret;

    ret = Lb_FAIL;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        ipx_receive_packet_from_player_nowait(plyr, data, dtlen);
        ret = Lb_SUCCESS;
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        ret = Lb_FAIL;
        break;
    case NetSvc_RADICA:
        ret = Lb_FAIL;
        break;
    }
    return ret;
}

TbResult LbNetworkInit(void)
{
    struct TbSerialDev *serhead;
    TbResult ret;

    ret = Lb_FAIL;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        ret = Lb_FAIL;
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        serhead = NetworkServicePtr.I.Id;
        ret = LbModemInit(serhead->comdev_id);
        break;
    case NetSvc_RADICA:
        ret = Lb_FAIL;
        break;
    }
    return ret;
}

TbResult LbNetworkDial(const char *distr)
{
    struct TbSerialDev *serhead;
    TbResult ret;

    ret = Lb_FAIL;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        ret = Lb_FAIL;
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        serhead = NetworkServicePtr.I.Id;
        ret = LbModemDial(serhead->comdev_id, distr);
        break;
    case NetSvc_RADICA:
        ret = Lb_FAIL;
        break;
    }
    return ret;
}

TbResult LbNetworkAnswer(void)
{
    struct TbSerialDev *serhead;
    TbResult ret;

    ret = Lb_FAIL;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        ret = Lb_FAIL;
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        serhead = NetworkServicePtr.I.Id;
        ret = LbModemAnswer(serhead->comdev_id);
        break;
    case NetSvc_RADICA:
        ret = Lb_FAIL;
        break;
    }
    return ret;
}

TbResult LbNetworkSessionStop(void)
{
    struct TbSerialDev *serhead;
    TbResult ret;

    ret = Lb_FAIL;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        ipx_stop_network();
        ret = Lb_SUCCESS;
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        serhead = NetworkServicePtr.I.Id;
        LbCommStopExchange(serhead->comdev_id);
        lbICommSessionActive = 0;
        ret = Lb_SUCCESS;
        break;
    case NetSvc_RADICA:
        ret = Lb_FAIL;
        break;
    }
    return ret;
}

TbResult LbNetworkHostPlayerNumber(void)
{
    struct TbSerialDev *serhead;
    TbResult ret;

    ret = Lb_FAIL;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        ret = ipx_get_host_player_number();
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        serhead = NetworkServicePtr.I.Id;
        if (!serhead->field_10A9)
            ret = 0;
        else
            ret = 1;
        break;
    case NetSvc_RADICA:
        ret = Lb_FAIL;
        break;
    }
    return ret;
}

TbResult LbNetworkSetupIPXAddress(ulong addr)
{
    if (addr == 0)
        return Lb_FAIL;
#if 0
    IPXHandler->unkn_addr_field = addr;
#endif
    return Lb_SUCCESS;
}

int LbNetworkPlayerNumber(void)
{
    struct TbSerialDev *serhead;
    int ret;

    ret = Lb_FAIL;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        ret = ipx_get_player_number();
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        serhead = NetworkServicePtr.I.Id;
        if (!serhead->field_10A9)
            ret = 0;
        else
            ret = 1;
        break;
    case NetSvc_RADICA:
        break;
    }
    return ret;
}

int LbNetworkPlayerName(char *name, int plyr)
{
    struct TbSerialDev *serhead;
    TbResult ret;

    ret = Lb_FAIL;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        ret = ipx_get_player_name(name, plyr);
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        serhead = NetworkServicePtr.I.Id;
        if (plyr == 0) {
            strcpy(name, &serhead->field_10AD[0]);
            ret = Lb_SUCCESS;
        } else if (plyr == 1) {
            strcpy(name, &serhead->field_10AD[16]);
            ret = Lb_SUCCESS;
        }
        break;
    case NetSvc_RADICA:
        break;
    }
    return ret;
}

TbResult LbNetworkExchange(void *a1, int a2)
{
    struct TbSerialDev *serhead;
    TbResult ret;

    ret = Lb_FAIL;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        ret = ipx_exchange_packets(a1, a2);
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        serhead = NetworkServicePtr.I.Id;
        LbCommExchange(serhead->comdev_id, a1, a2);
        ret = Lb_SUCCESS;
        break;
    case NetSvc_RADICA:
        radica_exchange_packets(a1, a2);
        ret = Lb_SUCCESS;
        break;
    }
    return ret;
}

TbResult LbNetworkReset(void)
{
    TbResult ret;

    ret = Lb_FAIL;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        if (ipx_is_initialized()) {
            ipx_shutdown(IPXHandler->field_8);
            ret = Lb_SUCCESS;
        }
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        ret = LbCommDeInit(NetworkServicePtr.I.Id);
        break;
    case NetSvc_RADICA:
        ret = radica_shutdown();
        break;
    }
    NetworkServicePtr.I.Type = NetSvc_NONE;
    return ret;
}

TbBool LbNetworkSessionActive(void)
{
    TbBool ret;

    ret = false;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        if (ipx_is_initialized())
            ret = IPXHandler->SessionActive;
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        ret = lbICommSessionActive;
        break;
    case NetSvc_RADICA:
        ret = false;
        break;
    }
    return ret;
}

TbResult LbNetworkHangUp(void)
{
    struct TbSerialDev *serhead;
    TbResult ret;

    ret = Lb_FAIL;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        ret = Lb_FAIL;
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        serhead = NetworkServicePtr.I.Id;
        ret = LbModemHangUp(serhead->comdev_id);
        break;
    case NetSvc_RADICA:
        ret = Lb_FAIL;
        break;
    }
    return ret;
}

TbResult LbNetworkReadConfig(const char *fname)
{
    return LbModemReadConfig(fname);
}

TbResult LbNetworkSetBaud(int rate)
{
    struct TbSerialDev *serhead;
    TbResult ret;

    ret = Lb_FAIL;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        ret = Lb_FAIL;
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        serhead = NetworkServicePtr.I.Id;
        LbCommSetBaud(rate, serhead->comdev_id);
        ret = Lb_SUCCESS;
        break;
    case NetSvc_RADICA:
        ret = Lb_FAIL;
        break;
    }
    return ret;
}

TbResult LbNetworkShutDownListeners(void)
{
    TbResult ret;

    ret = Lb_FAIL;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        ipx_shutdown_listeners();
        ret = Lb_SUCCESS;
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        ret = Lb_SUCCESS;
        break;
    case NetSvc_RADICA:
        ret = Lb_SUCCESS;
        break;
    }
    return ret;
}

TbResult LbNetworkOpenUpListeners(void)
{
    TbResult ret;

    ret = Lb_FAIL;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        ipx_openup_listeners();
        ret = Lb_SUCCESS;
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        ret = Lb_SUCCESS;
        break;
    case NetSvc_RADICA:
        ret = Lb_SUCCESS;
        break;
    }
    return ret;
}

TbResult LbCommSessionCreate(struct TbSerialDev *serhead, const char *sess_name, const char *a2)
{
    NSVC_SESSIONCB exchange_cb_bkp;
    char locstr[16];
    TbResult ret;

    LOGDBG("Starting");
    serhead->field_10A9 = 1;
    strcpy(serhead->field_10AD, a2);

    exchange_cb_bkp = NetworkServicePtr.F.SessionExchange;
    NetworkServicePtr.F.SessionExchange = NetworkServicePtr.F.SessionCreate;
    ret = LbCommExchange(serhead->comdev_id, serhead->field_10AD, 16);
    if (ret == Lb_SUCCESS)
    {
        strcpy(locstr, sess_name);
        ret = LbCommExchange(serhead->comdev_id, locstr, 8);
        lbICommSessionActive = 1;
    }
    NetworkServicePtr.F.SessionExchange = exchange_cb_bkp;

    return ret;
}

TbResult LbCommSessionJoin(struct TbSerialDev *serhead, char *sess_name, const char *a2)
{
    NSVC_SESSIONCB exchange_cb_bkp;
    char locstr[16];
    TbResult ret;

    LOGDBG("Starting");
    serhead->field_10A9 = 0;
    strcpy(&serhead->field_10AD[16], a2);

    exchange_cb_bkp = NetworkServicePtr.F.SessionExchange;
    NetworkServicePtr.F.SessionExchange = NetworkServicePtr.F.SessionJoin;
    ret = LbCommExchange(serhead->comdev_id, serhead->field_10AD, 16);
    if (ret == Lb_SUCCESS)
    {
        ret = LbCommExchange(serhead->comdev_id, locstr, 8);
        if (ret == Lb_SUCCESS) {
            strcpy(sess_name, locstr);
        }
        lbICommSessionActive = 1;
    }
    NetworkServicePtr.F.SessionExchange = exchange_cb_bkp;

    return ret;
}

TbResult LbNetworkSessionCreate(struct TbNetworkSession *session, char *a2)
{
    struct TbSerialDev *serhead;
    TbResult ret;

    ret = Lb_FAIL;
    NetTimeoutTicks = 3000;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        IPXPlayer.Data.field_10E = session->MaxPlayers;
        ret = ipx_create_session(session->Name, a2);
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        serhead = NetworkServicePtr.I.Id;
        ret = LbCommSessionCreate(serhead, session->Name, a2);
        break;
    case NetSvc_RADICA:
        ret = radica_create_session(session, a2);
        break;
    }
    LOGSYNC("Service %d create result=%d", (int)NetworkServicePtr.I.Type, (int)ret);
    return ret;
}

void read_a_line(FILE *fp, char *buf)
{
    char c;
    char *s;

    for (s = buf; ; s++)
    {
        c = fgetc(fp);
        if ((c == '\r') || (c == '\n'))
            break;
        *s = c;
    }
    *s = '\0';
}

TbResult LbNetworkSessionJoin(struct TbNetworkSession *session, char *a2)
{
    struct TbSerialDev *serhead;
    struct IPXSessionList ipxsess;
    TbResult ret;

    NetTimeoutTicks = 5000;
    ret = Lb_FAIL;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        memset(&ipxsess, 0, sizeof(struct IPXSessionList));
        memcpy(ipxsess.Session.Name, session->Name, sizeof(ipxsess.Session.Name));
        ipxsess.Session.Name[7] = '\0';
        memcpy(&ipxsess.Session.Reserved[22], &session->Reserved[0], 4);
        memcpy(&ipxsess.Session.Reserved[16], &session->Reserved[4], 6);
        ret = ipx_join_session(&ipxsess, a2);
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        serhead = NetworkServicePtr.I.Id;
        ret = LbCommSessionJoin(serhead, session->Name, a2);
        break;
    case NetSvc_RADICA:
        ret = radica_join_session(session, a2);
        break;
    }
    return ret;
}

TbResult LbModemReadConfig(const char *fname)
{
    char locstr[80];
    FILE *fp;
    int i;

    LOGDBG("Starting");
    if (fname == NULL)
        return Lb_FAIL;
    fp = fopen(fname, "rt");
    if (!fp)
        return Lb_FAIL;
    for (i = 0; i < 4; i++)
    {
        read_a_line(fp, locstr);
        read_a_line(fp, locstr);
        com_dev[i].field_4 = strtol(locstr, NULL, 16);
        read_a_line(fp, locstr);
        com_dev[i].field_8 = atol(locstr);
        com_dev[i].field_6 = com_dev[i].field_8 + 8;
    }
    read_a_line(fp, locstr);
    read_a_line(fp, modem_cmds[0].cmd);
    read_a_line(fp, modem_cmds[1].cmd);
    read_a_line(fp, modem_cmds[2].cmd);
    read_a_line(fp, modem_cmds[3].cmd);
    fclose(fp);
    return Lb_SUCCESS;
}

int my_net_session_callback()
{
    swap_wscreen();
    if (is_key_pressed(KC_ESCAPE, KMod_DONTCARE))
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
