#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stddef.h>

#include "bflib_basics.h"
#include "globals.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

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
  short field_0[1];
  char field_2;
  char field_3;
  short field_4;
  short field_6[12];
  short field_1E[17];
  short field_40[17];
  short field_62;
  char field_64[4];
  int field_68[4];
  int field_78;
  int field_7C[2];
  int field_84;
  int field_88;
  int field_8C;
  int field_90;
  int field_94;
  int field_98;
  int field_9C;
  int field_A0;
  int field_A4;
  char field_A8[3];
  char field_AB;
  char field_AC;
  char field_AD[3];
  char field_B0;
  char field_B1[3];
  char field_B4[7];
  char field_BB;
  char field_BC[2];
  char field_BE[8];
  char field_C6;
  char field_C7;
  char field_C8;
  char field_C9;
  char field_CA[2];
  int field_CC;
  int field_D0;
  int field_D4;
  int field_D8;
  int field_DC;
  short field_E0;
  short field_E2;
  short field_E4;
  int field_E6[2];
  int field_EE;
  int field_F2;
  int field_F6;
  char field_FA[4];
  int field_FE;
  int field_102;
  char field_106[3];
  char field_109;
  char field_10A[3];
  char field_10D;
  char field_10E[3];
  char field_111;
  char field_112[3];
  char field_115;
  char field_116[4];
  char field_11A;
  char field_11B[125];
  short field_198;
  short field_19A;
  char field_19C[4];
  short field_1A0;
  char field_1A2[4];
  char field_1A6[4];
} PlayerInfo;

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

#pragma pack()

extern char session_name[20];
extern char user_name[50];

extern uint8_t in_network_game;
extern uint8_t is_single_game;
extern uint8_t cmdln_param_tf;
extern uint8_t cmdln_param_bcg;
extern uint8_t cmdln_param_d;
extern uint8_t unkn01_maskarr[28];
extern uint8_t ingame__Cheats;
extern uint8_t cmdln_param_n;
extern uint8_t pktrec_mode;
extern uint8_t cmdln_pr_num;
extern uint8_t game_perspective;
extern uint8_t exit_game;
extern uint8_t input_char;

extern uint16_t ingame__GameMode;
extern uint16_t selected_map_index;
extern uint32_t ingame__Flags;
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

extern PlayerInfo players[8];
extern uint8_t local_player_no;

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
extern uint8_t unknoise_tmap[2048];

extern ubyte background_type;

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
