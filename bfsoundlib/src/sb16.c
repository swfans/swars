/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file sb16.c
 *     SoundBlaster 16 specific routines.
 * @par Purpose:
 *     Functions which are specific to SoundBlaster 16 and not part of AIL.
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
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "sb16.h"
/******************************************************************************/

ushort current_SB16_left_master_volume = 0;
ushort current_SB16_right_master_volume = 0;
ushort old_SB16_left_midi_volume = 0;
ushort old_SB16_right_midi_volume = 0;
ushort old_SB16_left_digi_volume = 0;
ushort old_SB16_right_digi_volume = 0;
ushort old_SB16_left_treble_volume = 0;
ushort old_SB16_right_treble_volume = 0;
ushort old_SB16_left_bass_volume = 0;
ushort old_SB16_right_bass_volume = 0;
ubyte sb16_mixer_set = 0;

/******************************************************************************/

void prepare_SB16_volumes(void)
{
#if defined(DOS)||defined(GO32)
    ubyte b;

    if (SoundInstallChoice.IO.IO != 0x220)
        return;
    // Get master volume
    outb(0x224, 0x30); // Select master left volume register
    b = inb(0x225);
    current_SB16_left_master_volume = b >> 3;
    outb(0x224, 0x31); // Select master right volume register
    b = inb(0x225);
    current_SB16_right_master_volume = b >> 3;
    // Get digi/voice volume, set relatively to master
    outb(0x224, 0x32); // Select digi/voice left volume register
    b = inb(0x225);
    old_SB16_left_digi_volume = b;
    outb(0x225, 8 * current_SB16_left_master_volume - 24);
    outb(0x224, 0x33); // Select digi/voice right volume register
    b = inb(0x225);
    old_SB16_right_digi_volume = b;
    outb(0x225, 8 * current_SB16_right_master_volume - 24);
    // Get treble volume, set to const value
    outb(0x224, 0x44);
    b = inb(0x225);
    old_SB16_left_treble_volume = b;
    outb(0x225, 0x60);
    outb(0x224, 0x45);
    b = inb(0x225);
    old_SB16_right_treble_volume = b;
    outb(0x225, 0x60);
    // Get bass volume, set to const value
    outb(0x224, 0x46);
    b = inb(0x225);
    old_SB16_left_bass_volume = b;
    outb(0x225, 0x80);
    outb(0x224, 0x47);
    b = inb(0x225);
    old_SB16_right_bass_volume = b;
    outb(0x225, 0x80);
    // Get MIDI volume, set relatively to master
    outb(0x224, 0x34); // Select midi left volume register
    b = inb(0x225);
    old_SB16_left_midi_volume = b;
    outb(0x225, 8 * current_SB16_left_master_volume - 16);
    outb(0x224, 0x35); // Select midi right volume register
    b = inb(0x225);
    old_SB16_right_midi_volume = b;
    outb(0x225, 8 * current_SB16_right_master_volume - 16);
#endif
    sb16_mixer_set = 1;
}

void reset_SB16_volumes(void)
{
#if defined(DOS)||defined(GO32)
    if (SoundInstallChoice.IO.IO != 0x220)
        return;
    // Re-set digi/voice volume
    outb(0x224, 0x32);
    outb(0x225, old_SB16_left_digi_volume);
    outb(0x224, 0x33);
    outb(0x225, old_SB16_right_digi_volume);
    // Re-set MIDI volume
    outb(0x224, 0x34);
    outb(0x225, old_SB16_left_midi_volume);
    outb(0x224, 0x35);
    outb(0x225, old_SB16_right_midi_volume);
    // Re-set treble volume
    outb(0x224, 0x44);
    outb(0x225, old_SB16_left_treble_volume);
    outb(0x224, 0x45);
    outb(0x225, old_SB16_right_treble_volume);
    // Re-set bass volume
    outb(0x224, 0x46);
    outb(0x225, old_SB16_left_bass_volume);
    outb(0x224, 0x47);
    outb(0x225, old_SB16_right_bass_volume);
#endif
}

/******************************************************************************/
