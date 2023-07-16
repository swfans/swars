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
#include <strings.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

/** Max # of notes "on" */
#define AIL_MAX_NOTES 32
/** # of possible MIDI channels */
#define AIL_NUM_CHANS 16
/** # of nested XMIDI FOR loops */
#define AIL_FOR_NEST   4
/** Min channel available for locking */
#define AIL_MIN_LOCK_CHAN ( 1-1)
/** Max channel available for locking */
#define AIL_MAX_LOCK_CHAN (16-1)
/** Percussion channel (no locking) */
#define AIL_PERCUSS_CHAN (10-1)

typedef struct SNDCARD_IO_PARMS SNDCARD_IO_PARMS;
typedef struct AIL_INI AIL_INI;
typedef struct AIL_DRIVER AIL_DRIVER;
typedef struct DIG_DRIVER DIG_DRIVER;
typedef struct MDI_DRIVER MDI_DRIVER;
typedef struct SNDSAMPLE SNDSAMPLE;
typedef struct AILSOUNDINFO AILSOUNDINFO;
typedef struct SNDSEQUENCE SNDSEQUENCE;
typedef struct CTRL_LOG CTRL_LOG;
typedef struct VDI_HDR VDI_HDR;
typedef struct VDI_CALL VDI_CALL;
typedef struct DIG_MODE DIG_MODE;
typedef struct DIG_DDT DIG_DDT;
typedef struct DIG_DST DIG_DST;
typedef struct MDI_DDT MDI_DDT;
typedef struct MDI_DST MDI_DST;
typedef struct MIDIOUT MIDIOUT;

/** There are two types of AIL drivers.
 */
enum AILDrvrTypes {
    AIL3DIG      = 0,          /**< .DIG driver */
    AIL3MDI      = 1,          /**< .MDI driver */
};

/** MIDI driver types.
 */
enum MDIDrvrTypes {
    MDIDRVRTYPE_GM   = 0,          /**< General MIDI driver (Roland-compatible) */
    MDIDRVRTYPE_FM_2 = 1,          /**< 2-operator FM MIDI driver (OPL2) */
    MDIDRVRTYPE_FM_4 = 2,          /**< 4-operator FM MIDI driver (OPL3) */
    MDIDRVRTYPE_SPKR = 3,          /**< Tandy or PC speaker "beep" driver */
};

/** .INI installation result codes.
 */
enum AILINIResult {
    AIL_INIT_SUCCESS = 0,          /**< Driver installed successfully */
    AIL_NO_INI_FILE  = 1,          /**< No MDI.INI or DIG.INI file exists */
    AIL_INIT_FAILURE = 2,          /**< Driver could not be initialized */
};

/** Errors for file operations, ie. `disk_err` variable.
 */
enum AILFileErrors {
    AIL_NO_ERROR        = 0,
    AIL_IO_ERROR        = 1,
    AIL_OUT_OF_MEMORY   = 2,
    AIL_FILE_NOT_FOUND  = 3,
    AIL_CANT_WRITE_FILE = 4,
    AIL_CANT_READ_FILE  = 5,
    AIL_DISK_FULL       = 6,
};

/** Function numbers for DIG and XMIDI driver calls.
 */
enum DIGMDIDriverFunctions {
    // Functions common to .MDI and .DIG drivers
    DRV_INIT          = 0x300,
    DRV_GET_INFO      = 0x301,
    DRV_SERVE         = 0x302,
    DRV_PARSE_ENV     = 0x303,
    DRV_VERIFY_IO     = 0x304,
    DRV_INIT_DEV      = 0x305,
    DRV_SHUTDOWN_DEV  = 0x306,
    // .DIG driver functions
    DIG_HW_VOLUME     = 0x400,
    DIG_START_P_CMD   = 0x401,
    DIG_STOP_P_REQ    = 0x402,
    DIG_START_R_CMD   = 0x403,
    DIG_STOP_R_REQ    = 0x404,
    DIG_VSE           = 0x405,
    // .MDI driver functions
    MDI_HW_VOLUME     = 0x500,
    MDI_INIT_INS_MGR  = 0x501,
    MDI_MIDI_XMIT     = 0x502,
    MDI_INSTALL_T_SET = 0x503,
    MDI_GET_T_STATUS  = 0x504,
    MDI_PROT_UNPROT_T = 0x505,
    MDI_VSE           = 0x506,
};

