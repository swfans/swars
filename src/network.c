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
#include <unistd.h>

#include "bfkeybd.h"
#include "bfdos.h"
#include "bfmemory.h"
#include "display.h"
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

struct IPXSessionList { // sizeof=218
    struct IPXSession Session; // offset=0
    struct IPXPlayer Player[8]; // offset=45
    ubyte NumberOfPlayers; // offset=269
    ubyte field_270; // offset=270
};

#pragma pack()
/******************************************************************************/

extern ubyte lbICommSessionActive;
extern struct TbIPXHandler *IPXHandler;
extern struct TbIPXPlayerHeader IPXPlayerHeader;

extern char ModemResponseString[80];
extern char ModemRequestString[80];
extern ubyte byte_1E81E0[1027];

extern ulong ipx_send_packet_count[8][8];
extern ulong ipx_got_player_send_packet_count[8];

extern struct ComHandlerInfo com_dev[4];
extern struct IPXDatagramBackup datagram_backup[8];

struct ModemCommand modem_cmds[] = {
    {"ATZ"},
    {"ATD"},
    {"ATH"},
    {"ATS0=1"},
};

struct NetworkServiceInfo Network_Service_List[] = {
    {0, 0, NetSvc_IPX, 0x02},
    {0, 0, NetSvc_COM1, 0x01},
    {0, 0, NetSvc_COM2, 0x01},
    {0, 0, NetSvc_COM3, 0x01},
    {0, 0, NetSvc_COM4, 0x01},
    {0, 0, NetSvc_Unkn6, 0x02},
    {0, 0, 0, 0},
};

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
    regs.x.ebx = IPXHandler->field_0;
    segread(&sregs);
    regs.x.edi = (unsigned int)&dpmi_regs;
    return int386x(0x31, &regs, &regs, &sregs);
}

