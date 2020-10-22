#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stddef.h>

#include "bflib_basics.h"
#include "globals.h"

#pragma pack(1)

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

#pragma pack()

extern char session_name[20];
extern char user_name[50];

extern uint8_t in_network_game;
extern uint8_t cmdln_param_mp;
extern uint8_t cmdln_param_tf;
extern uint8_t cmdln_param_bcg;
extern uint8_t cmdln_param_d;
extern uint8_t unkn01_maskarr[28];
extern uint8_t flags_general_unkn02;
extern uint8_t cmdln_param_n;
extern uint8_t pktrec_mode;
extern uint8_t cmdln_pr_num;
extern uint8_t game_perspective;
extern uint8_t exit_game;
extern uint8_t input_char;

extern uint16_t unkn01_mode;
extern uint16_t selected_map_index;
extern uint32_t flags_general_unkn01;

extern uint16_t cmdln_param_a;
extern uint16_t cmdln_param_w;
extern uint16_t cmdln_param_map_index;
extern uint32_t engine_mem_alloc_size;
extern void *engine_mem_alloc_ptr;
extern BuffUnknStruct02 buffer_allocs[40];

extern long navi2_unkn_counter;
extern long navi2_unkn_counter_max;
extern uint32_t trial_alloc;

extern PlayerInfo players[8];
extern uint8_t local_player_no;

#define STRINGS_MAX 652

extern char *gui_strings_data;
extern char *gui_strings_data_end;
extern char *gui_strings[STRINGS_MAX];

extern struct TbSprite *m_sprites;
extern ulong curr_tick_time;
extern ulong prev_tick_time;
extern GameTurn gameturn;
extern GameTurn prev_gameturn;
extern ulong turns_delta;
extern ushort msecs_per_frame;
extern ushort gamep_unknval_01;
extern uint8_t *vec_tmap;
extern uint8_t unknoise_tmap[2048];

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

#endif