/** Parameters for DRV_VERIFY_IO function driver calls.
 */
enum DIGMDIDriverVerifyIoParams {
    DIG_DETECT_8_BIT_ONLY    = 0x01,
    DIG_DETECT_16_BIT_ONLY   = 0x02,
    DIG_DETECT_8_AND_16_BITS = 0x03,
};

/** Flags for DIG_DRIVER.hw_mode_flags.
 */
enum DIGDriverHwModeFlags {
    DIG_PCM_SIGN       = 0x0001,
    DIG_PCM_ORDER      = 0x0002,
    DIG_PCM_POLARITY   = 0x0004,
    DIG_PCM_SPLIT      = 0x0008,
    DIG_BUFFER_SERVICE = 0x0010,
};

/** AIL internal preference names.
 */
enum AILPreferenceNames {
    DIG_SERVICE_RATE         =  0, /**< DMA buffer-polling rate */
    DIG_HARDWARE_SAMPLE_RATE =  1, /**< Hardware sample rate, either amount of samples per sec or value from
                                   AILPreferenceSelection */
    DIG_DMA_RESERVE          =  2, /**< Real-mode mem reserve for DMA */
    DIG_LATENCY              =  3, /**< Half-buffer size in ms */
    DIG_MIXER_CHANNELS        = 4, /**< Allocatable SNDSAMPLE structures */
    DIG_DEFAULT_VOLUME        = 5, /**< Default sample volume (0-127) */
    DIG_RESAMPLING_TOLERANCE  = 6, /**< Resampling trigger */
    DIG_USE_STEREO           =  7, /**< Use mono output only */
    DIG_USE_16_BITS          =  8, /**< Sample accuracy */
    DIG_ALLOW_16_BIT_DMA     =  9, /**< OK to use 16-bit DMA if necessary */
    DIG_SS_LOCK              = 10, /**< Don't disable IRQs while mixing */
    MDI_SERVICE_RATE         = 11, /**< XMIDI sequencer timing */
    MDI_SEQUENCES            = 12, /**< Sequence handles per driver */
    MDI_DEFAULT_VOLUME       = 13, /**< Default sequence volume (0-127) */
    MDI_QUANT_ADVANCE        = 14, /**< Beat/bar count +1 interval */
    MDI_ALLOW_LOOP_BRANCHING = 15, /**< Branches cancel XMIDI FOR loops */
    MDI_DEFAULT_BEND_RANGE   = 16, /**< Default pitch-bend range */
    AIL_SCAN_FOR_HARDWARE    = 17, /**< Scan for I/O settings if necessary */
    AIL_ALLOW_VDM_EXECUTION  = 18, /**< Allow Windows "DOS box" execution */
    //MDI_DOUBLE_NOTE_OFF      =  x, /**< For stuck notes on SB daughterboards; not defined in this MSS version */
    //DIG_ENABLE_RESAMPLE_FILTER= x, /**< Enable resampling filter; not defined in this MSS version  */
    //DIG_DECODE_BUFFER_SIZE   =  x, /**< Decode buffer size by default; not defined in this MSS version  */
};

/** Value selection for DIG_HARDWARE_SAMPLE_RATE preference.
 */
enum AILPreferenceSelection {
    AILPREF_MIN_VAL = 0,
    AILPREF_NOM_VAL = 1,
    AILPREF_MAX_VAL = 2,
};

/** SNDSEQUENCE.status and SNDSAMPLE.status flag values.
 */
enum SndSequenceStatusFlags {
    SNDSEQ_FREE          = 0x0001, /**< Sequence/sample is available for allocation */
    SNDSEQ_DONE          = 0x0002, /**< Sequence/sample has finished playing, or has
                                   never been started */
    SNDSEQ_PLAYING       = 0x0004, /**< Sequence/sample is playing */
    SNDSEQ_STOPPED       = 0x0008, /**< Sequence/sample has been stopped */
    SNDSEQ_PLAYINGRELEASED=0x0010, /**< Sequence/sample is playing, but MIDI/DIG handle
                                   has been temporarily released */
};

