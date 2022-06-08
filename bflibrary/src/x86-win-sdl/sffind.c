/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file sffind.c
 *     File listing in a directory routines.
 * @par Purpose:
 *     OS separation library for file i/o and compression routines.
 *     These should be used for all file access in the game.
 * @par Comment:
 *     Wraps OS routines, or simulates the routines to list directory content.
 *     Linux implementation based on findfirst project by Mathieu Turcotte.
 * @author   Tomasz Lis, Mathieu Turcotte
 * @date     10 Feb 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bffile.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "privbflog.h"

#if defined(LB_FILE_FIND_SIMULATED)
#include <dirent.h>
#include <stdint.h>
#include <libgen.h>
#include <errno.h>
#endif

#if defined(WIN32)

// Selected declarations frow Win32 API - we don't want to use whole API
// since it influences everything

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WINBASEAPI
#ifdef __W32API_USE_DLLIMPORT__
#define WINBASEAPI DECLSPEC_IMPORT
#else
#define WINBASEAPI
#endif
#endif
#define WINAPI __stdcall

typedef char *PCHAR,*LPCH,*PCH,*NPSTR,*LPSTR,*PSTR;
typedef const char *LPCCH,*PCSTR,*LPCSTR;
typedef unsigned long DWORD;
typedef int WINBOOL,*PWINBOOL,*LPWINBOOL;
#define BOOL WINBOOL
typedef void *PVOID,*LPVOID;
typedef PVOID HANDLE;
#define DECLARE_HANDLE(n) typedef HANDLE n
typedef HANDLE *PHANDLE,*LPHANDLE;

WINBASEAPI DWORD WINAPI GetShortPathNameA(LPCSTR,LPSTR,DWORD);
#define GetShortPathName GetShortPathNameA

#ifdef __cplusplus
}
#endif

#endif // defined(WIN32)

/** @internal
 */
TbResult LbDateTimeDecode(const time_t *datetime, struct TbDate *curr_date,
  struct TbTime *curr_time);

/** @internal
 * Converts file search information from platform-specific into independent form.
 * Yeah, right...
 */
static void convert_find_info(struct TbFileFind *ffind)
{
    struct _finddata_t *fdata = &(ffind->Reserved);

    strncpy(ffind->Filename,fdata->name,144);
    ffind->Filename[sizeof(ffind->Filename)-1] = '\0';
#if defined(WIN32)
    GetShortPathName(fdata->name,ffind->AlternateFilename,14);
#else
    strncpy(ffind->AlternateFilename,fdata->name,14);
#endif
    ffind->AlternateFilename[sizeof(ffind->AlternateFilename)-1] = '\0';
#if defined(LB_FILE_FIND_SIMULATED)
    if (fdata->st.st_size > ULONG_MAX)
        ffind->Length = ULONG_MAX;
    else
        ffind->Length = fdata->st.st_size;
    ffind->Attributes = S_ISDIR(fdata->st.st_mode) ? Lb_FILE_ATTR_SUBDIR : Lb_FILE_ATTR_NORMAL;
    LbDateTimeDecode(&fdata->st.st_ctime, &ffind->CreationDate, &ffind->CreationTime);
    LbDateTimeDecode(&fdata->st.st_mtime, &ffind->LastWriteDate, &ffind->LastWriteTime);
#else
    if (fdata->size > ULONG_MAX)
        ffind->Length = ULONG_MAX;
    else
        ffind->Length = fdata->size;
    ffind->Attributes = fdata->attrib; /* our Lb_FILE_ATTR_* flags are compatible with Windows definitions */
    LbDateTimeDecode(&fdata->time_create, &ffind->CreationDate, &ffind->CreationTime);
    LbDateTimeDecode(&fdata->time_write, &ffind->LastWriteDate, &ffind->LastWriteTime);
#endif
}

#if defined(LB_FILE_FIND_SIMULATED)

#define DOTDOT_HANDLE    0L
#define INVALID_HANDLE  -1L

typedef struct fhandle_t {
    DIR* dstream;
    short dironly;
    char* spec;
} fhandle_t;

intptr_t _findfirst(const char* filespec, struct _finddata_t* fileinfo);
int _findnext(intptr_t fhandle, struct _finddata_t* fileinfo);
int _findclose(intptr_t fhandle);

