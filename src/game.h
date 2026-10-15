#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stddef.h>

#include "bffile.h"
#include "bfflic.h"
#include "people.h"
#include "guiboxes.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

enum GameModes {
    GamM_None        = 0,
    GamM_Unkn1       = 1,
    GamM_Unkn2       = 2,
    GamM_Unkn3       = 3,
};

enum UserFlags {
    UsrF_WonBAT       =  0x0001,
    UsrF_Cheats       =  0x0004,
};

enum ScreenType {
  SCRT_NONE = 0x0,
  SCRT_MISSION,
  SCRT_PANET,
  SCRT_WORLDMAP,
  SCRT_CRYO,
  SCRT_EQUIP,
  SCRT_MAINMENU,
  SCRT_SYSMENU,
  SCRT_RESEARCH,
  SCRT_DEBRIEF,
  SCRT_LOGIN,
  SCRT_NETDEBRF,
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

enum LoginControlStates {
    LognCt_Unkn0 = 0,
    LognCt_Unkn1,
    LognCt_Unkn2,
    LognCt_Unkn3,
    LognCt_Unkn4,
    LognCt_NetStarted,
    LognCt_Unkn6,
    LognCt_Unkn7,
    LognCt_Unkn8,
    LognCt_Unkn9,
    LognCt_Unkn10,
};

// For some reason, we have different values for change_screen
//TODO consolidate with ScreenType, use the same values
enum ChangeScreenType {
  ChSCRT_NONE = 0,
  ChSCRT_SYSMENU,
  ChSCRT_PANET,
  ChSCRT_WORLDMAP,
  ChSCRT_CRYO,
  ChSCRT_EQUIP,
  ChSCRT_RESEARCH,
  ChSCRT_MISBRIEF,
};

enum OpenMissionEndStatus {
  OMiSta_NONE,
  /** Mission completed successfully, and ended - no continuation exists */
  OMiSta_EndSuccess,
  /** Mission failed, and ended - no continuation allowed */
  OMiSta_EndFailed,
  /** Mission completed successfully, but is a part of unfinished multi-mission chain - requires continuation */
  OMiSta_ContSuccess,
  /** Mission failed, but non-failable - is left active and can be repeated later */
  OMiSta_ContFailed,
  /** Mission completed successfully, but is a part of multi-mission chain with immediately starting next mission */
  OMiSta_ContImmSuccess,
  /** Final mission completed successfully, campaign finished. */
  OMiSta_CampaignDone,
};

enum MissionFMVPlay {
    MPly_Intro,
    MPly_MissiComplete,
    MPly_MissiFail,
    MPly_MPartComplete,
    MPly_GameOver,
    MPly_Outro,
};

struct Thing;

struct BezierPt { // sizeof=28
    ubyte field_0[26];
    ushort field_2A;
};

struct LevelDef {
  ubyte PlayableGroups[8];
  ubyte field_8[33];
  ubyte field_29;
  ubyte field_2A;
  ubyte field_2B;
};

struct LoginControl { // sizeof=0x0a
    ubyte lcunkn0;
    ubyte State;
    ubyte TechLevel;
    ubyte Faction;
    u32 Money;
    sbyte City;
    ubyte Team;
};

struct UnkPlayerGroup {
    ubyte AgentWeapons[4];
    short AgentOwners[4];
    short AgentMoods[4];
    ubyte GroupActive;
};


#pragma pack()

extern char session_name[20];
extern char user_name[50];

extern ubyte in_network_game;
extern ubyte is_single_game;
extern ubyte cmdln_colour_tables;
extern ubyte cmdln_param_bcg;

//TODO do something with the array - currently only 1st element is in use
extern struct LoginControl login_control[4];

extern ubyte exit_game;

extern long unkn01_downcount;
extern long unkn01_pos_x;
extern long unkn01_pos_y;

extern ushort current_map;
extern short current_level;

extern ubyte *scratch_buf1;

extern ushort word_1531E0;

extern ubyte *spare_map_buffer;

extern ubyte byte_1C6D4A;
extern ushort word_1C8446;

extern ubyte old_screentype;
extern ubyte screentype;
extern char *outro_text_s;
extern char *outro_text_z;
extern s32 data_1dd91c;
extern s32 outro_credits_enabled;
extern s32 dev_credits_group;
extern s32 dev_credits_groups_count;
extern u32 colwavapr_timer;
extern u32 colwavapr_reveal_tm;
extern s32 dword_1DDB6C;

extern char *dev_credits_desc[];
extern char *dev_credits_groups[];

extern sbyte mission_result;

extern ubyte net_player_teams[8];
extern long dword_153194;

extern ubyte group_factions[8];
extern ubyte data_1c4aa2;
extern ubyte start_into_mission;
extern ubyte edit_flag;
extern ubyte change_screen;
extern ubyte current_drawing_mod;
extern ubyte new_current_drawing_mod;

extern struct LevelDef level_def;

/** Index of the local user who controls mouse. */
extern ubyte mouser;
extern long dword_1AA5C4;
extern long dword_1AA5C8;

extern ushort next_mission;

extern int mouse_map_x;
extern int mouse_map_y;
extern int mouse_map_z;

extern char *misc_text[5];

// To be replaced by LbArcTanAngle()
short arctan(int dx, int dz);

bool game_initialise (void);
void game_handle_sdl_events (void);
void game_update (void);
int game_hacky_update(void);
void game_quit (void);

void read_conf_file(void);
TbBool game_setup(void);
void game_process(void);
void game_reset(void);
void host_reset(void);
void global_date_new_game_reset(void);
void init_unkn6_adjustable_variables(void);
void init_agents(void);
void srm_reset_research(void);
void net_new_game_prepare(void);

TbBool player_try_spend_money(long cost);
void campaign_new_game_prepare(void);

void process_sound_heap(void);
void update_danger_music(ubyte a1);

void check_mouse_overvehicle(struct Thing *p_thing, ubyte target_assign);

void draw_new_panel(void);

void update_map_thing_and_traffic_refs(void);
void unkn_lights_processing(void);
void bang_set_detail(int a1);
int sub_73C64(char *a1, ubyte a2);
void draw_mapcoord_line_col2(int a1, int a2, int a3, int a4, int a5, int a6, ubyte a7);

void ingame_palette_reload(void);
void game_set_cam_track_thing_xz(ThingIdx thing);
TbBool game_cam_tracked_thing_is_player_agent(void);

ubyte process_send_person(PlayerIdx plyr, ubyte dmuser);

short test_missions(ubyte flag);
void init_level_3d(ubyte flag);
void restart_back_into_mission(ushort missi);

#ifdef __cplusplus
};
#endif

#endif
