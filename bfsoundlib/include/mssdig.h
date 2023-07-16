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

/** Install and initialize digital audio driver.
 */
DIG_DRIVER *SS_construct_DIG_driver(AIL_DRIVER *drvr, const SNDCARD_IO_PARMS *iop);

/** Uninstall digital audio driver, freeing all allocated resources.
 *
 * This function is called via the AIL_DRIVER.destructor vector only.
 */
void SS_destroy_DIG_driver(DIG_DRIVER *digdrv);

/** Start driver-based DMA buffer playback.
 */
void SS_start_DIG_driver_playback(DIG_DRIVER *digdrv);

/** Stop driver-based DMA buffer playback.
 */
void SS_stop_DIG_driver_playback(DIG_DRIVER *digdrv);

DIG_DRIVER *AIL2OAL_API_install_DIG_driver_file(const char *fname,
        const SNDCARD_IO_PARMS *iop);
int32_t AIL2OAL_API_install_DIG_INI(DIG_DRIVER **digdrv);
void AIL2OAL_API_uninstall_DIG_driver(DIG_DRIVER *digdrv);

/** Timer interrupt routine to poll DMA buffer flags and process samples.
 *
 * Has anything to do with interrupts and DMA channels only on bare metal systems
 * like DOS. On modern OSes, its periodical calls just transfer data buffers
 * to a sound interface like OpenAL.
 */
void SS_serve(void *clientval);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // AIL2OAL_MSSDIG_H_
