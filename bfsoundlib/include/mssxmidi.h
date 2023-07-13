/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file mssxmidi.h
 *     Header file for mssxmidi.c.
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
#ifndef AIL2OAL_MSSXMIDI_H_
#define AIL2OAL_MSSXMIDI_H_

#include "mssal.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

void AIL2OAL_API_set_GTL_filename_prefix(char const *prefix);

int32_t AIL2OAL_API_MDI_driver_type(MDI_DRIVER *mdidrv);

void AIL2OAL_API_set_XMIDI_master_volume(MDI_DRIVER *mdidrv, int32_t master_volume);

int32_t AIL2OAL_API_install_MDI_INI(MDI_DRIVER **mdidrv);

void AIL2OAL_API_release_channel(MDI_DRIVER *mdidrv, int32_t channel);

MDI_DRIVER *AIL2OAL_API_install_MDI_driver_file(char *filename, SNDCARD_IO_PARMS *iop);
void AIL2OAL_API_uninstall_MDI_driver(MDI_DRIVER *mdidrv);

MDI_DRIVER *AIL2OAL_API_open_XMIDI_driver(uint32_t flags);
void AIL2OAL_API_close_XMIDI_driver(MDI_DRIVER *mdidrv);

SNDSEQUENCE *AIL2OAL_API_allocate_sequence_handle(MDI_DRIVER *mdidrv);

void AIL2OAL_API_release_sequence_handle(SNDSEQUENCE *seq);

int32_t AIL2OAL_API_init_sequence(SNDSEQUENCE *seq, const void *start,  int32_t sequence_num);

void AIL2OAL_API_start_sequence(SNDSEQUENCE *seq);

void AIL2OAL_API_stop_sequence(SNDSEQUENCE *seq);

void AIL2OAL_API_resume_sequence(SNDSEQUENCE *seq);

void AIL2OAL_API_end_sequence(SNDSEQUENCE *seq);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // AIL2OAL_MSSXMIDI_H_
