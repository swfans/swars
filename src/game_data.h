/******************************************************************************/
// Syndicate Wars Port - open-source reimplementation of SW.
/******************************************************************************/
/** @file game_data.h
 *     Header file for game_data.c.
 * @par Purpose:
 *     Structures and functions defining data used by the game.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     10 Feb 2020 - 16 Oct 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef GAME_DATA_H
#define GAME_DATA_H

#include "bflib_basics.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

enum DirPlacement {
    DirPlace_Data = 0,
    DirPlace_QData,
    DirPlace_Maps,
    DirPlace_Levels,
    DirPlace_Intro,
    DirPlace_Sound,
    DirPlace_Equip,
    DirPlace_None,
};

#pragma pack(1)

typedef struct {
    char *directory;
    TbBool use_cd;
} PathInfo;

typedef struct {
    char *Name;
    void **BufferPtr;
    u16 ESize;
    i32 N;
    void *PrivBuffer;
    i32 dum2;
    u8 dum3;
} MemSystem;

#pragma pack()

extern PathInfo game_dirs[8];
extern char cd_drive[52];
extern char language_3str[4];

extern MemSystem mem_game[];

/******************************************************************************/

const char *GetDirectoryUser(void);
const char *GetDirectoryHdd(void);
const char *GetDirectoryCd(void);

void setup_file_names(void);

int get_highest_used_packet_record_no(int campgn, int missi);
void get_packet_record_fname(char *fname, int campgn, int missi, int file_no);
void get_user_settings_fname(char *fname, const char *name);
void get_saved_game_fname(char *fname, u16 slot);

void adjust_memory_use(void);
void init_memory(MemSystem *mem_table);
i32 get_memory_ptr_allocated_count(void **mgptr);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
