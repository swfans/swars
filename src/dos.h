#ifndef DOS_H
#define DOS_H

#include <stddef.h>
#include <stdint.h>


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


struct DOS_Registers32
{
  uint32_t eax;    /* 00 */
  uint32_t ebx;    /* 04 */
  uint32_t ecx;	   /* 08 */
  uint32_t edx;	   /* 0c */
  uint32_t esi;	   /* 10 */
  uint32_t edi;	   /* 14 */
  uint32_t eflags; /* 18 */
} __attribute__ ((packed));

typedef struct DOS_Registers32 DOS_Registers32;


struct DOS_Registers16
{
  uint16_t ax;		uint16_t _unused_1;
  uint16_t bx;		uint16_t _unused_2;
  uint16_t cx;		uint16_t _unused_3;
  uint16_t dx;		uint16_t _unused_4;
  uint16_t si;		uint16_t _unused_5;
  uint16_t di;		uint16_t _unused_6;
  int16_t flags;
} __attribute__ ((packed));

typedef struct DOS_Registers16 DOS_Registers16;


struct DOS_Registers8
{
  uint8_t al;
  uint8_t ah;		uint16_t _unused_1;
  uint8_t bl;
  uint8_t bh;		uint16_t _unused_2;
  uint8_t cl;
  uint8_t ch;		uint16_t _unused_3;
  uint8_t dl;
  uint8_t dh;
} __attribute__ ((packed));

typedef struct DOS_Registers8 DOS_Registers8;


union DOS_Registers
{
  DOS_Registers32 r32;
  DOS_Registers16 r16;
  DOS_Registers8  r8;
};

typedef union DOS_Registers DOS_Registers;


struct DOS_SegmentRegisters
{
  uint16_t es;
  uint16_t cs;
  uint16_t ss;
  uint16_t ds;
  uint16_t fs;
  uint16_t gs;
};

typedef struct DOS_SegmentRegisters DOS_SegmentRegisters;


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

void dos_gettime (struct dostime_t *t);

void dos_getdate (struct dosdate_t *d);

int dos_int386 (int num, DOS_Registers *regs, DOS_Registers *out_regs)
      __attribute__ ((noreturn));

int dos_int386x (int num, DOS_Registers *regs, DOS_Registers *out_regs,
		 DOS_SegmentRegisters *sregs)
      __attribute__ ((noreturn));

void *dos_getvect (int num) __attribute__ ((noreturn));

void dos_setvect (int num, void *function) __attribute__ ((noreturn));

#endif
