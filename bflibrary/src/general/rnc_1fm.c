/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file rnc_1fm.asm
 *     Implementation of related functions.
 * @par Purpose:
 *     Unknown.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "rnc_1fm.h"
#include "bftypes.h"
#include "bfendian.h"
#include "bfmemory.h"
#include "bfmemut.h"
#include "privbflog.h"

typedef struct {
    u32 bitbuf;           /* holds between 16 and 32 bits */
    i32 bitcount;               /* how many bits does bitbuf hold? */
} bit_stream;

typedef struct {
    i32 num;                   /* number of nodes in the tree */
    struct {
    u32 code;
    i32 codelen;
    i32 value;
    } table[32];
} huf_table;

static void read_huftable (huf_table *h, bit_stream *bs,
                   uc8 **p, uc8 *pend);
static i32 huf_read (huf_table *h, bit_stream *bs,
                   uc8 **p,uc8 *pend);

static void bitread_init (bit_stream *bs, uc8 **p, uc8 *pend);
static void bitread_fix (bit_stream *bs, uc8 **p, uc8 *pend);
static u32 bit_peek (bit_stream *bs,  u32 mask);
static void bit_advance (bit_stream *bs, int n,
                   uc8 **p, uc8 *pend);
static u32 bit_read (bit_stream *bs, u32 mask,
                   int n, uc8 **p, uc8 *pend);

static u32 mirror(u32 x, int n);

/*
 * Return an error string corresponding to an error return code.
 */
const char *rnc_error (i32 errcode) {
    static const char *const errors[] = {
        "No error",
        "File is not RNC-1 format",
        "Huffman decode error",
        "File size mismatch",
        "CRC error in packed data",
        "CRC error in unpacked data",
        "Compressed file header invalid",
        "Huffman decode leads outside buffers",
        "Unknown error"
    };
    i32 errlimit;
    errlimit = sizeof(errors)/sizeof(*errors) - 1;
    errcode = -errcode;
    if (errcode < 0)
        errcode = 0;
    if (errcode > errlimit)
        errcode = errlimit;
    return errors[errcode];
}

/** @internal
 * Read a Huffman table out of the bit stream and data stream given.
 */
static void read_huftable (huf_table *h, bit_stream *bs,
                          uc8 **p, uc8 *pend)
{
    int i, j, k, num;
    int leaflen[32];
    int leafmax;
    u32 codeb;           // big-endian form of code

    num = bit_read (bs, 0x1F, 5, p, pend);
    if (!num)
        return;

    leafmax = 1;
    for (i=0; i<num; i++)
    {
        leaflen[i] = bit_read (bs, 0x0F, 4, p, pend);
        if (leafmax < leaflen[i])
            leafmax = leaflen[i];
    }

    codeb = 0L;
    k = 0;
    for (i=1; i<=leafmax; i++)
    {
    for (j=0; j<num; j++)
        if (leaflen[j] == i)
        {
            h->table[k].code = mirror (codeb, i);
            h->table[k].codelen = i;
            h->table[k].value = j;
            codeb++;
            k++;
        }
    codeb <<= 1;
    }

    h->num = k;
}

/** @internal
 * Read a value out of the bit stream using the given Huffman table.
 */
static i32 huf_read (huf_table *h, bit_stream *bs,
                   u8 **p, u8 *pend)
{
    int i;
    u32 val;

    for (i=0; i<h->num; i++)
    {
        u32 mask = (1 << h->table[i].codelen) - 1;
        if (bit_peek(bs, mask) == h->table[i].code)
            break;
    }
    if (i == h->num)
        return -1;
    bit_advance (bs, h->table[i].codelen, p, pend);

    val = h->table[i].value;

    if (val >= 2)
    {
        val = 1 << (val-1);
        val |= bit_read (bs, val-1, h->table[i].value - 1, p, pend);
    }
    return val;
}

/** @internal
 * Initialises a bit stream with the first two bytes of the packed
 * data.
 * Checks pend for proper buffer pointers range.
 */
static void bitread_init (bit_stream *bs, uc8 **p, uc8 *pend)
{
    if (pend-(*p) >= 0)
        bs->bitbuf = lword (*p);
    else
        bs->bitbuf = 0;
    bs->bitcount = 16;
}

