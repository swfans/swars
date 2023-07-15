/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file drv_oal.c
 *     Reimplementation of Bullfrog Sound Library.
 * @par Purpose:
 *     OpenAL output support.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     10 Jun 2023 - 15 Jun 2023
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
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include "drv_oal.h"
#include "aildebug.h"
#include OPENAL_ALC_H
/******************************************************************************/
static ALCdevice *oal_sound_device = NULL;


/******************************************************************************/
#define check_alc(source) check_alc_line((source), __LINE__)

bool check_alc_line(const char *source, int line)
{
    char msg[80];
    ALCenum err;

    assert (oal_sound_device != NULL);

    err = alcGetError(oal_sound_device);
    if (err != ALC_NO_ERROR) {
        snprintf(msg, sizeof(msg), "%s: Error code 0x%x at "__FILE__":%i.\n",
            source, err, line);
        AIL_set_error(msg);
        return false;
    }

    return true;
}

int32_t OPENAL_startup(void)
{
    ALCcontext *sound_context;

    sound_context = NULL;
    oal_sound_device = alcOpenDevice(NULL);
    if (oal_sound_device == NULL) {
        AIL_set_error("alcOpenDevice: Failed to open default OpenAL device.");
        goto err;
    }

    sound_context = alcCreateContext(oal_sound_device, NULL);
    if (!check_alc("alcCreateContext"))
        goto err;

    alcMakeContextCurrent(sound_context);
    if (!check_alc("alcMakeContextCurrent"))
        goto err;

    return 1;

err:
    if (sound_context) {
        alcDestroyContext(sound_context);
        check_alc("alcDestroyContext");
    }
    if (oal_sound_device) {
        alcCloseDevice(oal_sound_device);
        oal_sound_device = NULL;
    }
    return 0;
}

int32_t OPENAL_shutdown(void)
{
    alcDestroyContext(alcGetCurrentContext());
    check_alc("alcDestroyContext");
    alcCloseDevice(oal_sound_device);
    return 1;
}

/******************************************************************************/
