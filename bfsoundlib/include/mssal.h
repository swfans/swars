/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file mssal.h
 *     General header recreating MSS API/ABI.
 * @par Purpose:
 *     OpenAL based reimplementation of MSS API.
 * @par Comment:
 *     This only supports ABI for MSS version 3.03 for DOS. If your version or
 *     target OS is a bit different, it could be added to this implementation
 *     by proper ifdefs.
 * @author   Tomasz Lis
 * @date     12 Jun 2022 - 05 Sep 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef AIL2OAL_MSSAL_H_
#define AIL2OAL_MSSAL_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

typedef struct SNDCARD_IO_PARMS SNDCARD_IO_PARMS;
typedef struct AIL_DRIVER AIL_DRIVER;
typedef struct DIG_DRIVER DIG_DRIVER;
typedef struct SNDSAMPLE SNDSAMPLE;
typedef struct VDI_HDR VDI_HDR;
typedef struct VDI_CALL VDI_CALL;
typedef struct DIG_MODE DIG_MODE;
typedef struct DIG_DDT DIG_DDT;
typedef struct DIG_DST DIG_DST;

/** Handle to timer.
 *
 * Originally named `HTIMER`. This less generic name helps to remember this is sound-related.
 */
typedef long HSNDTIMER;

/** Sound card hardware I/O parameters structure.
 *
 * Originally named `IO_PARMS`. This less generic name helps when analyzing old code.
 */
struct SNDCARD_IO_PARMS {
   int16_t IO;
   int16_t IRQ;
   int16_t DMA_8_bit;
   int16_t DMA_16_bit;
   int32_t IO_reserved[4];
};

/** Base driver descriptor.
 */
struct AIL_DRIVER {
  uint32_t seg;                              /**< offs=0x00 Seg:off pointer to driver (off=0) */
  uint32_t sel;                              /**< offs=0x04 Selector for driver (off=0) */
  void *buf;                                 /**< offs=0x08 Protected-mode pointer to driver */
  uint32_t size;                             /**< offs=0x0C Size of driver image */
  VDI_HDR *VHDR;                             /**< offs=0x10 Pointer to driver header (same as buf) */
  int32_t type;                              /**< offs=0x14 AIL3DIG or AIL3MDI (see descriptor field) */
  int32_t initialized;                       /**< offs=0x18 1 if hardware successfully init'd, else 0 */
  int32_t PM_ISR;                            /**< offs=0x1C -1 if no PM ISR hooked, else IRQ # */
  HSNDTIMER server;                          /**< offs=0x20 DRV_SERVE periodic timer, if requested */
  void (*destructor)(void *);                /**< offs=0x24 Vector to high-level destructor, if any */
  void *descriptor;                          /**< offs=0x28 High-level descriptor (DIG_ or MDI_DRIVER) */
};

/** PCM digital sample playback driver.
 */
struct DIG_DRIVER {
  AIL_DRIVER *drvr;                          /**< offs=0x00 Handle to base driver descriptor */
  DIG_DDT *DDT;                              /**< offs=0x04 Protected-mode pointer to DDT */
  DIG_DST *DST;                              /**< offs=0x08 Protected-mode pointer to DST */
  HSNDTIMER timer;                           /**< offs=0x0C Buffer-polling timer */
  int32_t half_buffer_size;                  /**< offs=0x10 Size of DMA half-buffer */
  int32_t DMA_rate;                          /**< offs=0x14 Hardware sample rate */
  int32_t hw_format;                         /**< offs=0x18 DIG_F code in use */
  uint32_t hw_mode_flags;                    /**< offs=0x1C DIG_PCM_ flags for mode in use */
  uint32_t DMA_seg;                          /**< offs=0x20 Seg:off pointer to DMA buffers (off=0) */
  uint32_t DMA_sel;                          /**< offs=0x24 Selector for DMA buffers (off=0) */
  void *DMA_buf;                             /**< offs=0x28 Protected-mode pointer to DMA buffers */
  void *DMA[2];                              /**< offs=0x2C Protected-mode pointers to half-buffers
                                             (note that DMA[0] may != DMA_buf) */
  int16_t *buffer_flag;                      /**< offs=0x34 Protected-mode pointer to buffer flag */
  int32_t last_buffer;                       /**< offs=0x38 Last active buffer flag value in driver */
  int32_t channels_per_sample;               /**< offs=0x3C # of channels per sample (1 or 2) */
  int32_t bytes_per_channel;                 /**< offs=0x40 # of bytes per channel (1 or 2) */
  int32_t channels_per_buffer;               /**< offs=0x44 # of channels per half-buffer */
  int32_t samples_per_buffer;                /**< offs=0x48 # of samples per half-buffer */
  int32_t build_size;                        /**< offs=0x4C # of bytes in build buffer */
  int32_t *build_buffer;                     /**< offs=0x50 Build buffer (4 * n_samples bytes) */
  int32_t playing;                           /**< offs=0x54 Playback active if non-zero */
  int32_t quiet;                             /**< offs=0x58 # of consecutive quiet sample periods */
  SNDSAMPLE *samples;                        /**< offs=0x5C Pointer to list of sound samples */
  int32_t n_samples;                         /**< offs=0x60 # of sound samples */
  int32_t n_active_samples;                  /**< offs=0x64 # of samples being processed */
  int32_t master_volume;                     /**< offs=0x68 Master sample volume 0-127 */
  int32_t system_data[8];                    /**< offs=0x6C Miscellaneous system data */
};

