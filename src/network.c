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
#include "bfdos.h"
#include "display.h"
#include <assert.h>
#include <unistd.h>
/******************************************************************************/
extern ubyte lbICommSessionActive;
extern struct TbIPXHandler *IPXHandler;
extern struct TbIPXPlayerHeader IPXPlayerHeader;

extern char ModemResponseString[80];
extern char ModemRequestString[80];
extern ubyte byte_1E81E0[1027];

extern struct ComHandlerInfo com_dev[4];

struct ModemCommand modem_cmds[] = {
    {"ATZ"},
    {"ATD"},
    {"ATH"},
    {"ATS0=1"},
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

int ipx_get_host_player_number(void)
{
    if (!ipx_is_initialized())
        return -1;
    if (IPXHandler->field_A == 0)
        return -1;
    return IPXHandler->field_D;
}

int ipx_get_player_number(void)
{
    if (!ipx_is_initialized())
        return -1;
    if (IPXHandler->field_A == 0)
        return -1;
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
    asm volatile ("call ASM_ipx_shutdown\n"
        : : "a" (a1));
}

int ipx_shutdown_listeners(void)
{
    int ret;
    asm volatile ("call ASM_ipx_shutdown_listeners\n"
        : "=r" (ret) : );
    return ret;
}

int ipx_stop_network(void)
{
    int ret;
    asm volatile ("call ASM_ipx_stop_network\n"
        : "=r" (ret) : );
    return ret;
}

int net_unkn_func_352(void)
{
    int ret;
    asm volatile ("call ASM_net_unkn_func_352\n"
        : "=r" (ret) :  );
    return ret;
}

int net_unkn_func_338(void *a1)
{
    int ret;
    asm volatile ("call ASM_net_unkn_func_338\n"
        : "=r" (ret) : "a" (a1) );
    return ret;
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
    printf("QUITTING RADICA\n");
    return net_unkn_func_352();
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
    asm volatile ("call ASM_LbCommDeInit\n"
        : "=r" (ret) : "a" (serhead) );
    return ret;
}

int LbNetworkSessionNumberPlayers(void)
{
    struct TbSerialDev *serhead;
    struct TbIPXPlayerHeader *ipxhead;
    int ret;

    ret = Lb_FAIL;
    switch (NetworkServicePtr.Type)
    {
    case NetSvc_IPX:
        ipxhead = &IPXPlayerHeader;
        ret = ipxhead->num_players;
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        serhead = NetworkServicePtr.Id;
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
    switch (NetworkServicePtr.Type)
    {
    case NetSvc_IPX:
        ipx_stop_network();
        ret = Lb_SUCCESS;
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        serhead = NetworkServicePtr.Id;
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
    switch (NetworkServicePtr.Type)
    {
    case NetSvc_IPX:
        ret = ipx_get_host_player_number();
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        serhead = NetworkServicePtr.Id;
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
    switch (NetworkServicePtr.Type)
    {
    case NetSvc_IPX:
        ret = ipx_get_player_number();
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        serhead = NetworkServicePtr.Id;
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
    switch (NetworkServicePtr.Type)
    {
    case NetSvc_IPX:
        ret = ipx_exchange_packets(a1, a2);
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        serhead = NetworkServicePtr.Id;
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
    switch (NetworkServicePtr.Type)
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
        ret = LbCommDeInit(NetworkServicePtr.Id);
        break;
    case NetSvc_Unkn6:
        ret = netsvc6_shutdown();
        break;
    }
    NetworkServicePtr.Type = NetSvc_NONE;
    return ret;
}

TbBool LbNetworkSessionActive(void)
{
    TbBool ret;

    ret = false;
    switch (NetworkServicePtr.Type)
    {
    case NetSvc_IPX:
        if (ipx_is_initialized())
            ret = IPXHandler->field_A;
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
    switch (NetworkServicePtr.Type)
    {
    case NetSvc_IPX:
        ret = Lb_FAIL;
        break;
    case NetSvc_COM1:
    case NetSvc_COM2:
    case NetSvc_COM3:
    case NetSvc_COM4:
        serhead = NetworkServicePtr.Id;
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
    switch (NetworkServicePtr.Type)
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
