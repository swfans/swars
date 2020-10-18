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
#include "dos.h"

static char data_path_user[DISKPATH_SIZE] = "";
static char data_path_hdd[DISKPATH_SIZE] = "";
static char game_dir_savegame[] = "qdata" FS_SEP_STR "savegame";

/******************************************************************************/

const char *
GetDirectoryUser(void)
{
    if (data_path_user[0] == '\0')
    {
        if (!sys_get_user_path(data_path_user, sizeof(data_path_user)))
        {
            snprintf(data_path_user, sizeof(data_path_user), ".");
        }
        DEBUGLOG(0,"Dir for user files '%s'\n", data_path_user);
        char path_create[DISKPATH_SIZE];
        snprintf(path_create, sizeof(path_create), "%s" FS_SEP_STR "%s", data_path_user, game_dir_savegame);
        LbDirectoryMake(path_create, true);
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
        DEBUGLOG(0,"Dir with HDD data '%s'\n",data_path_hdd);
    }
    return data_path_hdd;
}


const char *
GetDirectoryCd(void)
{
    return cd_drive;
}

int GetDirPlaceFromPath(const char *fname)
{
    char *last_fs_sep;
    int path_len;
    int dir_place;

    last_fs_sep = strrchr(fname, '/');
    if (last_fs_sep == NULL)
        last_fs_sep = strrchr(fname, '\\');
    if (last_fs_sep == NULL)
        return DirPlace_None;
    path_len = (int)(last_fs_sep - fname);
    for (dir_place = 0; dir_place < (int)(sizeof(game_dirs)/sizeof(game_dirs[0])); dir_place++)
    {
        PathInfo *pinfo;
        pinfo = &game_dirs[dir_place];
        if (pinfo->directory != NULL) {
            if (strncmp(fname, pinfo->directory, path_len) == 0)
                return dir_place;
        }
    }
    return DirPlace_None;
}

void SyndFileNameTransform(char *out_fname, const char *inp_fname)
{
    char transformed[DISKPATH_SIZE];
    int dir_place;
    const char *base_dir;

    // Figure out whether the base folder should be data folder, user folder or CD
    dir_place = GetDirPlaceFromPath(inp_fname);
    if ( (dir_place == DirPlace_QData) && (strcasecmp(inp_fname, game_dir_savegame) == 0) ) {
        base_dir = GetDirectoryUser();
    }
    else if (dir_place != DirPlace_None) {
        if (game_dirs[dir_place].use_cd) {
            base_dir = GetDirectoryCd();
        } else {
            base_dir = GetDirectoryHdd();
        }
    } else {
        base_dir = GetDirectoryHdd();
    }

    // Add base path only if the input one is not absolute
    if (inp_fname[0] == '\\' || inp_fname[0] == '/'
      || (strlen (inp_fname) >= 2 && inp_fname[1] == ':')) {
        snprintf (transformed, DISKPATH_SIZE, "%s", inp_fname);
    } else {
        snprintf(transformed, DISKPATH_SIZE, "%s" FS_SEP_STR "%s", base_dir, inp_fname);
    }
    dos_path_to_native(transformed, out_fname, DISKPATH_SIZE);
}

void setup_file_names(void)
{
    lbFileNameTransform = SyndFileNameTransform;
    // This fills the path variable; for user, it also creates the folder
    GetDirectoryHdd();
    GetDirectoryUser();
}
/******************************************************************************/
