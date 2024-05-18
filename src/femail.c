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

/******************************************************************************/