/** @internal
 * Fixes up a bit stream after literals have been read out of the
 * data stream.
 * Checks pend for proper buffer pointers range.
 */
static void bitread_fix (bit_stream *bs, uc8 **p, uc8 *pend)
{
    bs->bitcount -= 16;
    bs->bitbuf &= (1<<bs->bitcount)-1; // remove the top 16 bits
    if (pend-(*p) >= 0)
        bs->bitbuf |= (lword(*p)<<bs->bitcount);// replace with what's at *p
    bs->bitcount += 16;
}

/** @internal
 * Returns some bits.
 */
static u32 bit_peek (bit_stream *bs, u32 mask)
{
    return bs->bitbuf & mask;
}

/** @internal
 * Advances the bit stream.
 * Checks pend for proper buffer pointers range.
 */
static void bit_advance (bit_stream *bs, int n, uc8 **p, uc8 *pend)
{
    bs->bitbuf >>= n;
    bs->bitcount -= n;
    if (bs->bitcount < 16)
    {
        (*p) += 2;
        if (pend-(*p) >= 0)
            bs->bitbuf |= (lword(*p)<<bs->bitcount);
        bs->bitcount += 16;
    }
}

/** @internal
 * Reads some bits in one go (ie the above two routines combined).
 */
static u32 bit_read (bit_stream *bs, u32 mask,
                   int n, uc8 **p, uc8 *pend)
{
    u32 result = bit_peek (bs, mask);
    bit_advance (bs, n, p, pend);
    return result;
}

/** @internal
 * Mirror the bottom n bits of x.
 */
static u32 mirror (u32 x, int n) {
    u32 top = 1 << (n-1), bottom = 1;
    while (top > bottom)
    {
        u32 mask = top | bottom;
        u32 masked = x & mask;
        if (masked != 0 && masked != mask)
            x ^= mask;
        top >>= 1;
        bottom <<= 1;
    }
    return x;
}

u16 crctab[256];
i16 crctab_ready=false;

/** @internal
 * Calculate a CRC, the RNC way
 */
i32 rnc_crc(void *data, u32 len)
{
    u16 val;
    int i, j;
    uc8 *p = (uc8 *)data;
    //computing CRC table
    if (!crctab_ready)
    {
      for (i=0; i<256; i++)
      {
          val = i;

          for (j=0; j<8; j++)
          {
            if (val & 1)
                 val = (val >> 1) ^ 0xA001;
            else
              val = (val >> 1);
          }
          crctab[i] = val;
      }
    crctab_ready=true;
    }

    val = 0;
    while (len--)
    {
       val ^= *p++;
       val = (val >> 8) ^ crctab[val & 0xFF];
    }
    return val;
}

/** @internal
 * Decompress a packed data block. Returns the unpacked length if
 * successful, or negative error codes if not.
 *
 * If COMPRESSOR is defined, it also returns the leeway number
 * (which gets stored at offset 16 into the compressed-file header)
 * in `*leeway', if `leeway' isn't NULL.
 */
