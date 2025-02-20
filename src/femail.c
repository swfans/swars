/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file femail.c
 *     Front-end desktop and menu system, emails handling.
 * @par Purpose:
 *     Implement functions for emails handling in front-end desktop.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     22 Apr 2023 - 22 Oct 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "femail.h"

#include "campaign.h"
#include "femain.h"
#include "swlog.h"
/******************************************************************************/

ushort activate_queued_mail(void)
{
    ushort ret;
    asm volatile ("call ASM_activate_queued_mail\n"
        : "=r" (ret) : );
    return ret;
}

void delete_mail(ushort mailnum, ubyte type)
{
    asm volatile ("call ASM_delete_mail\n"
        : : "a" (mailnum), "d" (type));
}

void queue_up_new_mail(ubyte emtype, short missi)
{
    int i;

    LOGSYNC("New email type %d after mission %d, source %d", (int)emtype,
      (int)missi, (int)mission_list[missi].SourceID);
    if ((emtype == 1) && (mission_list[missi].SourceID == 0))
        return;
    if (missi < 0) {
        missi = -missi;
        emtype = 0;
    }
    i = new_mail;
    newmail_store[i].Mission = missi;
    newmail_store[i].RecvDay = global_date.Day;
    newmail_store[i].RecvMonth = global_date.Month;
    newmail_store[i].RecvYear = global_date.Year;
    if (emtype != 1)
        newmail_store[i].Flag = 0x02;
    else
        newmail_store[i].Flag = 0x01;
    new_mail++;
}

/******************************************************************************/
