#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stddef.h>

#include "bflib_basics.h"
#include "globals.h"
#include "scanner.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

// Not sure about the value
#define GAME_FPS 20

enum GameFlags {
    GamF_Unkn1       = 0x0001,
    GamF_Unkn2       = 0x0002,
    GamF_Unkn4       = 0x0004,
    GamF_Unkn8       = 0x0008,
    GamF_Unkn10      = 0x0010,
    GamF_Unkn20      = 0x0020,
    GamF_Unkn40      = 0x0040,
    GamF_Unkn80      = 0x0080,
    GamF_Unkn100     = 0x0100,
    GamF_Unkn200     = 0x0200,
    GamF_Unkn400     = 0x0400,
    GamF_Unkn800     = 0x0800,
    GamF_Unkn1000    = 0x1000,
    GamF_Unkn2000    = 0x2000,
    GamF_Unkn4000    = 0x4000,
    GamF_Unkn8000    = 0x8000,
    GamF_Unkn10000   = 0x010000,
    GamF_Unkn20000   = 0x020000,
    GamF_Unkn40000   = 0x040000,
    GamF_Unkn80000   = 0x080000,
    GamF_Unkn100000  = 0x00100000,
    GamF_Unkn200000  = 0x00200000,
    GamF_Unkn400000  = 0x00400000,
    GamF_Unkn800000  = 0x00800000,
    GamF_Unkn1000000  = 0x1000000,
    GamF_Unkn2000000  = 0x2000000,
    GamF_Unkn4000000  = 0x4000000,
    GamF_Unkn8000000  = 0x8000000,
    GamF_Unkn10000000 = 0x10000000,
};

enum ScreenType {
  SCRT_NONE = 0x0,
  SCRT_MISSION,
  SCRT_2,
  SCRT_3,
  SCRT_CRYO,
  SCRT_EQUIP,
  SCRT_MAINMENU,
  SCRT_NETGAME,
  SCRT_RESEARCH,
  SCRT_9,
  SCRT_PAUSE,
  SCRT_B,
  SCRT_ALERTBOX,
  SCRT_D,
  SCRT_E,
  SCRT_F,
  SCRT_10,
  SCRT_11,
  SCRT_12,
  SCRT_13,
  SCRT_14,
  SCRT_15, // 0x15
  SCRT_99 = 99,
};

struct Thing;

typedef struct {
    char *Name;
    void *BufferPtr;
    int16_t field_8;
    int field_A;
    int field_E;
    char field_12[4];
    char field_16;
} BuffUnknStruct02;

typedef struct {
  char field_0[15];
  char field_F;
} PrimFaceTexture;

typedef struct {
  char field_0[17];
  char field_11;
} Prim4Texture;

typedef struct {
  char field_0[35];
  char field_23;
} PrimObject;

typedef struct {
  char field_0[39];
  char field_27;
} PrimObjectFace4;

typedef struct {
  char field_0[31];
  char field_1F;
} PrimObjectFace;

typedef struct {
  char field_0[9];
  char field_9;
} PrimObjectPoint;

struct StartScreenPoint {
	short X;
	short Y;
};

struct SRect {
	short x;
	short y;
	short w;
	short h;
};

struct DITrig { // sizeof=48
	ulong *vlcbuf[2];
	long vlcid;
	ushort *imgbuf;
	struct SRect rect[2];
	long rectid; // offs=0x20
	struct SRect slice;
	long isdone; // offs=0x2C
};

struct DITriangle {
	short X1;
	short Y1;
	short X2;
	short Y2;
	short X3;
	short Y3;
	ubyte Colour;
};

struct DIBox {
	short X;
	short Y;
	short Width;
	short Height;
	ubyte Colour;
};

struct DILine {
	short X1;
	short Y1;
	short X2;
	short Y2;
	ubyte Colour;
};

struct DISprite { // sizeof=12
	short X;
	short Y;
	struct Sprite *Sprite;
	ubyte Colour;
};

struct DIText { // sizeof=23
	short WindowX;
	short WindowY;
	short Width;
	short Height;
	short X;
	short Y; // offs=0x0A
	char *Text;
	struct Sprite *Font;
	ushort Line;
	ubyte Colour; // offs=0x16
};

struct DIFlic {
	void (*Function)();
};

struct DIHotspot {
	short X;
	short Y;
};

