/******************************************************************************/
// Syndicate Wars Port - open-source reimplementation of SW.
/******************************************************************************/
/** @file game_data.c
 *     Memory managing routines.
 * @par Purpose:
 *     Structures and functions defining data used by the game.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     10 Feb 2020 - 16 Oct 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "game_data.h"

#include "globals.h"
#include "bflib_basics.h"
#include "bflib_fileio.h"
#include "unix.h"
#include "windows.h"

static char data_path_user[DISKPATH_SIZE] = "";
static char data_path_hdd[DISKPATH_SIZE] = "";
static char data_path_cd[DISKPATH_SIZE] = "";

/******************************************************************************/

const char *
GetDirectoryUser(void)
{
    if (data_path_user[0] == '\0')
    {
        if (!sys_get_user_path(data_path_user, sizeof(data_path_user)))
        {
            snprintf(data_path_user, sizeof(data_path_user), "%s", "qdata" FS_SEP_STR "savegame");
        }
        DEBUGLOG(0,"Dir for user files '%s'",data_path_user);
        LbDirectoryMake(data_path_user, true);
    }
    return data_path_user;
}

const char *
GetDirectoryHdd(void)
{
    if (data_path_hdd[0] == '\0')
    {
        if (!sys_get_data_path(data_path_hdd, sizeof(data_path_hdd)))
        {
            snprintf(data_path_hdd, sizeof(data_path_hdd), "%s", ".");
        }
        DEBUGLOG(0,"Dir with HDD data '%s'",data_path_hdd);
    }
    return data_path_hdd;
}


const char *
GetDirectoryCd(void)
{
    if (data_path_cd[0] == '\0')
    {
    }
    DEBUGLOG(0,"Dir with CD data '%s'",data_path_cd);
    return data_path_cd;
}

/******************************************************************************/