ubyte CallRealModeInterrupt(ubyte a1, struct DPMI_REGS *dpmi_regs)
{
    union REGS regs;
    struct SREGS sregs;

    memset(&regs, 0, sizeof(union REGS));
    /* Use DMPI call 300h to issue the DOS interrupt */
    regs.x.eax = 0x300;
    regs.x.ebx = a1;
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

int ipx_update(void)
{
    TbResult ret;
    asm volatile ("call ASM_ipx_update\n"
        : "=r" (ret) : );
    return ret;
}

void ipx_service_init(ushort a1)
{
#if 0
    TbResult ret;
    asm volatile ("call ASM_ipx_service_init\n"
        : "=r" (ret) : "a" (a1) );
    return ret;
#endif
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
#if 0
    TbResult ret;
    asm volatile ("call ASM_ipx_create_session\n"
        : "=r" (ret) : "a" (a1), "d" (a2) );
    return ret;
#endif
    struct TbIPXHandler *ipxhndl;
    struct TbIPXPlayer *plyr;
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
            if (ipxhndl->field_46[i])
            {
                ipxhndl->field_46[i] = 0;
                plyr = &ipxhndl->PlayerData[i];
                if (IPXPlayerHeader.field_2 == plyr->Header.field_2)
                {
                    if (strcasecmp(plyr->Header.field_4, a1) == 0) {
                        LOGERR("String same as remote");
                        return Lb_FAIL;
                    }
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
    setup_bullfrog_header(&IPXPlayerHeader, 1);
    IPXHandler->field_44 = 1;

    if (strlen(a1) > 7)
            a1[7] = 0;
    strcpy(IPXPlayerHeader.field_4, a1);

    memcpy(IPXPlayerHeader.field_2D, &IPXHandler->field_2A, 0x1Cu);
    strcpy(IPXPlayerHeader.field_C, IPXHandler->field_34);

    memset(ipx_send_packet_count, 0, 0x100u);
    memset(ipx_got_player_send_packet_count, 0, 0x20u);
    memset(datagram_backup, 0, sizeof(struct IPXDatagramBackup) * 8);

    ipxhndl = IPXHandler;
    ipxhndl->SessionActive = 1;
    ipxhndl->field_C = 0;
    ipxhndl->field_D = 0;
    IPXPlayerHeader.num_players = 1;
    IPXPlayerHeader.field_26 = 0;

    return Lb_SUCCESS;
}

TbResult ipx_session_list(struct IPXSessionList *ipxsess, int a2)
{
    TbResult ret;
    asm volatile ("call ASM_ipx_session_list\n"
        : "=r" (ret) : "a" (ipxsess), "d" (a2) );
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
    return IPXPlayerHeader.field_2B;
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
    LOGDBG("Starting");
    asm volatile ("call ASM_ipx_shutdown\n"
        : : "a" (a1));
}

int ipx_shutdown_listeners(void)
{
    int ret;
    LOGDBG("Starting");
    asm volatile ("call ASM_ipx_shutdown_listeners\n"
        : "=r" (ret) : );
    return ret;
}

int ipx_stop_network(void)
{
    int ret;
    LOGDBG("Starting");
    asm volatile ("call ASM_ipx_stop_network\n"
        : "=r" (ret) : );
    return ret;
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

int netsvc6_create_session(struct TbNetworkSession *session, const char *a2)
{
    int ret;
    LOGDBG("Starting");
    asm volatile ("call ASM_netsvc6_create_session\n"
        : "=r" (ret) : "a" (session), "d" (a2) );
    return ret;
}

int netsvc6_update(void)
{
    assert(!"Not implemented");
    return Lb_SUCCESS;
}

int netsvc6_exchange_packets(void *a1, int a2)
{
    int ret;
    asm volatile ("call ASM_netsvc6_exchange_packets\n"
        : "=r" (ret) : "a" (a1), "d" (a2) );
    return ret;
}

int netsvc6_shutdown(void)
{
    LOGSYNC("Quitting RADICA");
    return net_unkn_func_352();
}

struct TbSerialDev *LbCommInit(int idx)
{
    struct TbSerialDev *ret;
    LOGDBG("Starting");
    asm volatile ("call ASM_LbCommInit\n"
        : "=r" (ret) : "a" (idx) );
    return ret;
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
    return 1;
}

int LbCommDeInit(struct TbSerialDev *serhead)
{
    int ret;
    LOGDBG("Starting");
    asm volatile ("call ASM_LbCommDeInit\n"
        : "=r" (ret) : "a" (serhead) );
    return ret;
}

TbResult netsvc6_service_init(struct NetworkServiceInfo *nsvc)
{
    TbResult ret;
    LOGDBG("Starting");
    asm volatile ("call ASM_netsvc6_service_init\n"
        : "=r" (ret) : "a" (nsvc) );
    return ret;
}

int netsvc6_session_list(struct TbNetworkSessionList *nslist, int listlen)
{
    int ret;
    LOGDBG("Starting");
    asm volatile ("call ASM_netsvc6_session_list\n"
        : "=r" (ret) : "a" (nslist), "d" (listlen) );
    return ret;
}

TbResult LbNetworkServiceStart(struct NetworkServiceInfo *nsvc)
{
#if 0
    TbResult ret;
    asm volatile ("call ASM_LbNetworkServiceStart\n"
        : "=r" (ret) : "a" (nsvc) );
    return ret;
#endif
    TbResult ret;
    ulong k;

    ret = Lb_FAIL;
    memcpy(&NetworkServicePtr.I, nsvc, sizeof(struct NetworkServiceInfo));
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
#if 0
        if (IPXHandler == NULL) {
            IPXHandler = LbMemoryAlloc(sizeof(struct TbIPXHandler));
        }
#endif
        if (IPXHandler == NULL) {
            LOGERR("Allocating IPX handler failed");
            ret = Lb_FAIL;
            break;
        }
        memset(&IPXPlayerHeader, 0, sizeof(struct TbIPXPlayerHeader));
        IPXPlayerHeader.field_2 = nsvc->GameId;
        k = (nsvc->Flags >> 16) + 0x4545;
        NetworkServicePtr.I.Id = &IPXPlayerHeader;
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
    case NetSvc_Unkn6:
        ret = netsvc6_service_init(nsvc);
        if (ret != Lb_SUCCESS) {
            LOGERR("Unkn6 service init failed");
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
    case NetSvc_Unkn6:
        netsvc6_update();
        ret = Lb_SUCCESS;
        break;
    }
    return ret;
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

int LbNetworkSessionList(struct TbNetworkSessionList *nslist, int listlen)
{
#if 0
    int ret;
    asm volatile ("call ASM_LbNetworkSessionList\n"
        : "=r" (ret) : "a" (nslist), "d" (listlen) );
    return ret;
#endif
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
    case NetSvc_Unkn6:
        ret = netsvc6_session_list(nslist, listlen);
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
    struct TbIPXPlayerHeader *ipxhead;
    int ret;

    ret = Lb_FAIL;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        ipxhead = &IPXPlayerHeader;
        ret = ipxhead->num_players;
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        serhead = NetworkServicePtr.I.Id;
        ret = serhead->num_players;
        break;
    case NetSvc_Unkn6:
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
    case NetSvc_Unkn6:
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
    case NetSvc_Unkn6:
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
    case NetSvc_Unkn6:
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
    case NetSvc_Unkn6:
        netsvc6_exchange_packets(a1, a2);
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
    case NetSvc_Unkn6:
        ret = netsvc6_shutdown();
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
    case NetSvc_Unkn6:
        ret = false;
        break;
    }
    return ret;
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
    unsigned int i;
    char c;

    for (i = 0; i < strlen(locstr); i++)
    {
        c = locstr[i];
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

int get_modem_response(struct TbSerialDev *serdev)
{
    int ret;
    asm volatile ("call ASM_get_modem_response\n"
        : "=r" (ret) : "a" (serdev) );
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
    case NetSvc_Unkn6:
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
    case NetSvc_Unkn6:
        ret = Lb_SUCCESS;
        break;
    }
    return ret;
}

TbResult LbCommSessionCreate(struct TbSerialDev *serhead, const char *sess_name, const char *a2)
{
    NSVC_SESSIONCB exchange_cb_bkp;
    char locstr[16];
    char *s;
    TbResult ret;

    LOGDBG("Starting");
    serhead->field_10A9 = 1;
    s = serhead->field_10AD;
    strcpy(s, a2);

    exchange_cb_bkp = NetworkServicePtr.F.SessionExchange;
    NetworkServicePtr.F.SessionExchange = NetworkServicePtr.F.SessionCreate;
    ret = LbCommExchange(serhead->comdev_id, s, 16);
    if (ret == Lb_SUCCESS)
    {
        s = locstr;
        strcpy(s, sess_name);
        ret = LbCommExchange(serhead->comdev_id, s, 8);
        lbICommSessionActive = 1;
    }
    NetworkServicePtr.F.SessionExchange = exchange_cb_bkp;

    return ret;
}

TbResult LbNetworkSessionCreate(struct TbNetworkSession *session, char *a2)
{
#if 0
    TbResult ret;
    asm volatile ("call ASM_LbNetworkSessionCreate\n"
        : "=r" (ret) : "a" (session), "d" (a2) );
    return ret;
#endif
    struct TbSerialDev *serhead;
    TbResult ret;

    ret = Lb_FAIL;
    NetTimeoutTicks = 3000;
    switch (NetworkServicePtr.I.Type)
    {
    case NetSvc_IPX:
        IPXPlayerHeader.field_10E = session->MaxPlayers;
        ret = ipx_create_session(session->Name, a2);
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        serhead = NetworkServicePtr.I.Id;
        ret = LbCommSessionCreate(serhead, session->Name, a2);
        break;
    case NetSvc_Unkn6:
        ret = netsvc6_create_session(session, a2);
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

TbResult LbModemReadConfig(const char *fname)
{
    char locstr[80];
    FILE *fp;
    int i;

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