/*
 * Windows implementation of _findfirst either returns EINVAL,
 * ENOENT or ENOMEM. This function makes sure that the above
 * implementation doesn't return anything else when an error
 * condition is encountered.
 */
static void findfirst_set_errno() {
    if (errno != ENOENT &&
        errno != ENOMEM &&
        errno != EINVAL) {
        errno = EINVAL;
    }
}

int _match_spec(const char* spec, const char* text) {
    /*
     * If the whole specification string was consumed and
     * the input text is also exhausted: it's a match.
     */
    if (spec[0] == '\0' && text[0] == '\0') {
        return 1;
    }

    /* A star matches 0 or more characters. */
    if (spec[0] == '*') {
        /*
         * Skip the star and try to find a match after it
         * by successively incrementing the text pointer.
         */
        do {
            if (_match_spec(spec + 1, text)) {
                return 1;
            }
        } while (*text++ != '\0');
    }

    /*
     * An interrogation mark matches any character. Other
     * characters match themself. Also, if the input text
     * is exhausted but the specification isn't, there is
     * no match.
     */
    if (text[0] != '\0' && (spec[0] == '?' || spec[0] == text[0])) {
        return _match_spec(spec + 1, text + 1);
    }

    return 0;
}

int match_spec(const char* spec, const char* text) {
    /* On Windows, *.* matches everything. */
    if (strcmp(spec, "*.*") == 0) {
        return 1;
    }

    return _match_spec(spec, text);
}

/* Perfom a scan in the directory identified by dirpath. */
static intptr_t findfirst_in_directory(const char* dirpath,
        const char* spec, struct _finddata_t* fileinfo) {
    DIR* dstream;
    fhandle_t* ffhandle;

    if (spec[0] == '\0') {
        errno = ENOENT;
        return INVALID_HANDLE;
    }

    if ((dstream = opendir(dirpath)) == NULL) {
        findfirst_set_errno();
        return INVALID_HANDLE;
    }

    if ((ffhandle = malloc(sizeof(fhandle_t))) == NULL) {
        closedir(dstream);
        errno = ENOMEM;
        return INVALID_HANDLE;
    }

    /* On Windows, *. returns only directories. */
    ffhandle->dironly = strcmp(spec, "*.") == 0 ? 1 : 0;
    ffhandle->dstream = dstream;
    ffhandle->spec = strdup(spec);

    if (_findnext((intptr_t) ffhandle, fileinfo) != 0) {
        _findclose((intptr_t) ffhandle);
        errno = ENOENT;
        return INVALID_HANDLE;
    }

    return (intptr_t) ffhandle;
}

/* On Windows, . and .. return canonicalized directory names. */
static intptr_t findfirst_dotdot(const char* filespec,
        struct _finddata_t* fileinfo) {
    char* dirname;
    char* canonicalized;

    if (stat(filespec, &fileinfo->st) != 0) {
        findfirst_set_errno();
        return INVALID_HANDLE;
    }

    /* Resolve filespec to an absolute path. */
    if ((canonicalized = realpath(filespec, NULL)) == NULL) {
        findfirst_set_errno();
        return INVALID_HANDLE;
    }

    /* Retrieve the basename from it. */
    dirname = basename(canonicalized);

    /* Make sure that we actually have a basename. */
    if (dirname[0] == '\0') {
        free(canonicalized);
        errno = ENOENT;
        return INVALID_HANDLE;
    }

    /* Make sure that we won't overflow finddata_t::name. */
    if (strlen(dirname) > 259) {
        free(canonicalized);
        errno = ENOMEM;
        return INVALID_HANDLE;
    }

    strcpy(fileinfo->name, dirname);

    free(canonicalized);

    /*
     * Return a special handle since we can't return
     * NULL. The findnext and findclose functions know
     * about this custom handle.
     */
    return DOTDOT_HANDLE;
}