#define SNDSMP_FREE		SNDSEQ_FREE
#define SNDSMP_DONE		SNDSEQ_DONE
#define SNDSMP_PLAYING	SNDSEQ_PLAYING
#define SNDSMP_STOPPED	SNDSEQ_STOPPED
#define SNDSMP_PLAYINGRELEASED	SNDSEQ_PLAYINGRELEASED

/** Timer status values.
 */
enum AILTimerStatus {
    AILT_FREE            = 0, /**< Timer handle is free for allocation */
    AILT_STOPPED         = 1, /**< Timer is stopped */
    AILT_RUNNING         = 2, /**< Timer is running */
};

/** Non-specific XMIDI/MIDI event types.
 */
enum AILMIDIEvents {
    MDI_EV_NOTE_OFF      = 0x80,
    MDI_EV_NOTE_ON       = 0x90,
    MDI_EV_POLY_PRESS    = 0xa0,
    MDI_EV_CONTROL       = 0xb0,
    MDI_EV_PROGRAM       = 0xc0,
    MDI_EV_CHAN_PRESS    = 0xd0,
    MDI_EV_PITCH         = 0xe0,
    MDI_EV_SYSEX         = 0xf0,
    MDI_EV_ESC           = 0xf7,
    MDI_EV_META          = 0xff,
};

/** Sub-event types of the XMIDI/MIDI META event.
 */
enum AILMIDIMetaEvents {
    MDI_META_EOT        = 0x2f,
    MDI_META_TEMPO      = 0x51,
    MDI_META_TIME_SIG   = 0x58,
};

/** MIDI controllers and Channel Mode messages.
 * Thedse are recognized by AIL drivers when sent with
 * MDI_EV_CONTROL events.
 */
enum AILMIDIControlMsgs {
    MDI_CTR_MODULATION       = 1,
    MDI_CTR_PB_RANGE         = 6, // later reused for DATA_MSB
    MDI_CTR_PART_VOLUME      = 7,
    MDI_CTR_PANPOT           = 10,
    MDI_CTR_EXPRESSION       = 11,
    MDI_CTR_DATA_LSB         = 38, // not set everywhere - unsure why
    MDI_CTR_SUSTAIN          = 64,
    MDI_CTR_REVERB           = 91,
    MDI_CTR_CHORUS           = 93,
    MDI_CTR_RPN_LSB          = 100,
    MDI_CTR_RPN_MSB          = 101,
    MDI_CTR_CHAN_MUTE        = 107,
    MDI_CTR_CHAN_PROTECT     = 111,
    MDI_CTR_VOICE_PROTECT    = 112,
    MDI_CTR_PATCH_BANK_SEL   = 114,
    MDI_CTR_RESET_ALL_CTRLS  = 121,
    MDI_CTR_ALL_NOTES_OFF    = 123,
};

/** Digital pipeline stages.
 *
 * These are the points at which external modules may be installed into
 * a given SNDSAMPLE or DIG_DRIVER's processing pipeline
 */
enum SNDSAMPLESTAGE {
   SNDSMST_DP_ASI_DECODER=0, /**< Must be "ASI codec stream" provider */
   SNDSMST_DP_FILTER,        /**< Must be "MSS pipeline filter" provider */
   SNDSMST_DP_MERGE,         /**< Must be "MSS mixer" provider */
   SNDSMST_N_SAMPLE_STAGES,  /**< Placeholder for end of list (= # of valid stages) */
   SNDSMST_SAMPLE_ALL_STAGES, /**< Used to signify all pipeline stages, for shutdown */
};

/** Digital sound saple PCM data format flags.
 */
