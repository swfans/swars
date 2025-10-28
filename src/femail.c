/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
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

#include "bfmemut.h"

#include "campaign.h"
#include "febrief.h"
#include "femain.h"
#include "misstat.h"
#include "swlog.h"
/******************************************************************************/

ushort activate_queued_mail(void)
{
    ushort ret;
    asm volatile ("call ASM_activate_queued_mail\n"
        : "=r" (ret) : );
    return ret;
}

void delete_mail(ushort mailnum, ubyte emtype)
{
#if 0
    asm volatile ("call ASM_delete_mail\n"
        : : "a" (mailnum), "d" (emtype));
#else
    ushort i;

    if (emtype == MlTp_Mission)
    {
        for (i = mailnum + 1; i < next_brief; i++)
        {
            LbMemoryCopy(&brief_store[i - 1], &brief_store[i], sizeof(struct EmailItem));
            LbMemoryCopy(&mission_status[i - 1], &mission_status[i], sizeof(struct MissionStatus));
        }
        next_brief--;
    }
    else
    {
        for (i = mailnum + 1; i < next_email; i++)
        {
            LbMemoryCopy(&email_store[i - 1], &email_store[i], sizeof(struct EmailItem));
        }
        next_email--;
    }
#endif
}

void queue_up_new_mail(ubyte emtype, short missi)
{
    int i;

    LOGSYNC("New email type %d after mission %d, source %d", (int)emtype,
      (int)missi, (int)mission_list[missi].SourceID);
    if ((emtype == MlTp_Mission) && (mission_list[missi].SourceID == 0))
        return;
    if (missi < 0) {
        missi = -missi;
        emtype = MlTp_Email;
    }
    i = new_mail;
    newmail_store[i].Mission = missi;
    newmail_store[i].RecvDay = global_date.Day;
    newmail_store[i].RecvMonth = global_date.Month;
    newmail_store[i].RecvYear = global_date.Year;
    if (emtype != MlTp_Mission)
        newmail_store[i].Flag = 0x02;
    else
        newmail_store[i].Flag = 0x01;
    new_mail++;
}

/******************************************************************************/
