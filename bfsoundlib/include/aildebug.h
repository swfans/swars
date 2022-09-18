/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file aildebug.h
 *     Header file for aildebug.c.
 * @par Purpose:
 *     OpenAL based reimplementation of MSS API.
 * @par Comment:
 *     Wrappers for MSS API functions, providing debug log capabilities.
 * @author   Tomasz Lis
 * @date     12 Jun 2022 - 05 Sep 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef AIL2OAL_AILDEBUG_H_
#define AIL2OAL_AILDEBUG_H_

#include "mssal.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

AIL_DRIVER *AIL_install_driver(const uint8_t *driver_image, uint32_t n_bytes);

const SNDCARD_IO_PARMS *AIL2OAL_get_IO_environment(AIL_DRIVER *drvr);

uint32_t AIL_sample_status(SNDSAMPLE *s);

MDI_DRIVER *AIL_install_MDI_driver_file(char *filename, SNDCARD_IO_PARMS *iop);

void AIL_uninstall_MDI_driver(MDI_DRIVER *mdidrv);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // AIL2OAL_AILDEBUG_H_
