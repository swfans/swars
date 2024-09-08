#ifndef DOS_H
#define DOS_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "bfdos.h"

#define DOS_O_RDONLY    0x0000
#define DOS_O_WRONLY    0x0001
#define DOS_O_RDWR      0x0002
#define DOS_O_APPEND    0x0010
#define DOS_O_CREAT     0x0020
#define DOS_O_TRUNC     0x0040
#define DOS_O_NOINHERIT 0x0080
#define DOS_O_TEXT      0x0100
#define DOS_O_BINARY    0x0200
#define DOS_O_EXCL      0x0400

#define DOS_CLOCKS_PER_SEC 100


struct dostime_t
{
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  uint8_t hsecond; /* .01 of a sec */
};


struct dosdate_t
{
  uint8_t  day;
  uint8_t  month;
  uint16_t year;
  uint8_t  dayofweek;
};


int dos_open_flags_to_native (int flags);

bool dos_path_to_native (const char *path, char *buffer, size_t len);

int dos_sopen (const char *path, int open_flags, int share_flags, ...);

int dos_open (const char *path, int open_flags, ...);

uint32_t dos_clock(void);

void dos_free(unsigned short n);

int dos_int386 (int num, union REGS *regs, union REGS *out_regs)
      __attribute__ ((noreturn));

int dos_int386x (int num, union REGS *regs, union REGS *out_regs,
		 SREGS *sregs)
      __attribute__ ((noreturn));

void *dos_getvect (int num) __attribute__ ((noreturn));

void dos_setvect (int num, void *function) __attribute__ ((noreturn));

int dos_access(const char *path, int access_flags);

#endif
