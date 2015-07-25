#ifndef SOUND_H
#define SOUND_H

#include <stdbool.h>
#include <stdint.h>

#include "timer.h"

enum SoundDriverType
{
  SOUND_DRIVER_PCM  = 0,
  SOUND_DRIVER_MIDI = 1
};


typedef struct SoundDriver SoundDriver;
typedef struct SoundDriverCallParameters SoundDriverCallParameters;
typedef struct SoundIOParameters SoundIOParameters;
typedef struct SoundPCMDriver SoundPCMDriver;
typedef struct SoundSample SoundSample;


struct SoundDriver                   // Handle to driver
{
  /* 00 */ uint32_t seg;                 // Seg:off pointer to driver (off=0)
  /* 04 */ uint32_t sel;                 // Selector for driver (off=0)
  /* 08 */ void	   *buf;                 // Protected-mode pointer to driver
  /* 0c */ uint32_t size;                // Size of driver image
  /* 10 */ void	   *VHDR;                // Pointer to driver header (same as buf)
  /* 14 */ int32_t  type;                // AIL3DIG or AIL3MDI (see below)
  /* 18 */ int32_t  initialized;         // 1 if hardware successfully init'd, else 0
  /* 1c */ int32_t  PM_ISR;              // -1 if no PM ISR hooked, else IRQ #
  /* 20 */ int32_t  server;              // DRV_SERVE periodic timer, if requested
  /* 24 */ void   (*destructor)(void *); // Vector to high-level destructor, if any
  /* 28 */ void    *descriptor; // High-level descriptor (DIG_ or MDI_DRIVER)
} __attribute__ ((packed));

struct SoundPCMDriver
{
  /* FIXME: this is newer and ABI-incompatible */
  /* 00 */ SoundDriver *drvr;                // Base driver descriptor
  /* 04 */ void	       *DDT;                 // Protected-mode pointer to DDT
  /* 08 */ void        *DST;                 // Protected-mode pointer to DST
  /* 0c */ int32_t      timer;               // Buffer-polling timer
  /* 10 */ int32_t      half_buffer_size;    // Size of DMA half-buffer
  /* 14 */ int32_t      DMA_rate;            // Hardware sample rate
  /* 18 */ int32_t      hw_format;           // DIG_F code in use
  /* 1c */ uint32_t     hw_mode_flags;       // DIG_PCM_ flags for mode in use
  /* 20 */ uint32_t     DMA_seg;             // Seg:off pointer to DMA buffers (off=0)
  /* 24 */ uint32_t     DMA_sel;             // Selector for DMA buffers (off=0)
  /* 28 */ void        *DMA_buf;             // Protected-mode pointer to DMA buffers
  /* 2c */ void        *DMA[2];              // Protected-mode pointers to half-buffers
					     // (note that DMA[0] may != DMA_buf)
  /* 34 */ int16_t     *buffer_flag;         // Protected-mode pointer to buffer flag
  /* 38 */ int32_t      last_buffer;         // Last active buffer flag value in driver
  /* 3c */ int32_t      channels_per_sample; // # of channels per sample (1 or 2)
  /* 40 */ int32_t      bytes_per_channel;   // # of bytes per channel (1 or 2)
  /* 44 */ int32_t      channels_per_buffer; // # of channels per half-buffer
  /* 48 */ int32_t      samples_per_buffer;  // # of samples per half-buffer
  /* 4c */ int32_t      build_size;          // # of bytes in build buffer
  /* 50 */ int32_t     *build_buffer;        // Build buffer (4 * n_samples bytes)

  /* confirmed: */
  /* 54 */ int32_t      playing;             // Playback active if non-zero

  /* 58 */ int32_t      quiet;               // # of consecutive quiet sample periods

  /* confirmed: */
  /* 5c */ SoundSample *samples;             // Pointer to list of SAMPLEs
  /* 60 */ int32_t      n_samples;           // # of SAMPLEs

  /* 64 */ int32_t      n_active_samples;    // # of samples being processed

  /* confirmed: */
  /* 68 */ int32_t      master_volume;       // Master sample volume 0-127

  /* 6c */ int32_t      system_data[8];      // Miscellaneous system data
} __attribute__ ((packed));

struct SoundSample
{
  /*  00 */ SoundDriver *driver;            // Driver for playback
  /*  04 */ uint32_t     status;            // SMP_ flags: _FREE, _DONE, _PLAYING
  /*  08 */ void        *start[2];          // Sample buffer address (W)
  /*  10 */ uint32_t     len[2];            // Sample buffer size in bytes (W)
  /*  18 */ uint32_t     pos[2];            // Index to next byte (R/W)
  /*  20 */ uint32_t     done[2];           // Nonzero if buffer with len=0 sent by app
  /*  28 */ int32_t      current_buffer;    // Buffer # active (0/1)
  /*  2c */ int32_t      last_buffer;       // Last active buffer (for double-buffering)
  /*  30 */ int32_t      loop_count;        // 0-inf, 1-1
  /*  34 */ int32_t      format;            // DIG_F format (8/16 bits, mono/stereo)
  /*  38 */ uint32_t     flags;             // DIG_PCM_SIGN / DIG_PCM_ORDER (stereo only)
  /*  3c */ int32_t      playback_rate;     // Playback rate in hertz
  /*  40 */ int32_t      volume;            // Sample volume 0-127
  /*  44 */ int32_t      pan;               // Mono panpot/stereo balance (0=L ... 127=R)
  /*  48 */ int32_t      vol_scale[2][256]; // [left/mono=0,right=1][256] channel volume
  /* ... */ void	*SOB;               // Start-of-block callback function
  /* ... */ void	*EOB;               // End-of-buffer callback function
  /* ... */ void        *EOS;               // End-of-sample callback function
  /* ... */ int32_t      user_data[8];      // Miscellaneous user data
  /* ... */ int32_t      system_data[8];    // Miscellaneous system data
} __attribute__ ((packed));

struct SoundIOParameters
{
   int16_t IO;
   int16_t IRQ;
   int16_t DMA_8_bit;
   int16_t DMA_16_bit;
   int32_t IO_reserved[4];
} __attribute__ ((packed));

struct SoundDriverCallParameters
{
  int16_t ax;
  int16_t bx;
  int16_t cx;
  int16_t dx;
  int16_t si;
  int16_t di;
} __attribute__ ((packed));

bool sound_initialise (void);
void sound_finalise (void);
void sound_update (void);
bool sound_open_music_file (const char *fname);
void sound_play_music (void);
void sound_stop_music (void);
void sound_pause_music (void);

#endif