enum SndDigSampleFormatFlags {
    DIG_F_16BITS_MASK          = 0x01,
    DIG_F_STEREO_MASK          = 0x02,
    DIG_F_ADPCM_MASK           = 0x04,
    DIG_F_XBOX_ADPCM_MASK      = 0x08,
    DIG_F_MULTICHANNEL_MASK    = 0x10,
    DIG_F_OUTPUT_FILTER_IN_USE = 0x20,
};

#define DIG_F_MONO_8           0
#define DIG_F_MONO_16          (DIG_F_16BITS_MASK)
#define DIG_F_STEREO_8         (DIG_F_STEREO_MASK)
#define DIG_F_MULTICHANNEL_8   (DIG_F_MULTICHANNEL_MASK)
#define DIG_F_STEREO_16        (DIG_F_STEREO_MASK|DIG_F_16BITS_MASK)
#define DIG_F_MULTICHANNEL_16  (DIG_F_MULTICHANNEL_MASK|DIG_F_16BITS_MASK)

/** Handle to timer.
 *
 * Originally named `HTIMER`. This less generic name helps to remember this is sound-related.
 */
typedef long HSNDTIMER;

/** Handle to interface provider.
 */
typedef uint32_t HAILPROVIDER;


/** Timer trigger callback function type.
 */
typedef void (*AILTIMERCB) (void *user_data);

/** Sequence related callback function type.
 */
typedef void (*AILSEQUENCECB) (SNDSEQUENCE *seq);

/** Generic sample-related callback function type.
 */
typedef void (*AILSAMPLECB) (SNDSAMPLE *s);

/** Generic sequence-related callback function type.
 */
typedef void (*AILSEQUENCECB) (SNDSEQUENCE *seq);

/** XMIDI beat/bar callback function type.
 */
typedef void (*AILBEATCB) (MDI_DRIVER *, SNDSEQUENCE *, int32_t, int32_t);

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

/** Initialization file structure.
 */
