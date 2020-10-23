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
#include "bflib_picture.h"
#include "unix.h"
#include "windows.h"
#include "dos.h"

static char data_path_user[DISKPATH_SIZE] = "";
static char data_path_hdd[DISKPATH_SIZE] = "";
static char game_dir_savegame[] = "qdata/savegame";
static char game_dir_screenshots[] = "qdata/screenshots";
static char game_file_text_dat[] = "data/text.dat";

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
        DEBUGLOG(0,"Dir for user files '%s'", data_path_user);
        //char path_create[DISKPATH_SIZE];
        //snprintf(path_create, sizeof(path_create), "%s/%s", data_path_user, game_dir_savegame);
        LbDirectoryMake(game_dir_savegame, true);
        //snprintf(path_create, sizeof(path_create), "%s/%s", data_path_user, game_dir_screenshots);
        LbDirectoryMake(game_dir_screenshots, true);
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
    return cd_drive;
}

int
GetDirPlaceFromPath(const char *fname)
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
    // First pass - try matching whole path
    for (dir_place = 0; dir_place < (int)(sizeof(game_dirs)/sizeof(game_dirs[0])); dir_place++)
    {
        PathInfo *pinfo;
        pinfo = &game_dirs[dir_place];
        if (pinfo->directory != NULL) {
            if (strncasecmp(fname, pinfo->directory, path_len) == 0)
                return dir_place;
        }
    }
    // Second pass - match starting part of the path
    for (dir_place = 0; dir_place < (int)(sizeof(game_dirs)/sizeof(game_dirs[0])); dir_place++)
    {
        PathInfo *pinfo;
        pinfo = &game_dirs[dir_place];
        if (pinfo->directory != NULL) {
            path_len = strlen(pinfo->directory);
            if (strncasecmp(fname, pinfo->directory, path_len) == 0) {
                if ((fname[path_len] == '/') || (fname[path_len] == '\\'))
                    return dir_place;
            }
        }
    }
    return DirPlace_None;
}

static void
replace_backslash_with_fs_separator(char *path)
{
    size_t n;
    for (n = 0; path[n] != '\0'; n++)
    {
#if FS_SEP == '\\'
        if (path[n] == '/')
#else
        if (path[n] == '\\')
#endif
            path[n] = FS_SEP;
    }
}

void
SyndFileNameTransform(char *out_fname, const char *inp_fname)
{
    char fs_fname[DISKPATH_SIZE];
    int dir_place;
    const char *base_dir;

    // Figure out whether the base folder should be data folder, user folder or CD
    dir_place = GetDirPlaceFromPath(inp_fname);
    if ( (dir_place == DirPlace_QData) && (strncasecmp(inp_fname, game_dir_savegame, strlen(game_dir_savegame)) == 0) ) {
        base_dir = GetDirectoryUser();
    }
    else if ( (dir_place == DirPlace_QData) && (strncasecmp(inp_fname, game_dir_screenshots, strlen(game_dir_screenshots)) == 0) ) {
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

    // Special file name switch for using language-specific files from CD
    if ( (dir_place == DirPlace_Data) && game_dirs[dir_place].use_cd &&
      (strcasecmp(inp_fname, game_file_text_dat) == 0) ) {
        snprintf(fs_fname, DISKPATH_SIZE, "language/%s/text.dat", language_3str);
    } else {
        strncpy(fs_fname, inp_fname, DISKPATH_SIZE);
    }
    // Switch the input folder separators to proper ones for current os
    replace_backslash_with_fs_separator(fs_fname);
    // Add base path only if the input one is not absolute
    if (fs_fname[0] == FS_SEP || (strlen(fs_fname) >= 2 && fs_fname[1] == ':')) {
        snprintf (out_fname, DISKPATH_SIZE, "%s", fs_fname);
    } else {
        snprintf(out_fname, DISKPATH_SIZE, "%s" FS_SEP_STR "%s", base_dir, fs_fname);
    }
}

void
setup_file_names(void)
{
    lbFileNameTransform = SyndFileNameTransform;
    // This fills the path variable; for user, it also creates the folder
    GetDirectoryHdd();
    GetDirectoryUser();
}

int
get_new_packet_record_no(int map_index)
{
    char fname[DISKPATH_SIZE];
    sprintf(fname, "%s/rec%03d.*", game_dir_savegame, map_index);
    return get_highest_file_no(fname, 7, game_dir_savegame);
}

void
get_packet_record_fname(char *fname, int map_index, int file_no)
{
    sprintf(fname, "%s/rec%03d.%d", game_dir_savegame, map_index, file_no);
}
/******************************************************************************/
