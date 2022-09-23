/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file awe32.h
 *     Header file for awe32.c.
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
#ifndef AIL2OAL_AWE32_H_
#define AIL2OAL_AWE32_H_

#include <stdint.h>
#include "mssal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SF_DATA SF_DATA;

/**
 * Sound Font functions
 */
enum AweSoundFontFunctions {
    AWESF_GETTOTALRAM   = 0x10,
    AWESF_DEFBANKSIZES  = 0x11,
    AWESF_FREEBANK      = 0x12,
    AWESF_LOADREQ       = 0x17,
    AWESF_STREAMSMPL    = 0x18,
    AWESF_SETPRESETS    = 0x19,
};

struct SF_DATA {
    int16_t parm;
    uint16_t data_seg;
};

/** Release one or all of the banks in the AWE32 memory, free preset buffer.
 *
 * @param mdi Pointer to MSS MIDI driver
 * @param bank_no Bank to be freed, or ALL (-1)
 * @return OK (0) or ERROR (-1)
 */
int AWEFreeMem(MDI_DRIVER *mdidrv, short bank_no);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // AIL2OAL_AWE32_H_
