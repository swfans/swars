/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file sdir.c
 *     Directory handling routines wrapper.
 * @par Purpose:
 *     OS separation library for directory handling routines.
 * @par Comment:
 *     Wraps standard c or UNIX directory handling functions.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bfdir.h"

#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "bffile.h"
#include "privbflog.h"

TbResult LbDirectoryChange(const char *path)
{
    if ( chdir(path) )
        return Lb_FAIL;
    return Lb_SUCCESS;
}

TbResult LbDirectoryCreate(const char *path)
{
    int err;
#if defined(WIN32)
    err = mkdir(path);
#else
    mode_t mode = 0755;
    err = mkdir(path, mode);
#endif
    if (err != 0) {
        LOGERR("while creating dir \"%s\": %s", path, strerror(errno));
        return Lb_FAIL;
    }
    return Lb_SUCCESS;
}

TbResult LbDirectoryMake(const char *path, TbBool recursive)
{
    char buffer[FILENAME_MAX];
    char fname[FILENAME_MAX];
    char *p;
    size_t len;
    struct stat st;
    int err;
#if !defined(WIN32)
    mode_t mode = 0755;
#endif
    int num_levels = 0;

    // We need to transform the path here - if we did it later,
    // then we would skip base directories in recursion.
    // Also, the function expects file name, not path - make one
    if (lbFileNameTransform != NULL) {
        strncpy(fname, path, FILENAME_MAX-2);
        strcat(fname, "/a");
        lbFileNameTransform(buffer, fname);
        len = strlen(buffer) - 2;
        buffer[len] = '\0';
    } else {
        len = snprintf(buffer, sizeof(buffer), "%s", path);
    }

    // First, find the longest existing path
    do
    {
        err = stat(buffer, &st);
        if (err == 0)
        {
            if (!S_ISDIR(st.st_mode))
            {
                LOGERR("%s: not a directory", buffer);
                return Lb_FAIL;
            }
        }
        else
        {
            if (errno != ENOENT)
            {
                LOGERR("while getting stat on dir \"%s\": %s", buffer, strerror(errno));
                return Lb_FAIL;
            }

            p = strrchr(buffer, FS_SEP);
            if (p == NULL)
                break;
            num_levels++;

            *p = 0;
        }
    }
    while (err != 0);

    if ((num_levels > 1) && (!recursive))
    {
        LOGERR("cannot create %d dirs (recursion disabled), path %s",
          num_levels, buffer);
        return Lb_FAIL;
    }
    // At this point, buffer contains the longest existing path.  Go forward
    // through the rest of the path and create the missing directories.
    p = buffer;

    for (;;)
    {
        while (*p != '\0')
            p++;

        if (p >= buffer + len)
            break;

        *p = FS_SEP;

        LOGSYNC("creating directory '%s'", buffer);
#if defined(WIN32)
        err = mkdir(buffer);
#else
        err = mkdir(buffer, mode);
#endif
        if (err != 0)
        {
            LOGERR("while creating dir \"%s\": %s", path, strerror(errno));
            return Lb_FAIL;
        }
    }

    return Lb_SUCCESS;
}

TbResult LbDirectoryCurrent(char *buf, unsigned long buflen)
{
    if ( getcwd(buf,buflen) == NULL )
        return Lb_FAIL;
    if ( buf[1] == ':' )
        // Unsafe to use strcpy() on overlapping buffers, but we can memmove().
        memmove(buf, buf+2, strlen(buf+2) + 1);
    int len = strlen(buf);
    if ( len > 1 )
    {
        if ( (buf[len-1] == '\\') || (buf[len-1] == '/') )
          buf[len-1] = '\0';
    }
    return Lb_SUCCESS;
}

TbBool LbDirectoryExists(const char *dirname)
{
    struct stat stbuf;

    if ( access(dirname, 0) || stat(dirname, &stbuf) == -1 )
        return false;
    return S_ISDIR(stbuf.st_mode);
}

TbResult LbDirectoryRemove(const char *path)
{
    if ( rmdir(path) )
        return Lb_FAIL;
    return Lb_SUCCESS;
}

/******************************************************************************/