/** Representation of a sound sample.
 *
 * Originally named `_SAMPLE`. This less generic name helps when analyzing old code.
 */
struct SNDSAMPLE {
  DIG_DRIVER *driver;                        /**< offs=0x00 Driver for playback */
  uint32_t status;                           /**< offs=0x04 SMP_ flags: _FREE, _DONE, _PLAYING */
  void *start[2];                            /**< offs=0x08 Sample buffer address (W) */
  uint32_t len[2];                           /**< offs=0x10 Sample buffer size in bytes (W) */
  uint32_t pos[2];                           /**< offs=0x18 Index to next byte (R/W) */
  uint32_t done[2];                          /**< offs=0x20 Nonzero if buffer with len=0 sent by app */
  int32_t current_buffer;                    /**< offs=0x28 Buffer # active (0/1) */
  int32_t last_buffer;                       /**< offs=0x2C Last active buffer (for double-buffering) */
  int32_t loop_count;                        /**< offs=0x30 0-inf, 1-1 */
  int32_t format;                            /**< offs=0x34 DIG_F format (8/16 bits, mono/stereo) */
  uint32_t flags;                            /**< offs=0x38 DIG_PCM_SIGN / DIG_PCM_ORDER (stereo only) */
  int32_t playback_rate;                     /**< offs=0x3C Playback rate in hertz */
  int32_t volume;                            /**< offs=0x40 Sample volume 0-127 */
  int32_t pan;                               /**< offs=0x44 Mono panpot/stereo balance (0=L ... 127=R) */
  int32_t vol_scale[2][256];                 /**< offs=0x48 [left/mono=0,right=1][256] channel volume */
  void *SOB;                                 /**< offs=0x848 Start-of-block callback function */
  void *EOB;                                 /**< offs=0x84C End-of-buffer callback function */
  void *EOS;                                 /**< offs=0x850 End-of-sample callback function */
  int32_t user_data[8];                      /**< offs=0x854 Miscellaneous user data */
  int32_t system_data[8];                    /**< offs=0x874 Miscellaneous system data */
};

/** Standard MSS Vendor Device Interface driver header.
 *
 * This implementation uses OS drivers though OpenAL, so this struct is only used to provide mock driver.
 */
struct VDI_HDR {
   char ID[8];                               /**< offs=0x00 Magic ID string, "AIL3xxx" followed by ^Z */
   uint32_t driver_version;                  /**< offs=0x08 Version of this driver */
   uint32_t common_IO_configurations;        /**< offs=0x0C Offset to common SNDCARD_IO_PARMS configs */
   uint16_t num_IO_configurations;           /**< offs=0x10 Count of common SNDCARD_IO_PARMS provided */
   uint32_t environment_string;              /**< offs=0x12 Offset to a string containing name of env
                                             variable with can be used to store IO params */
   SNDCARD_IO_PARMS IO;                      /**< offs=0x16 Sound hardware I/O parameters */
   int16_t service_rate;                     /**< offs=0x2E Frequency of backgound timer interrupts
                                             for the driver */
   uint16_t busy;                            /**< offs=0x30 If non-zero, the driver is servicing a request
                                             and cannot be interacted with */
   uint16_t driver_num;                      /**< offs=0x32 Driver number */
   uint16_t this_ISR;                        /**< offs=0x34 Offset of INT 66H dispatcher */
   uint32_t prev_ISR;                        /**< offs=0x36 Pointer to previous INT 66H ISR */
   int8_t scratch[128];                      /**< offs=0x3A Shared scratch workspace */
   int8_t dev_name[80];                      /**< offs=0xBA Device name (only in newer versions) */
};

/** Vendor Device Interface register parameters.
 */
struct VDI_CALL {
  int16_t AX;
  int16_t BX;
  int16_t CX;
  int16_t DX;
  int16_t SI;
  int16_t DI;
};

/** Digital playback timings for a specific Mode.
 */
struct DIG_MODE {
   uint16_t minimum_physical_sample_rate;
   uint16_t nominal_physical_sample_rate;
   uint16_t maximum_physical_sample_rate;
   uint16_t minimum_DMA_half_buffer_size;
   uint16_t maximum_DMA_half_buffer_size;
   uint32_t flags;
};

/** Digital Driver Timings gathered on initialization.
 */
struct DIG_DDT {
    int8_t format_supported[16];             /**< offs=0x00 List of formats for which data was gathered */
    DIG_MODE format_data[16];                /**< offs=0x10 Timing bounds for each mode */
};

#pragma pack()

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // AIL2OAL_MSSAL_H_
