/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file variable.c
 *     Reimplementation of Bullfrog Sound Library.
 * @par Purpose:
 *     Handles for global properties.
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
#include <stdbool.h>
#include <assert.h>

#include "bfsvaribl.h"
/******************************************************************************/

extern TbBool MusicInstalled;
extern TbBool MusicAble;
extern TbBool MusicActive;
extern TbBool SoundInstalled;
extern TbBool SoundAble;
extern TbBool SoundActive;

/******************************************************************************/

TbBool GetSoundAble(void)
{
    return SoundAble;
}

TbBool GetMusicAble(void)
{
    return MusicAble;
}

TbBool GetSoundInstalled(void)
{
    return SoundInstalled;
}

TbBool GetSoundActive(void)
{
    return SoundActive;
}


/******************************************************************************/