struct PurpleDrawItem { // sizeof=26
	union {
		//struct DITrig Trig; // unused? too large to be here
		struct DITriangle Triangle;
		struct DIBox Box;
		struct DILine Line;
		struct DISprite Sprite;
		struct DIText Text;
		struct DIFlic Flic;
		struct DIHotspot Hotspot;
	} U;
	ubyte Type;
	ushort Flags;
};

struct City { // sizeof=40
    short X;
    short Y;
    ubyte MapID;
    ubyte Level;
    ubyte Flags;
    char Info;
    ushort TextIndex[6];
    long Dummy2[5];
};

struct SynTime {
    ubyte Minute;
    ubyte Hour;
    ubyte Day;
    ubyte Month;
    ubyte Year;
};

#pragma pack()

extern char session_name[20];
extern char user_name[50];

extern uint8_t in_network_game;
extern uint8_t is_single_game;
extern uint8_t cmdln_param_tf;
extern uint8_t cmdln_param_bcg;
extern uint8_t cmdln_param_d;
extern uint8_t unkn01_maskarr[28];

extern ubyte ingame__Cheats;
extern ulong ingame__Credits;
extern ushort ingame__GameMode;
extern ulong ingame__Flags;
extern struct Scanner ingame__Scanner;

extern ubyte login_control__State;

extern uint8_t cmdln_param_n;
extern uint8_t pktrec_mode;
extern uint8_t cmdln_pr_num;
extern uint8_t game_perspective;
extern uint8_t exit_game;
extern uint8_t input_char;

extern uint16_t selected_map_index;
extern uint32_t active_flags_general_unkn01;

extern long unkn01_downcount;
extern long unkn01_pos_x;
extern long unkn01_pos_y;

extern uint16_t cmdln_param_a;
extern uint16_t cmdln_param_w;
extern uint16_t cmdln_param_current_map;
extern uint32_t engine_mem_alloc_size;
extern void *engine_mem_alloc_ptr;
extern BuffUnknStruct02 buffer_allocs[40];

extern long navi2_unkn_counter;
extern long navi2_unkn_counter_max;
extern uint32_t triangulation;

extern uint32_t smack_malloc_used_tot;

#define STRINGS_MAX 652

extern char *gui_strings_data;
extern char *gui_strings_data_end;
extern char *gui_strings[STRINGS_MAX];

extern ulong curr_tick_time;
extern ulong prev_tick_time;
extern GameTurn gameturn;
extern GameTurn prev_gameturn;
extern ulong turns_delta;
extern ushort fifties_per_gameturn;
extern ushort gamep_unknval_01;
extern uint8_t *vec_tmap;
extern ubyte *memload;

extern ubyte background_type;
extern ubyte old_screentype;
extern ubyte screentype;
extern long data_155704;
extern ubyte data_1c498d;
extern char *outro_text_s;
extern char *outro_text_z;
extern long data_197150;
extern long data_1dd91c;
extern ushort overall_scale;
extern ubyte unkn_flags_01;
extern ushort palette_brightness;
extern long outro_unkn01;
extern long outro_unkn02;
extern long outro_unkn03;
extern long people_groups_count;
extern long data_1ddb68;
extern char *people_credits_groups[];
extern ubyte playable_agents;
extern ubyte game_high_resolution;
extern ushort people_frames[22][16];
extern char *mission_briefing_text;
extern char *mem_unkn03;
extern char *weapon_text;
extern struct PurpleDrawItem *purple_draw_list;
extern ubyte *save_game_buffer;
extern ubyte *unkn_buffer_05;

extern ubyte mouser;
extern struct Thing *things;

extern ubyte num_cities;
extern struct City cities[];
extern struct SynTime global_date;

extern ushort mouse_map_x;
extern ushort mouse_map_z;
extern ushort render_area_a;
extern ushort render_area_b;
extern void *scratch_malloc_mem;
extern struct StartScreenPoint *hotspot_buffer;

extern char net_unkn2_text[];

bool game_initialise (void);
void game_handle_sdl_events (void);
void game_update (void);
void game_quit (void);
void game_transform_path (const char *file_name, char *result);
const char *game_get_data_path (void);
const char *game_get_user_path (void);

void read_conf_file(void);
void game_setup(void);
void game_process(void);
void game_reset(void);
void host_reset(void);
void free_texturemaps(void);
int joy_grip_shutdown(void);

#ifdef __cplusplus
};
#endif

#endif