intptr_t _findfirst(const char* filespec, struct _finddata_t* fileinfo) {
    char* rmslash;      /* Rightmost forward slash in filespec. */
    const char* spec;   /* Specification string. */

    if (!fileinfo || !filespec) {
        errno = EINVAL;
        return INVALID_HANDLE;
    }

    if (filespec[0] == '\0') {
        errno = ENOENT;
        return INVALID_HANDLE;
    }

    rmslash = strrchr(filespec, FS_SEP);

    if (rmslash != NULL) {
        /*
         * At least one forward slash was found in the filespec
         * string, and rmslash points to the rightmost one. The
         * specification part, if any, begins right after it.
         */
        spec = rmslash + 1;
    } else {
        /*
         * Since no slash was found in the filespec string, its
         * entire content can be used as our spec string.
         */
        spec = filespec;
    }

    if (strcmp(spec, ".") == 0 || strcmp(spec, "..") == 0) {
        /* On Windows, . and .. must return canonicalized names. */
        return findfirst_dotdot(filespec, fileinfo);
    } else if (rmslash == filespec) {
        /*
         * Since the rightmost slash is the first character, we're
         * looking for something located at the file system's root.
         */
        return findfirst_in_directory("/", spec, fileinfo);
    } else if (rmslash != NULL) {
        /*
         * Since the rightmost slash isn't the first one, we're
         * looking for something located in a specific folder. In
         * order to open this folder, we split the folder path from
         * the specification part by overwriting the rightmost
         * forward slash.
         */
        size_t pathlen = strlen(filespec) +1;
        char* dirpath = alloca(pathlen);
        memcpy(dirpath, filespec, pathlen);
        dirpath[rmslash - filespec] = '\0';
        return findfirst_in_directory(dirpath, spec, fileinfo);
    } else {
        /*
         * Since the filespec doesn't contain any forward slash,
         * we're looking for something located in the current
         * directory.
         */
        return findfirst_in_directory(".", spec, fileinfo);
    }
}

int _findnext(intptr_t fhandle, struct _finddata_t* fileinfo) {
    struct dirent entry, *result;
    struct fhandle_t* handle;

    if (fhandle == DOTDOT_HANDLE) {
        errno = ENOENT;
        return -1;
    }

    if (fhandle == INVALID_HANDLE || !fileinfo) {
        errno = EINVAL;
        return -1;
    }

    handle = (struct fhandle_t*) fhandle;

    while (readdir_r(handle->dstream, &entry, &result) == 0 && result != NULL) {
        if (!handle->dironly && !match_spec(handle->spec, entry.d_name)) {
            continue;
        }

        if (fstatat(dirfd(handle->dstream), entry.d_name, &fileinfo->st, 0) == -1) {
            return -1;
        }

        if (handle->dironly && !S_ISDIR(fileinfo->st.st_mode)) {
            continue;
        }

        strcpy(fileinfo->name, entry.d_name);

        return 0;
    }

    errno = ENOENT;
    return -1;
}

int _findclose(intptr_t fhandle) {
    struct fhandle_t* handle;

    if (fhandle == DOTDOT_HANDLE) {
        return 0;
    }

    if (fhandle == INVALID_HANDLE) {
        errno = ENOENT;
        return -1;
    }

    handle = (struct fhandle_t*) fhandle;

    closedir(handle->dstream);
    free(handle->spec);
    free(handle);

    return 0;
}
#endif

TbResult LbFileFindFirst(const char *filespec, struct TbFileFind *ffind, unsigned int attributes)
{
    int result;
#if LB_FILENAME_TRANSFORM
    char real_filespec[FILENAME_MAX];

    if (lbFileNameTransform != NULL) {
        lbFileNameTransform(real_filespec, filespec);
        filespec = real_filespec;
    }
#endif
    // We skip 'attributes' as Win32 prototypes seem not to use them
    ffind->ReservedHandle = _findfirst(filespec, &(ffind->Reserved));
    if (ffind->ReservedHandle == -1)
    {
      result = -1;
    } else
    {
      convert_find_info(ffind);
      result = 1;
    }
    return result;
}

TbResult LbFileFindNext(struct TbFileFind *ffind)
{
    int result;
    if ( _findnext(ffind->ReservedHandle, &(ffind->Reserved)) < 0 )
    {
        _findclose(ffind->ReservedHandle);
        ffind->ReservedHandle = -1;
        result = Lb_FAIL;
    } else
    {
        convert_find_info(ffind);
        result = 1;
    }
    return result;
}

TbResult LbFileFindEnd(struct TbFileFind *ffind)
{
    if (ffind->ReservedHandle != -1)
    {
        _findclose(ffind->ReservedHandle);
    }
    return 1;
}

/******************************************************************************/
