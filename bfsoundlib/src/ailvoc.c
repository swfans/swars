/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file ailss.c
 *     OpenAL based reimplementation of MSS API.
 * @par Purpose:
 *     SS functions from MSS API.
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
#include <assert.h>

#include "ailss.h"
#include "aildebug.h"
#include "memfile.h"
/******************************************************************************/

/** .VOC voice block
 */
typedef struct {
    uint8_t block_ID;
    uint8_t block_len[ 3 ];
    uint8_t time_constant;
    uint8_t pack_method;
} BLK_VOIC;

/** .VOC marker block
 */
typedef struct {
    uint8_t block_ID;
    uint8_t block_len[ 3 ];
    int16_t marker;
} BLK_MRKR;

/** .VOC repeat loop block
 */
typedef struct {
    uint8_t block_ID;
    uint8_t block_len[3];
    uint16_t repeat_count;
} BLK_RLOOP;

/** .VOC extended attribute block
 * (always followed by block 1)
 */
typedef struct {
    uint8_t block_ID;
    uint8_t block_len[3];
    uint16_t time_constant;
    uint8_t pack_method;
    uint8_t voice_mode;
} BLK_EATTR;

/** .VOC extended voice block
 * (replaces blocks 1 and 8)
 */
typedef struct {
    uint8_t block_ID;
    uint8_t block_len[3];
    uint32_t sample_rate;
    uint8_t bits_per_sample;
    uint8_t channels;
    uint16_t format;
    uint8_t reserved[4];
} BLK_EVOIC;

/******************************************************************************/

/** Get length of .VOC block.
 */
uint32_t AIL_VOC_block_len(void *block)
{
    return (*(uint32_t *)block) >> 8;
}

/** Terminate playback of .VOC file.
 *
 * Invoke application callback function, if any, and release the sample
 * allocated to play this file.
 */
void AIL_VOC_terminate(SNDSAMPLE *s)
{
    AILSAMPLECB cb;

    cb = (AILSAMPLECB)s->system_data[0];
    if (cb != NULL)
        cb(s);

    if (s->system_data[6] > 0)
        AIL_release_sample_handle(s);

    s->system_data[6] = -1;
}

void AIL_process_VOC_block(SNDSAMPLE *s, int32_t play_flag)
{
#if 0
    asm volatile (
      "push %1\n"
      "push %0\n"
      "call ASM_AIL_process_VOC_block\n"
      "add $0x8, %%esp\n"
        :  : "g" (s), "g" (play_flag) : "eax" );
#endif
    int32_t voice_block;
    void *b;

    voice_block = 0;

    // Loop until voice block is found
    while (!voice_block)
    {
        b = (void*)s->system_data[1]; // Pointer to current block

        switch (*(uint8_t*)b)
        {
        case 0: // Terminator block
            // Terminate playback, then return without advance to next block
            AIL_VOC_terminate(s);
            return;

        case 1: // Voice block
            // Skip block if desired marker has not been found
            if (!s->system_data[5]) // Whether desired marker is found
                break;
            // Set up sample data and start playback
            AIL_set_sample_address(s, b + sizeof(BLK_VOIC), AIL_VOC_block_len(b) - 2);

            AIL_set_sample_playback_rate(s, 1000000 / (256 - ((BLK_VOIC*)b)->time_constant));

            AIL_set_sample_type(s, DIG_F_MONO_8, 0);
            if (play_flag)
                AIL_start_sample(s);
            voice_block = 1;
            break;

        case 4: // Marker block
            // Ignore if entire file to be played
            if (s->system_data[4] == (uintptr_t)-1) // Marker to search for, -1 if all
                break;
            // If this is the desired marker, set MARKER_FOUND flag --
            // otherwise, clear MARKER_FOUND flag to prevent playback
            // of future voice blocks
            if (s->system_data[4] == (uintptr_t)((BLK_MRKR*)b)->marker)
                s->system_data[5] = 1;
            else
                s->system_data[5] = 0;
            break;

        case 6: // Repeat block
            // Log repeat count and starting address of repeat block
            s->system_data[2] = (uintptr_t)b; // Pointer to beginning of repeat loop block
            s->system_data[3] = (uintptr_t)((BLK_RLOOP*)b)->repeat_count; // # of iterations left in repeat loop
            break;

        case 7: // End repeat block
            // If finite repeat block active, check and decrement repeat
            // count
            if (s->system_data[3] != 0xffff) // # of iterations left in repeat loop
            {
                if (s->system_data[3]-- == 0)
                    break;
            }
            b = (void*)s->system_data[2]; // Pointer to beginning of repeat loop block
            break;

        case 8: // Extended attribute block (followed by block 1)
            // Skip block if desired marker has not been found
            if (!s->system_data[5])
                break;

            // Set up sample data and start playback
            if (((BLK_EATTR *)b)->voice_mode)
            {
                AIL_set_sample_type(s, DIG_F_STEREO_8, 0);
                AIL_set_sample_playback_rate(s,
                  128000000LU / (65536LU - ((BLK_EATTR*)b)->time_constant));
            }
            else
            {
                AIL_set_sample_type(s, DIG_F_MONO_8, 0);
                AIL_set_sample_playback_rate(s,
                  256000000LU / (65536LU - ((BLK_EATTR*)b)->time_constant));
            }
            // Advance to paired voice block (type 1) in .VOC image
            b = (uint8_t *)(b + AIL_VOC_block_len(b) + 4);
            // Set sample address and size, and start playback
            AIL_set_sample_address(s, b + sizeof(BLK_VOIC), AIL_VOC_block_len(b) - 2);
            if (play_flag)
                AIL_start_sample(s);
            voice_block = 1;
            break;

        case 9: // Extended voice block
            // Skip block if desired marker has not been found
            if (!s->system_data[5])
                break;
            // Set up sample data and start playback
            AIL_set_sample_address(s, b + sizeof(BLK_EVOIC), AIL_VOC_block_len(b) - 12);
            AIL_set_sample_playback_rate(s, ((BLK_EVOIC*) b)->sample_rate);
            if ((((BLK_EVOIC*)b)->channels == 1) && (((BLK_EVOIC*)b)->format == 0))
                AIL_set_sample_type(s, DIG_F_MONO_8, 0);
            else if ((((BLK_EVOIC*)b)->channels == 2) && (((BLK_EVOIC*)b)->format == 0))
            {
                AIL_set_sample_type(s, DIG_F_STEREO_8, 0);
            }
            else if ((((BLK_EVOIC*)b)->channels == 1) && (((BLK_EVOIC*)b)->format == 4))
            {
                AIL_set_sample_type(s, DIG_F_MONO_16, DIG_PCM_SIGN);
            }
            else if ((((BLK_EVOIC*)b)->channels == 2) && (((BLK_EVOIC*)b)->format == 4))
            {
                AIL_set_sample_type(s, DIG_F_STEREO_16, DIG_PCM_SIGN);
            }
            if (play_flag)
                AIL_start_sample(s);
            voice_block = 1;
            break;
        }

        // Advance pointer to next block in .VOC image
        s->system_data[1] = (uintptr_t)(b + AIL_VOC_block_len(b) + 4);
    }
}

/******************************************************************************/
