/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file mssdig.h
 *     Header file for mssdig.c.
 * @par Purpose:
 *     OpenAL based reimplementation of MSS API.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Jun 2022 - 05 Sep 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef AIL2OAL_MSSDIG_H_
#define AIL2OAL_MSSDIG_H_

#include <stdint.h>
#include "mssal.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

DIG_DRIVER *AIL2OAL_API_install_DIG_driver_file(const char *fname,
        const SNDCARD_IO_PARMS *iop);
int32_t AIL2OAL_API_install_DIG_INI(DIG_DRIVER **digdrv);
void AIL2OAL_API_uninstall_DIG_driver(DIG_DRIVER *digdrv);

/** Timer interrupt handler which processes samples.
 */
void SS_serve(uintptr_t clientval);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // AIL2OAL_MSSDIG_H_