i32 rnc_unpack(void *packed, void *unpacked, u32 flags
#ifdef COMPRESSOR
         , i32 *leeway
#endif
         )
{
    uc8 *input = (uc8 *)packed;
    uc8 *output = (uc8 *)unpacked;
    uc8 *inputend, *outputend;
    bit_stream bs;
    huf_table raw, dist, len;
    u32 ch_count;
    u32 ret_len, inp_len;
    i32 out_crc;
#ifdef COMPRESSOR
    i32 lee = 0;
#endif
    if (blong(input) != RNC_SIGNATURE)
        if (!(flags & RNC_IGNORE_HEADER_VAL_ERROR)) return RNC_HEADER_VAL_ERROR;
    ret_len = blong(input+4);
    inp_len = blong(input+8);
    if ((ret_len>(1<<30))||(inp_len>(1<<30)))
        return RNC_HEADER_VAL_ERROR;

    outputend = output + ret_len;
    inputend = input + 18 + inp_len;

    input += 18;               // skip header

    // Check the packed-data CRC. Also save the unpacked-data CRC
    // for later.

    if (rnc_crc(input, inputend-input) != (i32)bword(input-4))
        if (!(flags&RNC_IGNORE_PACKED_CRC_ERROR)) return RNC_PACKED_CRC_ERROR;
    out_crc = bword(input-6);

    bitread_init (&bs, &input, inputend);
    bit_advance (&bs, 2, &input, inputend);      // discard first two bits

    // Process chunks.

    while (output < outputend)
    {
#ifdef COMPRESSOR
      i32 this_lee;
#endif
      if (inputend-input < 6)
      {
          if (!(flags & RNC_IGNORE_HUF_EXCEEDS_RANGE))
              return RNC_HUF_EXCEEDS_RANGE;
          else {
              output = outputend;
              ch_count = 0;
              break;
          }
      }
      read_huftable (&raw,  &bs, &input, inputend);
      read_huftable (&dist, &bs, &input, inputend);
      read_huftable (&len,  &bs, &input, inputend);
      ch_count = bit_read (&bs, 0xFFFF, 16, &input, inputend);

      while (1)
      {
        i32 length, posn;

        length = huf_read (&raw, &bs, &input,inputend);
        if (length == -1)
            {
            if (!(flags & RNC_IGNORE_HUF_DECODE_ERROR))
                return RNC_HUF_DECODE_ERROR;
            else
                {output=outputend;ch_count=0;break;}
            }
        if (length)
        {
            while (length--)
            {
                if ((input >= inputend) || (output >= outputend))
                {
                    if (!(flags & RNC_IGNORE_HUF_EXCEEDS_RANGE))
                        return RNC_HUF_EXCEEDS_RANGE;
                    else {
                        output = outputend;
                        ch_count = 0;
                        break;
                    }
                }
                *output++ = *input++;
            }
            bitread_fix (&bs, &input, inputend);
        }
        if (--ch_count <= 0)
            break;

        posn = huf_read (&dist, &bs, &input,inputend);
        if (posn == -1)
        {
            if (!(flags&RNC_IGNORE_HUF_DECODE_ERROR))
                return RNC_HUF_DECODE_ERROR;
            else
                {output=outputend;ch_count=0;break;}
        }
        length = huf_read (&len, &bs, &input,inputend);
        if (length == -1)
        {
            if (!(flags&RNC_IGNORE_HUF_DECODE_ERROR))
                return RNC_HUF_DECODE_ERROR;
            else
                {output=outputend;ch_count=0;break;}
        }
        posn += 1;
        length += 2;
        while (length--)
        {
            if (((output-posn) < (uc8 *)unpacked)
             || ((output-posn) > (uc8 *)outputend)
             || ((output) < (uc8 *)unpacked)
             || ((output) > (uc8 *)outputend))
            {
                   if (!(flags & RNC_IGNORE_HUF_EXCEEDS_RANGE))
                       return RNC_HUF_EXCEEDS_RANGE;
                   else {
                       output = outputend-1;
                       ch_count = 0;
                       break;
                   }
            }
            *output = output[-posn];
            output++;
        }
#ifdef COMPRESSOR
        this_lee = (inputend - input) - (outputend - output);
        if (lee < this_lee)
            lee = this_lee;
#endif
      }
    }

    if (outputend != output)
    {
        if (!(flags & RNC_IGNORE_FILE_SIZE_MISMATCH))
            return RNC_FILE_SIZE_MISMATCH;
    }

#ifdef COMPRESSOR
    if (leeway)
        *leeway = lee;
#endif

    // Check the unpacked-data CRC.

    if (rnc_crc(outputend-ret_len, ret_len) != out_crc)
    {
        if (!(flags & RNC_IGNORE_UNPACKED_CRC_ERROR))
            return RNC_UNPACKED_CRC_ERROR;
    }

    return ret_len;
}

i32 UnpackM1(unsigned char *buffer, u32 bufsize)
{
    LOGWARN("UnpackM1: (expected size %zu)", bufsize);
    //If file isn't compressed - return zero
    if (blong(buffer+0) != RNC_SIGNATURE)
        return 0;
    // Originally this function was able do decompress data without additional buffer.
    // If you know how to decompress the data this way, please correct this.
    u32 packedsize = blong(buffer+4);
    if (packedsize > bufsize)
        packedsize = bufsize;
    void *packed = LbMemoryAlloc(packedsize);
    LbMemoryCopy(packed, buffer, packedsize);
    if (packed == NULL) return -1;
    int retcode = rnc_unpack(packed, buffer, 0);
    LbMemoryFree(packed);
    return retcode;
}

/******************************************************************************/
