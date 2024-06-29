/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file awe32.h
 *     Header file for awe32.c.
 * @par Purpose:
 *     SoundBlaster AWE32 specific routines.
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
#include "bftypes.h"
#include "mssal.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

typedef struct SF_DATA SF_DATA;
typedef struct SF_INFO SF_INFO;

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
#if defined(DOS)||defined(GO32)
    uint16_t data_seg;
#else
    void *data_ptr;
#endif
};

struct SF_INFO {
    short no_sample_packets;  /**< Number of packets of sound sample to stream */
    long sample_seek;         /**< Start file location of sound sample */
    long preset_seek;         /**< Address of preset_seek location */
    long preset_read_size;    /**< Number of bytes from preset_seek to allocate and read */
};

/******************************************************************************/

extern TbBool UseCurrentAwe32Soundfont;
extern TbBool Awe32SoundfontLoaded;

/**
 * Returns the total amount of RAM found on the AWE32 card.
 *
 * @param mdidrv pointer to MSS MIDI driver
 * @return Total RAM on card or ERROR (-1)
 */
long AWEGetTotalRAM(MDI_DRIVER *mdidrv);

/**
 * Defines the way that memory on the AWE32 will be partitioned.
 * Uses global variable "awe_data".
 *
 * @param mdidrv Pointer to MSS MIDI driver
 * @param part_cnt Number of partitions
 * @param memMap Array of partition sizes
 * @return Gives OK (0) or ERROR (-1)
 */
int AWEDefMemMap(MDI_DRIVER *mdidrv, long *memMap, short part_cnt);

/**
 * Uses header to retrieve pertinent info about a Sound Font.
 * Uses global variable "awe_data".
 *
 * @param mdidrv Pointer to MSS MIDI driver
 * @param hdr_sata Buffer containing the 1st 512 bytes of a Sound Font.
 * @param bank_no Bank which will receive Sound Font
 * @return Pointer to Sound Font Info or NULL on error
 */
SF_INFO *AWEGetSFInfo(MDI_DRIVER *mdidrv, uint8_t *hdr_data, short bank_no);

/**
 * Loads 16 bit mono PCM data into the RAM on the AWE32.
 * Call repeatedly until all data is streamed to the card.
 *
 * @param mdidrv Pointer to MSS MIDI driver
 * @param smpl_data Pointer to a buffer containing PCM sample data.
 * @param bank_no Bank which will receive Sound Font
 * @return Gives OK (0) or ERROR (-1)
 */
int AWEStreamSample(MDI_DRIVER *mdidrv, uint8_t *smpl_data, short bank_no);

/**
 * Loads Sound Font preset data.
 * Buffer must not be freed until Sound Font is no longer needed.
 *
 * @param mdidrv Pointer to MSS MIDI driver
 * @param preset_data Pointer to preset data. Must exist within the 1st meg of memory.
 * @param bank_no Bank which will receive preset
 * @return Gives OK (0) or ERROR (-1)
 */
int AWELoadPreset(MDI_DRIVER *mdidrv, uint8_t *preset_data, short bank_no);

/** Release one or all of the banks in the AWE32 memory, free preset buffer.
 *
 * @param mdidrv Pointer to MSS MIDI driver
 * @param bank_no Bank to be freed, or ALL (-1)
 * @return Gives OK (0) or ERROR (-1)
 */
int AWEFreeMem(MDI_DRIVER *mdidrv, short bank_no);

void FreeAwe32Soundfont(void);
void LoadAwe32Soundfont(const char *str);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // AIL2OAL_AWE32_H_
