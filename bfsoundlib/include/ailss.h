/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file ailss.h
 *     Header file for ailss.c.
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
#ifndef AIL2OAL_AILSS_H_
#define AIL2OAL_AILSS_H_

#include "mssal.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

DIG_DRIVER *SS_construct_DIG_driver(AIL_DRIVER *drvr, const SNDCARD_IO_PARMS *iop);

SNDSAMPLE *AIL2OAL_API_allocate_sample_handle(DIG_DRIVER *digdrv);

void AIL2OAL_API_init_sample(SNDSAMPLE *s);

uint32_t AIL2OAL_API_sample_status(SNDSAMPLE *s);

AILSAMPLECB AIL2OAL_API_register_EOS_callback(SNDSAMPLE *s, AILSAMPLECB EOS);

void AIL2OAL_API_set_sample_user_data(SNDSAMPLE *s, uint32_t index, int32_t value);

void AIL2OAL_API_end_sample(SNDSAMPLE *s);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // AIL2OAL_AILSS_H_