struct AIL_INI {
    char device_name[128];                   /**< offs=0x000 Device name */
    char driver_name[128];                   /**< offs=0x080 Driver filename */
    SNDCARD_IO_PARMS IO;                     /**< offs=0x100 I/O parameters for driver */
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
  HSNDTIMER timer;                           /**< offs=0x0C Buffer-polling timer; also called backgroundtimer */
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
  int32_t channels_per_buffer;               /**< offs=0x44 # of channels per half-buffer
                                             (channels_per_sample * samples_per_buffer) */
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

/** MIDI output driver structure.
 */
struct MIDIOUT {
    uint8_t a1;
};

/* XMIDI playback driver.
 * sizeof=468
 */
struct MDI_DRIVER {
    AIL_DRIVER *drvr;                        /**< offs=0   Handle to base driver descriptor */
    MDI_DDT *DDT;                            /**< offs=4   Protected-mode pointer to DDT; specific to DOS implementation  */
    MDI_DST *DST;                            /**< offs=8   Protected-mode pointer to DST; specific to DOS implementation  */
    HSNDTIMER timer;                         /**< offs=12  XMIDI quantization timer */
    int32_t interval_time;                   /**< offs=16  XMIDI quantization timer interval in uS */
    int32_t disable;                         /**< offs=20  Set > 0 to disable XMIDI service */
    SNDSEQUENCE *sequences;                  /**< offs=24  Pointer to list of SNDSEQUENCEs */
    int32_t n_sequences;                     /**< offs=28  # of SNDSEQUENCEs */
    int32_t lock[16];                        /**< offs=32  1 if locked, 2 if protected, else 0 */
    SNDSEQUENCE *locker[16];                 /**< offs=96  Ref to SNDSEQUENCE which locked channel */
    SNDSEQUENCE *owner[16];                  /**< offs=160 Ref to SNDSEQUENCE which owned locked channel */
    SNDSEQUENCE *user[16];                   /**< offs=224 Ref to last SNDSEQUENCE to use channel */
    int32_t state[16];                       /**< offs=288 Lock state prior to being locked */
    int32_t notes[16];                       /**< offs=352 # of active notes in channel */
    void *event_trap;                        /**< offs=416 MIDI event trap callback function */
    void *timbre_trap;                       /**< offs=420 Timbre request callback function */
    int32_t message_count;                   /**< offs=424 MIDI message count; specific to DOS implementation */
    int32_t offset;                          /**< offs=428 MIDI buffer offset; specific to DOS implementation  */
    int32_t master_volume;                   /**< offs=432 Master XMIDI note volume 0-127 */
    union {
      struct {
        int32_t system_data[8];              /**< offs=436 Miscellaneous system data */
      } dos;
      struct {
        void *mhdr;                          /**< SysEx header */
        MDI_DRIVER *next;                    /**< Pointer to next HMDIDRIVER in use */
        int32_t callingCT;                   /**< Calling EXE's task number (16 bit only) */
        int32_t callingDS;                   /**< Calling EXE's DS (used in 16 bit only) */
        MIDIOUT *hMidiOut;                   /**< MIDI output driver */
      } win;
    };
};

/** Representation of a sound sample.
 *
 * Originally named `_SAMPLE`. This less generic name helps when analyzing old code.
 */
struct SNDSAMPLE {
  DIG_DRIVER *driver;                        /**< offs=0x00 Driver for playback */
  uint32_t status;                           /**< offs=0x04 SNDSMP_ flags: _FREE, _DONE, _PLAYING */
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
  AILSAMPLECB SOB;                           /**< offs=0x848 Start-of-block callback function */
  AILSAMPLECB EOB;                           /**< offs=0x84C End-of-buffer callback function */
  AILSAMPLECB EOS;                           /**< offs=0x850 End-of-sample callback function */
  int32_t user_data[8];                      /**< offs=0x854 Miscellaneous user data */
  int32_t system_data[8];                    /**< offs=0x874 Miscellaneous system data */
};

struct AILSOUNDINFO {
    int32_t format;
    const void *data_ptr;
    uint32_t data_len;
    uint32_t rate;
    int32_t bits;
    int32_t channels;
    uint32_t samples;
    uint32_t block_size;
    const void *initial_ptr;
};

/** MIDI status log structure.
 *
 * sizeof=1152
 */
struct CTRL_LOG {
    int32_t program[16];                     /**< offset=0   */
    int32_t pitch_l[16];                     /**< offset=64  */
    int32_t pitch_h[16];                     /**< offset=128 */
    int32_t c_lock[16];                      /**< offset=192 */
    int32_t c_prot[16];                      /**< offset=256 */
    int32_t c_mute[16];                      /**< offset=320 */
    int32_t c_v_prot[16];                    /**< offset=384 */
    int32_t bank[16];                        /**< offset=448 */
    int32_t indirect[16];                    /**< offset=512 */
    int32_t callback[16];                    /**< offset=576 */
    int32_t mod[16];                         /**< offset=640 */
    int32_t vol[16];                         /**< offset=704 */
    int32_t pan[16];                         /**< offset=768 */
    int32_t exp[16];                         /**< offset=832 */
    int32_t sus[16];                         /**< offset=896 */
    int32_t reverb[16];                      /**< offset=960 */
    int32_t chorus[16];                      /**< offset=1024 */
    int32_t bend_range[16];                  /**< offset=1088 */
};

/** Representation of a MIDI sample sequence.
 *
 * Originally named `_SEQUENCE`. This less generic name helps when analyzing old code.
 * sizeof=1816
 */
struct SNDSEQUENCE {
    MDI_DRIVER *driver;                      /**< offset=0   */
    uint32_t status;                         /**< offset=4   */
    void *TIMB;                              /**< offset=8   */
    void *RBRN;                              /**< offset=12  */
    void *EVNT;                              /**< offset=16  */
    uint8_t *EVNT_ptr;                       /**< offset=20  Current event pointer */
    uint8_t *ICA;                            /**< offset=24  */
    void *prefix_callback;                   /**< offset=28  */
    void *trigger_callback;                  /**< offset=32  */
    AILBEATCB beat_callback;                 /**< offset=36  XMIDI beat/bar change handler */
    AILSEQUENCECB EOS;                       /**< offset=40  End-of-sequence callback function */
    int32_t loop_count;                      /**< offset=44  */
    int32_t interval_count;                  /**< offset=48  */
    int32_t interval_num;                    /**< offset=52  */
    int32_t volume;                          /**< offset=56  */
    int32_t volume_target;                   /**< offset=60  */
    int32_t volume_accum;                    /**< offset=64  */
    int32_t volume_period;                   /**< offset=68  */
    int32_t tempo_percent;                   /**< offset=72  */
    int32_t tempo_target;                    /**< offset=76  */
    int32_t tempo_accum;                     /**< offset=80  */
    int32_t tempo_period;                    /**< offset=84  */
    int32_t tempo_error;                     /**< offset=88  */
    int32_t beat_count;                      /**< offset=92  */
    int32_t measure_count;                   /**< offset=96  */
    int32_t time_numerator;                  /**< offset=100 */
    int32_t time_fraction;                   /**< offset=104 */
    int32_t beat_fraction;                   /**< offset=108 */
    int32_t time_per_beat;                   /**< offset=112 */
    void *FOR_ptrs[4];                       /**< offset=116 */
    int32_t FOR_loop_count[4];               /**< offset=132 */
    int32_t chan_map[16];                    /**< offset=148 */
    CTRL_LOG shadow;                         /**< offset=212 */
    int32_t note_count;                      /**< offset=1364 */
    int32_t note_chan[32];                   /**< offset=1368 */
    int32_t note_num[32];                    /**< offset=1496 */
    int32_t note_time[32];                   /**< offset=1624 */
    int32_t user_data[8];                    /**< offset=1752 */
    int32_t system_data[8];                  /**< offset=1784 */
};

/** Standard MSS Vendor Device Interface driver header.
 *
 * This implementation uses OS drivers though OpenAL, so this struct is only used to provide mock driver.
 */
struct VDI_HDR {
   char ID[8];                               /**< offs=0x00 Magic ID string, "AIL3xxx" followed by ^Z */
   uint32_t driver_version;                  /**< offs=0x08 Version of this driver */
   uintptr_t common_IO_configurations;       /**< offs=0x0C Offset to common SNDCARD_IO_PARMS configs */
   uint16_t num_IO_configurations;           /**< offs=0x10 Count of common SNDCARD_IO_PARMS provided */
   uintptr_t environment_string;             /**< offs=0x12 Offset to a string containing name of env
                                             variable with can be used to store IO params */
   SNDCARD_IO_PARMS IO;                      /**< offs=0x16 Sound hardware I/O parameters */
   int16_t service_rate;                     /**< offs=0x2E Frequency of backgound timer interrupts
                                             for the driver */
   uint16_t busy;                            /**< offs=0x30 If non-zero, the driver is servicing a request
                                             and cannot be interacted with */
   uint16_t driver_num;                      /**< offs=0x32 Driver number */
   uint16_t this_ISR;                        /**< offs=0x34 Offset of INT 66H dispatcher */
   uintptr_t prev_ISR;                       /**< offs=0x36 Pointer to previous INT 66H ISR */
   int8_t scratch[128];                      /**< offs=0x3A Shared scratch workspace */
   char dev_name[80];                        /**< offs=0xBA Device name (only in newer versions) */
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

struct DIG_DST {
   void *DMA_buffer_A;
   void *DMA_buffer_B;
   int16_t active_buffer;
};

struct MDI_DST {
   char library_directory[128];
   char GTL_filename[128];
   int8_t MIDI_data[512];
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

/** MIDI Driver Timings gathered on initialization.
 */
struct MDI_DDT {
    void *library_environment;               /**< offset=0  Name of environment variable for the Instrument Manager */
    void *GTL_suffix;                        /**< offset=4  Global Timbre Library suffix */
    uint16_t num_voices;                     /**< offset=8  # of parallel voices */
    uint16_t max_melodic_channel;            /**< offset=10 Melodic channel max */
    uint16_t min_melodic_channel;            /**< offset=12 Melodic channel min */
    uint16_t percussion_channel;             /**< offset=14 Percussion channel */
};

#pragma pack()

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // AIL2OAL_MSSAL_H_
