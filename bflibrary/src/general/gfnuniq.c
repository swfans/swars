/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gfnuniq.c
 *     Unique file names generation.
 * @par Purpose:
 *     Prepares unique file names, to be used for storing images and logs.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     10 Feb 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bffnuniq.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bffile.h"
#include "bflog.h"
#include "bfdir.h"

static char dir_images[FILENAME_MAX] = "qdata/screenshots";

unsigned int get_highest_file_no(const char *fnmask, int no_pos_in_fnmask, const char *fndir)
{
    unsigned int highest_no, curr_no;
    struct TbFileFind ffind;
    int no_pos_in_fname;
    /* This is what we could do if we didn't have fndir param
    char *last_sep;
    last_sep = strrchr(fnmask, '/');
    if (last_sep == NULL)
        last_sep = strrchr(fnmask, '\\');
    if (last_sep != NULL)
        no_pos_in_fname = no_pos_in_fnmask - (last_sep - fnmask + 1);
    else
        no_pos_in_fname = no_pos_in_fnmask;
    */
    no_pos_in_fname = no_pos_in_fnmask - (strlen(fndir) + 1);
    highest_no = 0;
    if ( LbFileFindFirst(fnmask, &ffind, 0x21u) != -1 )
    {
        do {
            curr_no = atol(ffind.Filename + no_pos_in_fname);
            if (curr_no > highest_no)
                highest_no = curr_no;
        } while ( LbFileFindNext(&ffind) != -1 );
    }
    return highest_no;
}

TbResult prepare_unique_file_name(char *out_fname, const char *dir_base,
    const char *fn_base, const char *ext)
{
    unsigned int i;
    unsigned int highest_num;
    sprintf(out_fname, "%s/%-5s", dir_base, fn_base);
    for (i = 1; i < FILENAME_MAX; i++)
    {
        int ch;
        ch = out_fname[i];
        if (ch == ' ')
            break;
        if (ch == '\0')
            break;
    }
    sprintf(&out_fname[i], "*.%s", ext);
    highest_num = get_highest_file_no(out_fname, i, dir_base);
    LIBLOG("%s: highest_num = %d", out_fname, highest_num);
    sprintf(&out_fname[i], "%03d.%s", highest_num + 1, ext);

    if (LbDirectoryMake(dir_base, true) != Lb_FAIL)
        return 0;

    return 1;
}

void LbSetImagesDirectory(const char *dir_name)
{
    strncpy(dir_images, dir_name, sizeof(dir_images));
}

TbResult LbPrepareImageFilename(char *out_fname, const char *fn_base,
    const char *ext)
{
    return prepare_unique_file_name(out_fname, dir_images, fn_base, ext);
}

/******************************************************************************/
