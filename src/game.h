#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stddef.h>

#include "bffile.h"
#include "bfflic.h"
#include "globals.h"
#include "scanner.h"
#include "people.h"
#include "guiboxes.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

#define WEP_ROCKETS_FIRED_LIMIT 15

enum GameModes {
    GamM_None        = 0,
    GamM_Unkn1       = 1,
    GamM_Unkn2       = 2,
    GamM_Unkn3       = 3,
};

enum GameFlags {
    GamF_BillboardMovies = 0x0001,
    GamF_AdvLights    = 0x0002,
    GamF_Unkn0004     = 0x0004,
    GamF_Unkn0008     = 0x0008,
    GamF_MortalGame   = 0x0010,
    GamF_Unkn0020     = 0x0020,
    GamF_RenderScene  = 0x0040,
    GamF_StopThings   = 0x0080, /**< Do not process things, stopping ingame action. */
    GamF_Unkn0100     = 0x0100,
    GamF_Unkn0200     = 0x0200,
    GamF_DeepRadar    = 0x0400,
    GamF_Unkn0800     = 0x0800,
    GamF_Unkn1000     = 0x1000,
    GamF_HUDPanel     = 0x2000,
    GamF_Unkn4000     = 0x4000,
    GamF_ThermalView  = 0x8000,
    GamF_Unkn00010000 = 0x00010000,
    GamF_Unkn00020000 = 0x00020000,
    GamF_Unkn00040000 = 0x00040000,
    GamF_SkipIntro    = 0x00080000,
    GamF_Unkn00100000 = 0x00100000,
    GamF_NoScannerBeep = 0x00200000,
    GamF_Unkn00400000 = 0x00400000,
    GamF_Unkn00800000 = 0x00800000,
    GamF_Unkn01000000 = 0x01000000,
    GamF_Unkn02000000 = 0x02000000,
    GamF_Unkn04000000 = 0x04000000,
    GamF_Unkn08000000 = 0x08000000,
    GamF_Unkn10000000 = 0x10000000,
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

enum DisplayModes {
  DpM_NONE = 0x0,
  DpM_UNKN_1 = 0x1,
  DpM_2 = 0x2,
  DpM_31 = 0x31,
  DpM_ENGINEPLY = 0x32,
  DpM_UNKN_33 = 0x33,
  DpM_34 = 0x34,
  DpM_36 = 0x36,
  DpM_PURPLEMNU = 0x37,
  DpM_38 = 0x38,
  DpM_39 = 0x39,
  DpM_UNKN_3A = 0x3A,
  DpM_UNKN_3B = 0x3B,
};

enum AnimSlot {
  AniSl_FULLSCREEN = 0,
  AniSl_BILLBOARD = 1,
  AniSl_EQVIEW = 2,	/**< equipment (weapon or mod) presentation in buy/sell window */
  AniSl_CYBORG_INOUT = 3,	/**< cyborg mod insertion or removal anim */
  AniSl_UNKN4 = 4,
  AniSl_UNKN5 = 5,
  AniSl_UNKN6 = 6,
  AniSl_UNKN7 = 7,
  AniSl_CYBORG_BRTH = 8,
  AniSl_NETSCAN = 9,
  AniSl_SCRATCH = 10,	/**< scratch buffer for some transparent menu animations */
};

enum PacketRecordMode {
  PktR_NONE = 0x0,
  PktR_RECORD = 0x1,
  PktR_PLAYBACK = 0x2,
};

enum AppBarIcons {
  ApBar_SYSTEM = 0,
  ApBar_PANET,
  ApBar_WORLDMAP,
  ApBar_CRYO,
  ApBar_EQUIP,
  ApBar_RESEARCH,
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
  ChSCRT_MISSION,
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

struct ColColumn { // sizeof=16
    uint QBits[4];
};

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

struct InGame {
    short GameMode;
    short DisplayMode;
    short fld_unk7D8;
    ubyte fld_unk7DA;
    ubyte CDTrack;
    ubyte DangerTrack;
    ubyte UseMultiMedia;
    ubyte AutoResearch;
    ubyte GameOver;
    struct Scanner Scanner; // offset=0x0C
    long Credits; // offset=0x473 (on original Scanner struct)
    ulong fld_unkC4B;	// offset=0x477
    short fld_unkC4F;
    short MissionStatus;
    long Flags;
    ushort fld_unkC57;
    short fld_unkC59;
    short draw_unknprop_01;
    short Rocket1[WEP_ROCKETS_FIRED_LIMIT];	// offset=0x489
    short NextRocket;
    short TrainMode;
    short MyGroup;
    short CurrentMission;
    ubyte fld_unk4AF;
    ubyte DetailLevel;
    short UserZoom;
    short cmdln_param_a;
    short LowerMemoryUse;
    short fld_unkC8B;
    short fld_unkC8D[2];
    long fld_unkC91;
    ushort TrackX;
    ushort TrackZ;
    short TrackThing;
    ubyte fld_unkC98[5];
    short fld_unkCA0;
    ubyte fld_unkCA2;
    sbyte PanelPermutation;
    ubyte TrenchcoatPreference;
    ubyte MissionNo;
    short fld_unkCA6;	// offset=0x4D2
    short fld_unkCA8;
    ubyte fld_unkCAA;
    ubyte PalType;
    short FlameCount;
    ubyte LastTmap;
    short SoundThing;
    ubyte fld_unkCB1;
    ubyte fld_unkCB2;
    short VisibleBillboardThing;
    short fld_unkCB5;
    short fld_unkCB7;
    /** Flags representing active human players, in both net and local games. */
    ubyte InNetGame_UNSURE;
    ubyte fld_unkCBA[5];
    ushort UserFlags;
    long CashAtStart;
    long Expenditure;
};

#pragma pack()

extern char session_name[20];
extern char user_name[50];
extern char unkn2_names[8][16];

extern ubyte in_network_game;
extern ubyte is_single_game;
extern ubyte cmdln_colour_tables;
extern ubyte cmdln_param_bcg;
extern ubyte keyboard_mode_direct;
extern ubyte unkn01_maskarr[28];
extern long map_editor;

extern struct InGame ingame;

extern ubyte login_control__State;
extern ulong login_control__Money;
extern sbyte login_control__City;
extern ubyte login_control__TechLevel;
extern ubyte byte_181183;
extern ubyte byte_181189;

extern ubyte cmdln_param_n;
extern ubyte pktrec_mode;
extern ushort packet_rec_no;
extern ubyte exit_game;
extern ubyte input_char;

extern ulong active_flags_general_unkn01;

extern long unkn01_downcount;
extern long unkn01_pos_x;
extern long unkn01_pos_y;

extern ushort current_map;
extern short current_level;

extern ulong engine_mem_alloc_size;
extern void *engine_mem_alloc_ptr;

extern long navi2_unkn_counter;
extern long navi2_unkn_counter_max;

extern ubyte anim_slots[];
extern struct Animation animations[2];

extern ubyte *scratch_buf1;

extern void *dword_1810D1;
extern ulong dword_1810D5;

extern ushort game_level_unique_id;
extern ubyte game_level_unkn1[40];
extern ubyte game_level_unkn2[40];

extern ushort word_1531E0;

extern struct ColColumn *game_col_columns;
extern ushort next_col_column;
extern struct SingleObjectFace3 *game_special_object_faces;
extern struct SingleObjectFace4 *game_special_object_faces4;
extern ubyte *game_user_heap;
extern struct UnknBezEdit *bezier_pts;
extern ushort next_bezier_pt;
extern ubyte *spare_map_buffer;
extern long target_old_frameno;
extern ushort word_176E38;

extern ubyte byte_1C4A7C;
extern ubyte linear_vec_pal[256];
extern ulong nsta_size;

extern short *dword_1C529C[6];
extern short *landmap_2B4;

extern ubyte *memload;
#define memload_len 16384

extern ubyte byte_1C83D1;
extern ubyte net_host_player_no;
extern ubyte byte_1C6D4A;
extern ubyte byte_1C6DDC[5];
extern ushort word_1C8446;
extern ushort unkn3de_len;
extern ubyte byte_19EC6F;

extern ushort weapon_text_index[32];
extern ushort cybmod_text_index[16];
extern ubyte background_type;
extern ubyte old_screentype;
extern ubyte screentype;
extern long data_155704;
extern ubyte data_1c498d;
extern ubyte mouse_sprite_anim_frame;
extern char *outro_text_s;
extern char *outro_text_z;
extern long data_197150;
extern long data_1dd91c;
extern ubyte unkn_flags_01;
extern ushort palette_brightness;
extern long outro_credits_enabled;
extern long outro_unkn02;
extern long outro_unkn03;
extern long people_groups_count;
extern long data_1ddb68;
extern ubyte byte_1DDC40;

extern ubyte debug_hud_collision;

extern void *dword_177750;
extern void *unkn_mech_arr7;

extern long mech_unkn_dw_1DC880;
extern long mech_unkn_dw_1DC884;
extern long mech_unkn_dw_1DC888;
extern long mech_unkn_dw_1DC88C;
extern long mech_unkn_dw_1DC890;
extern long mech_unkn_dw_1DC894;

extern char *people_credits_desc[];
extern char *people_credits_groups[];

extern ubyte playable_agents;

extern char *mission_briefing_text;
#define mission_briefing_text_len 16384

extern sbyte mission_result;

extern char *weapon_text;
#define weapon_text_len 32768

extern ubyte scientists_lost;
extern ulong new_mods_researched;
extern ulong new_weapons_researched;
extern ubyte redraw_screen_flag;

extern sbyte byte_15516C;
extern sbyte byte_15516D;
extern ubyte byte_1C5C28[8];
extern ubyte byte_1C6D48;
extern ubyte unkn_flags_08;
extern long dword_153194;
extern ulong starting_cash_amounts[4];
extern sbyte unkn_city_no;
extern ubyte group_types[8];
extern ubyte byte_1C4AA3;
extern ubyte net_unkn_pos_02;
extern ubyte data_1c4aa2;
extern ubyte start_into_mission;
extern ubyte edit_flag;
extern ubyte change_screen;
extern ubyte restore_savegame;
extern ubyte game_projector_speed;
extern ubyte current_drawing_mod;
extern ubyte mod_draw_states[4];
extern ubyte new_current_drawing_mod;
extern ubyte refresh_equip_list;
extern ubyte flic_mods[5];
extern ubyte old_flic_mods[5];
extern ubyte reload_background_flag;
extern ushort word_1811AE;

extern struct LevelDef level_def;

extern long dword_17710C;
extern long dword_177110;
extern ubyte mouser;
extern long dword_1AA5C4;
extern long dword_1AA5C8;

extern short next_ref;
extern ushort next_mission;
extern ushort replay_intro_timer;
extern ubyte show_alert;
extern sbyte mo_weapon;

extern sbyte selected_agent;

extern int mouse_map_x;
extern int mouse_map_y;
extern int mouse_map_z;
extern void *scratch_malloc_mem;

extern ubyte game_gfx_advanced_lights;
extern ubyte game_billboard_movies;
extern ubyte game_gfx_deep_radar;
extern ubyte byte_1C4A6F;

extern char net_unkn2_text[];
extern char brief_netscan_cost_text[];
extern char *misc_text[5];

extern ubyte game_system_screen;
extern ubyte byte_197160;

extern ubyte execute_commands;

// To be replaced by LbArcTanAngle()
short arctan(int dx, int dz);

bool game_initialise (void);
void game_handle_sdl_events (void);
void game_update (void);
int game_hacky_update(void);
void game_quit (void);

/** File name transform function, to be used only for DOS calls simulation.
 */
void game_transform_path (const char *file_name, char *result);

const char *game_get_data_path (void);
const char *game_get_user_path (void);

void read_conf_file(void);
void game_setup(void);
void game_process(void);
void game_reset(void);
void host_reset(void);
void free_texturemaps(void);

/** Decode and draw next frame of the animation.
 */
int xdo_next_frame(ubyte anislot);

/** Decode and draw previous frame of the animation.
 *
 * Note that printing a previous frame of the FLI file requires
 * decoding all frames from start - these files do not use
 * bi-directional FLIC format.
 */
int xdo_prev_frame(ubyte anislot);

void flic_unkn03(ubyte a1);

void my_preprocess_text(char *text);
ushort my_count_lines(const char *text);

TbBool player_try_spend_money(long cost);
TbBool is_unkn_current_player(void);
void campaign_new_game_prepare(void);

void process_sound_heap(void);
void update_danger_music(ubyte a1);

void check_mouse_overvehicle(struct Thing *p_thing, ubyte target_assign);
int mech_unkn_func_03(struct Thing *p_thing);

void draw_new_panel(void);

void update_map_thing_and_traffic_refs(void);
void unkn_lights_processing(void);
void bang_set_detail(int a1);
int sub_73C64(char *a1, ubyte a2);
void func_6fd1c(int a1, int a2, int a3, int a4, int a5, int a6, ubyte a7);
void show_goto_point(uint flag);

void ingame_palette_reload(void);
void game_set_cam_track_thing_xz(ThingIdx thing);
TbBool game_cam_tracked_thing_is_player_agent(void);

void local_to_worldr(int *dx, int *dy, int *dz);
ubyte process_send_person(ushort player, int i);

ubyte *anim_type_get_output_buffer(ubyte anislot);

void net_unkn_func_33(void);
void net_players_copy_equip_and_cryo(void);
void net_players_copy_cryo(void);

void set_default_gfx_settings(void);
void set_default_visual_prefernces(void);
void set_default_audio_tracks(void);

#ifdef __cplusplus
};
#endif

#endif
