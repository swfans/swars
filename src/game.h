#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stddef.h>

#include "bflib_basics.h"
#include "bffile.h"
#include "globals.h"
#include "scanner.h"
#include "people.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

// Not sure about the value
#define GAME_FPS 16

enum GameModes {
    GamM_None        = 0,
    GamM_Unkn1       = 1,
    GamM_Unkn2       = 2,
    GamM_Unkn3       = 3,
};

enum GameFlags {
    GamF_Unkn0001     = 0x0001,
    GamF_Unkn0002     = 0x0002,
    GamF_Unkn0004     = 0x0004,
    GamF_Unkn0008     = 0x0008,
    GamF_Unkn0010     = 0x0010,
    GamF_Unkn0020     = 0x0020,
    GamF_Unkn0040     = 0x0040,
    GamF_Unkn0080     = 0x0080,
    GamF_Unkn0100     = 0x0100,
    GamF_Unkn0200     = 0x0200,
    GamF_Unkn0400     = 0x0400,
    GamF_Unkn0800     = 0x0800,
    GamF_Unkn1000     = 0x1000,
    GamF_Unkn2000     = 0x2000,
    GamF_Unkn4000     = 0x4000,
    GamF_Unkn8000     = 0x8000,
    GamF_Unkn00010000 = 0x00010000,
    GamF_Unkn00020000 = 0x00020000,
    GamF_Unkn00040000 = 0x00040000,
    GamF_SkipIntro    = 0x00080000,
    GamF_Unkn00100000 = 0x00100000,
    GamF_Unkn00200000 = 0x00200000,
    GamF_Unkn00400000 = 0x00400000,
    GamF_Unkn00800000 = 0x00800000,
    GamF_Unkn01000000 = 0x01000000,
    GamF_Unkn02000000 = 0x02000000,
    GamF_Unkn04000000 = 0x04000000,
    GamF_Unkn08000000 = 0x08000000,
    GamF_Unkn10000000 = 0x10000000,
};

enum ScreenType {
  SCRT_NONE = 0x0,
  SCRT_MISSION,
  SCRT_2,
  SCRT_WORLDMAP,
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

enum DisplayModes {
  DpM_NONE = 0x0,
  DpM_UNKN_1 = 0x1,
  DpM_2 = 0x2,
  DpM_31 = 0x31,
  DpM_UNKN_32 = 0x32,
  DpM_UNKN_33 = 0x33,
  DpM_34 = 0x34,
  DpM_36 = 0x36,
  DpM_UNKN_37 = 0x37,
  DpM_38 = 0x38,
  DpM_39 = 0x39,
  DpM_UNKN_3A = 0x3A,
  DpM_UNKN_3B = 0x3B,
};

enum PacketRecordMode {
  PktR_NONE = 0x0,
  PktR_RECORD = 0x1,
  PktR_PLAYBACK = 0x2,
};

enum MissionFMVPlay {
    MPly_MissiComplete,
    MPly_MissiFail,
    MPly_MPartComplete,
    MPly_GameOver,
};

struct Thing;

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
	i16 X;
	i16 Y;
};

struct SRect {
	i16 x;
	i16 y;
	i16 w;
	i16 h;
};

struct DITrig { // sizeof=48
	u32 *vlcbuf[2];
	long vlcid;
	u16 *imgbuf;
	struct SRect rect[2];
	long rectid; // offs=0x20
	struct SRect slice;
	long isdone; // offs=0x2C
};

struct DITriangle {
	i16 X1;
	i16 Y1;
	i16 X2;
	i16 Y2;
	i16 X3;
	i16 Y3;
	u8 Colour;
};

struct DIBox {
	i16 X;
	i16 Y;
	i16 Width;
	i16 Height;
	u8 Colour;
};

struct DILine {
	i16 X1;
	i16 Y1;
	i16 X2;
	i16 Y2;
	u8 Colour;
};

struct DISprite { // sizeof=12
	i16 X;
	i16 Y;
	struct Sprite *Sprite;
	u8 Colour;
};

struct DIText { // sizeof=23
	i16 WindowX;
	i16 WindowY;
	i16 Width;
	i16 Height;
	i16 X;
	i16 Y; // offs=0x0A
	char *Text;
	struct Sprite *Font;
	u16 Line;
	u8 Colour; // offs=0x16
};

struct DIFlic {
	void (*Function)();
};

struct DIHotspot {
	i16 X;
	i16 Y;
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
	u8 Type;
	u16 Flags;
};

struct SynTime {
    u8 Minute;
    u8 Hour;
    u8 Day;
    u8 Month;
    u8 Year;
};

struct ScreenButton;

struct ScreenButton {
    i16 X;
    i16 Y;
    u16 Width;
    u16 Height;
    const char *Text;
    struct TbSprite *Font;
    u8 (*DrawFn)(struct ScreenButton *btn);
    u8 (*DrawTextFn)();
    u8 (*CallBackFn)(u8 click);
    u8 *Radio;
    u16 TextTopLine;
    u16 TextFadePos;
    u16 Flags;
    u8 DrawSpeed;
    u8 Timer;
    u8 TextSpeed;
    u8 Border;
    u8 Colour;
    u8 BGColour;
    u8 AccelKey;
    u8 RadioValue;
};

struct ScreenBox;

struct ScreenBox
{
    i16 X;
    i16 Y;
    u16 Width;
    u16 Height;
    u8 DrawSpeed;
    u8 Timer;
    u16 Flags;
    u8 (*DrawFn)(struct ScreenBox *box);
    u8 (*SpecialDrawFn)(struct ScreenBox *box);
    u16 Timer2;
    char field_16;
    char field_17;
};

struct ScreenInfoBox;

struct ScreenInfoBox { // sizeof=43
    i16 X;
    i16 Y;
    u16 Width;
    u16 Height;
    char *Text1;
    char *Text2;
	struct TbSprite *Font1;
	struct TbSprite *Font2;
	u8 (*DrawFn)();
	u8 (*DrawTextFn)();
    i16 TextFadePos1;
    i16 TextFadePos2;
    u16 Flags;
    u8 DrawSpeed;
    u8 Timer;
    u8 TextSpeed;
    u8 Colour;
    u8 BGColour;
};

struct ScreenTextBox;

struct ScreenTextBox {
  i16 X;
  i16 Y;
  u16 Width;
  u16 Height;
  u8 DrawSpeed;
  u8 Timer;
  u8 TextSpeed;
  u8 LineSpacing;
  i16 ScrollBarPos;
  u16 ScrollBarSize;
  i16 ScrollWindowHeight;
  i16 ScrollWindowOffset;
  u16 GrabPos;
  u16 Lines;
  const char *Text;
  struct TbSprite *Font;
  u8 (*DrawFn)(struct ScreenTextBox *box);
  u8 (*DrawTextFn)(struct ScreenTextBox *box);
  struct ScreenButton *Buttons[2];
  i16 Infos[2];
  //struct ScreenInfoBox *Infos[2];
  u16 TextTopLine;
  u16 field_36;
  u16 field_38;
  i16 TextFadePos;
  u16 Flags;
  u16 field_3E;
  u8 BGColour;
  u8 LineHeight;
  u8 field_42;
  u8 field_43;
};

struct SingleObject { // sizeof=36
  u16 StartFace;
  u16 NumbFaces;
  u16 NextObject;
  u16 StartFace4;
  u16 NumbFaces4;
  u16 ZScale;
  i16 OffsetX;
  i16 OffsetY;
  i16 OffsetZ;
  i16 ObjectNo;
  i16 MapX;
  i16 MapZ;
  u16 StartPoint;
  u16 EndPoint;
  u8 field_1C[7];
  u8 field_23;
};

struct MissionStatus { // sizeof=40
    u32 CityDays;
    u32 CityHours;
    u32 Days;
    u32 Hours;
    u16 CivsKilled;
    u16 CivsPersuaded;
    u16 SecurityKilled;
    u16 SecurityPersuaded;
    u16 EnemiesKilled;
    u16 EnemiesPersuaded;
    u32 CashAtStart;
    u32 Expenditure;
    u8 HitAccuracy;
    u8 ObjectivesComplete;
    u8 AgentsLost;
    u8 AgentsGained;
};

struct EmailItem { // sizeof=5
	u8 RefNum;
	u8 RecvDay;
	u8 RecvMonth;
	u8 RecvYear;
	u8 Mission;
};

struct NewMailItem { // sizeof=5
	u8 RecvDay;
	u8 RecvMonth;
	u8 RecvYear;
	u8 Mission;
	u8 Flag;
};

struct ScreenMenuBox { // sizeof=42
  u8 field_0;
  char field_1[8];
  char field_9[8];
  u8 field_11[2];
  char field_13[8];
  char field_1B[8];
  char field_23[2];
  u8 field_25;
  u8 field_26;
  u16 Flags;
  i8 field_29;
};

struct LevelDef {
  u8 PlayableGroups[8];
  u8 field_8[33];
  u8 field_29;
  u8 field_2A;
  u8 field_2B;
};

struct InGame {
    i16 GameMode;
    i16 DisplayMode;
    i16 fld_unk7D8;
    u8 fld_unk7DA;
    u8 CDTrack;
    u8 DangerTrack;
    u8 UseMultiMedia;
    u8 fld_unk7DE;
    u8 GameOver;
    struct Scanner Scanner;
    long Credits;
    i16 fld_unkC4B;
    i16 fld_unkC4D;
    i16 fld_unkC4F;
    i16 MissionStatus;
    long Flags;
    u16 fld_unkC57;
    i16 fld_unkC59;
    i16 draw_unknprop_01;
    i16 Rocket1[15];
    i16 NextRocket;
    i16 TrainMode;
    i16 MyGroup;
    i16 CurrentMission;
    u8 fld_unk4AF;
    u8 DetailLevel;
    i16 UserZoom;
    i16 cmdln_param_a;
    i16 LowerMemoryUse;
    i16 fld_unkC8B[3];
    long fld_unkC91;
    u16 TrackX;
    u16 TrackZ;
    i16 TrackThing;
    u8 fld_unkC98[5];
    i16 fld_unkCA0;
    u8 fld_unkCA2;
    i8 PanelPermutation;
    u8 TrenchcoatPreference;
    u8 MissionNo;
    i16 fld_unkCA6;
    i16 fld_unkCA8;
    u8 fld_unkCAA;
    u8 PalType;
    i16 FlameCount;
    u8 LastTmap;
    i16 SoundThing;
    u8 fld_unkCB1;
    u8 fld_unkCB2;
    i16 fld_unkCB3;
    i16 fld_unkCB5;
    i16 fld_unkCB7;
    /** Flags representing active human players, in both net and local games. */
    u8 InNetGame_UNSURE;
    u8 fld_unkCBA[5];
    u8 Cheats;
    u8 fld_unkCC0;
    long CashAtStart;
    long Expenditure;
};

struct Animation {
  u8 *OutBuf;
  long field_4;
  i16 PosX;
  i16 PosY;
  i16 field_C;
  u16 Flags;
  long field_10;
  long field_14;
  i16 field_18;
  i16 field_1A[1];
  i16 field_1C[1];
  i16 field_1E[1];
  long field_20;
  i16 field_24;
  i16 field_26[5];
  long field_30;
  long field_34;
  long FileHandle;
  char Filename[48];
  i16 anonymous_15;
  i16 field_6E;
};

#pragma pack()

extern char session_name[20];
extern char user_name[50];
extern char unkn2_names[8][16];
extern char login_name[16];

extern u8 in_network_game;
extern u8 is_single_game;
extern u8 cmdln_colour_tables;
extern u8 cmdln_param_bcg;
extern u8 keyboard_mode_direct;
extern u8 unkn01_maskarr[28];
extern long map_editor;

extern struct InGame ingame;

extern u8 login_control__State;
extern u32 login_control__Money;
extern i8 login_control__City;
extern u8 login_control__TechLevel;
extern u8 byte_181183;
extern u8 byte_181189;

extern u8 cmdln_param_n;
extern u8 pktrec_mode;
extern u16 packet_rec_no;
extern u8 game_perspective;
extern u8 exit_game;
extern u8 input_char;

extern u32 active_flags_general_unkn01;

extern long unkn01_downcount;
extern long unkn01_pos_x;
extern long unkn01_pos_y;

extern u16 current_map;
extern i16 current_level;

extern u32 engine_mem_alloc_size;
extern void *engine_mem_alloc_ptr;

extern long navi2_unkn_counter;
extern long navi2_unkn_counter_max;
extern u32 triangulation;

extern u32 smack_malloc_used_tot;
extern u8 anim_slots[];
extern struct Animation animations[2];

#define STRINGS_MAX 652

extern char *gui_strings_data;
extern char *gui_strings_data_end;
extern char *gui_strings[STRINGS_MAX];

extern u8 *fade_data;

extern void *dword_1810D1;
extern u32 dword_1810D5;

extern struct SingleFloorTexture *game_textures;
extern struct SingleTexture *game_face_textures;
extern struct SinglePoint *game_object_points;
extern u16 next_normal;
extern u16 next_quick_light;
extern u16 next_full_light;
extern u16 word_1531E0;
extern u16 next_face_texture;
extern u16 next_floor_texture;
extern u16 next_object_point;
extern u16 next_object_face;
extern u16 next_object_face4;
extern u16 next_object;
extern struct SingleObjectFace3 *game_object_faces;
extern struct SingleObject *game_objects;
extern struct QuickLight *game_quick_lights;
extern struct FullLight *game_full_lights;
extern struct Normal *game_normals;
extern u16 next_local_mat;
extern u16 next_special_face;
extern u16 next_special_face4;
extern struct SingleObjectFace4 *game_object_faces4;
extern struct AnimTmap *game_anim_tmaps;
extern struct TrafficNode *game_traffic_nodes;
extern struct LightCommand *game_light_commands;
extern struct ColVectList *game_col_vects_list;
extern struct ColVect *game_col_vects;
extern struct WalkHeader *game_walk_headers;
extern i16 *game_walk_items;
extern struct ColColumn *game_col_columns;
extern struct SingleObjectFace3 *game_special_object_faces;
extern struct SingleObjectFace4 *game_special_object_faces4;
extern struct FloorTile *game_floor_tiles;
extern u16 next_col_vect;
extern u8 *game_user_heap;
extern struct SpecialPoint *game_screen_point_pool;
extern struct DrawItem *game_draw_list;
extern struct SortSprite *game_sort_sprites;
extern struct SortLine *game_sort_lines;
extern struct UnknBezEdit *bez_edit;
extern u8 *spare_map_buffer;
extern struct Objective *game_used_lvl_objectives;
extern u16 next_used_lvl_objective;
extern struct LevelMisc *game_level_miscs;
extern u16 word_176E38;

extern PrimObjectPoint *prim_object_points;
extern PrimObjectFace *prim_object_faces;
extern PrimObjectFace4 *prim_object_faces4;
extern PrimObject *prim_objects;
extern Prim4Texture *prim4_textures;
extern PrimFaceTexture *prim_face_textures;

extern u16 prim_object_points_count;
extern u16 prim_object_faces_count;
extern u16 prim_object_faces4_count;
extern u16 prim_objects_count;
extern u16 prim4_textures_count;
extern u16 prim_face_textures_count;
extern u16 prim_unknprop01;

extern u8 byte_1C4A7C;
extern u8 byte_1C4A9F;
extern u32 curr_tick_time;
extern u32 prev_tick_time;
extern GameTurn gameturn;
extern GameTurn prev_gameturn;
extern u32 turns_delta;
extern u16 fifties_per_gameturn;
extern u16 gamep_unknval_01;
extern u8 *vec_tmap;

extern u8 *memload;
#define memload_len 16384

extern u8 net_host_player_no;
extern u8 byte_1C6D4A;
extern u8 byte_1C6DDC[5];
extern u16 word_1C8446;
extern u8 data_19ec6f;
extern u32 save_mortal_salt;

extern u16 weapon_text_index[32];
extern u16 cybmod_text_index[16];
extern u8 background_type;
extern u8 old_screentype;
extern u8 screentype;
extern long data_155704;
extern i16 flic_mod_coords_b[8];
extern u8 flic_mod_heights[4];
extern u8 flic_mod_widths[4];
extern u8 data_1c498d;
extern u8 data_1c498e;
extern char *outro_text_s;
extern char *outro_text_z;
extern long data_197150;
extern long data_1dd91c;
extern u16 overall_scale;
extern u8 unkn_flags_01;
extern u16 palette_brightness;
extern long outro_unkn01;
extern long outro_unkn02;
extern long outro_unkn03;
extern long people_groups_count;
extern long data_1ddb68;
extern u8 byte_1DDC40;

extern char *people_credits_desc[];
extern char *people_credits_groups[];

extern u8 playable_agents;

extern u8 save_crypto_tables_state[3];
extern u8 save_crypto_data_state[3];

extern u8 game_high_resolution;

extern char *mission_briefing_text;
#define mission_briefing_text_len 16384

extern i16 mission_open[50];
extern i16 mission_state[50];
extern i8 mission_result;

extern char *weapon_text;
#define weapon_text_len 32768

extern struct PurpleDrawItem *purple_draw_list;
extern u8 *save_game_buffer;
extern char save_active_desc[28];
extern u8 *unkn_buffer_05;
extern u8 *data_1c6de4;
extern u8 *data_1c6de8;
extern u8 scientists_lost;
extern u32 new_mods_researched;
extern u32 new_weapons_researched;
extern u8 redraw_screen_flag;

extern i8 byte_15516C;
extern i8 byte_15516D;
extern u8 byte_1C5C28[8];
extern u8 byte_1C6D48;
extern u8 unkn_flags_08;
extern long dword_153194;
extern u32 starting_cash_amounts[4];
extern i8 unkn_city_no;
extern i8 selected_weapon;
extern i8 selected_mod;
extern u8 group_types[8];
extern u8 byte_1C4AA3;
extern u8 net_unkn_pos_02;
extern u8 data_1c498f;
extern u8 data_1c4990;
extern u8 data_1c4991;
extern u8 data_1c4aa2;
extern u8 start_into_mission;
extern u32 text_buf_pos;
extern u8 edit_flag;
extern u8 change_screen;
extern u8 restore_savegame;
extern u8 game_projector_speed;
extern u8 current_drawing_mod;
extern u8 mod_draw_states[4];
extern char equip_cost_text[20];
extern u8 new_current_drawing_mod;
extern u8 refresh_equip_list;
extern u8 flic_mods[5];
extern u8 old_flic_mods[5];
extern u8 reload_background_flag;
extern u16 word_1811AE;

extern struct LevelDef level_def;

extern struct Element *melement_ani;
extern struct Element *mele_ani_end;
extern u16 *nstart_ani;
extern u16 *nstart_ani_end;
extern struct Frame *frame;
extern struct Frame *frame_end;

extern long dword_17710C;
extern long dword_177110;
extern u8 mouser;
extern u8 *dword_1AA280;
extern long dword_1AA5C4;
extern long dword_1AA5C8;

extern struct SynTime global_date;
extern struct SynTime research_curr_wep_date;
extern struct SynTime research_curr_mod_date;

extern struct MissionStatus mission_status[120];
extern u8 new_mail;
extern struct NewMailItem newmail_store[29];
extern u16 next_email;
extern i16 old_mission_brief;
extern i16 open_brief;
extern i16 next_brief;
extern i16 next_ref;
extern u16 next_mission;
extern u16 replay_intro_timer;
extern struct EmailItem email_store[20];
extern struct EmailItem brief_store[10];
extern u8 show_alert;
extern i8 mo_weapon;
extern u8 reload_background_flag;

extern u8 selected_agent;

extern u16 mouse_map_x;
extern u16 mouse_map_z;
extern u16 render_area_a;
extern u16 render_area_b;
extern void *scratch_malloc_mem;

extern struct StartScreenPoint *hotspot_buffer;
#define hotspot_buffer_len 512

extern u8 unkn_gfx_option_2;
extern u8 unkn_option_3;
extern u8 unkn_option_4;
extern u8 byte_1C4A6F;

extern char unkn39_text[];
extern char net_unkn40_text[];
extern char unkn41_text[];
extern char net_unkn2_text[];
extern char equip_name_text[];
extern char unkn_opt_number_text[];
extern char options_title_text[];
extern char brief_netscan_cost_text[];
extern const char *misc_text[5];

extern u8 game_system_screen;
extern char alert_text[200];
extern u8 byte_197160;
extern u16 text_window_x1;
extern u16 text_window_y1;
extern u16 text_window_x2;
extern u16 text_window_y2;

extern struct ScreenTextBox heading_box;
extern struct ScreenTextBox loading_INITIATING_box;
extern struct ScreenTextBox unkn13_SYSTEM_button;
extern struct ScreenTextBox unkn35_box;
extern struct ScreenBox unkn39_box;
extern struct ScreenTextBox unkn37_box;
extern struct ScreenButton sysmnu_buttons[6];
extern struct ScreenButton storage_LOAD_button;
extern struct ScreenButton storage_SAVE_button;
extern struct ScreenButton storage_NEW_MORTAL_button;
extern struct ScreenBox unkn34_box;
extern struct ScreenBox unkn04_boxes[3];
extern struct ScreenBox unkn33_box;
extern struct ScreenButton options_audio_buttons[7];
extern struct ScreenButton options_gfx_buttons[16];
extern struct ScreenButton net_protocol_option_button;
extern struct ScreenBox unkn32_box;
extern struct ScreenTextBox unkn36_box;
extern struct ScreenTextBox mission_text_box;
extern struct ScreenButton unkn1_ACCEPT_button;
extern struct ScreenButton unkn1_CANCEL_button;
extern struct ScreenButton brief_NETSCAN_button;
extern struct ScreenInfoBox brief_NETSCAN_COST_box;
extern struct ScreenBox unkn30_box;
extern struct ScreenBox unkn31_box;
extern struct ScreenBox unkn29_box;
extern struct ScreenTextBox unkn38_box;
extern struct ScreenButton unkn2_ACCEPT_button;
extern struct ScreenButton unkn2_CANCEL_button;
extern struct ScreenButton main_quit_button;
extern struct ScreenButton main_login_button;
extern struct ScreenButton pause_continue_button;
extern struct ScreenButton pause_abort_button;
extern struct ScreenButton main_map_editor_button;
extern struct ScreenButton main_load_button;
extern struct ScreenBox pause_unkn11_box;
extern struct ScreenBox pause_unkn12_box;
extern struct ScreenTextBox slots_box;
extern struct ScreenTextBox equip_name_box;
extern struct ScreenBox weapon_slots;
extern struct ScreenTextBox equip_list_box;
extern struct ScreenTextBox equip_display_box;
extern struct ScreenButton buy_equip_button;
extern struct ScreenButton unkn11_CANCEL_button;
extern struct ScreenInfoBox equip_cost_box;
extern struct ScreenMenuBox unk11_menu[5];
extern struct ScreenButton all_agents_button;
extern struct ScreenTextBox research_unkn21_box;
extern struct ScreenButton research_submit_button;
extern struct ScreenButton unkn12_WEAPONS_MODS_button;
extern struct ScreenTextBox research_progress_button;
extern struct ScreenBox research_unkn20_box;
extern struct ScreenButton research_list_buttons[2];
extern struct ScreenTextBox agent_list_box;
extern struct ScreenBox blokey_box;
extern struct ScreenTextBox mod_list_box;
extern struct ScreenButton net_INITIATE_button;
extern struct ScreenButton net_groups_LOGON_button;
extern struct ScreenButton unkn8_EJECT_button;
extern struct ScreenButton net_protocol_select_button;
extern struct ScreenButton net_unkn40_button;
extern struct ScreenButton net_SET2_button;
extern struct ScreenButton net_SET_button;
extern struct ScreenBox net_groups_box;
extern struct ScreenBox net_users_box;
extern struct ScreenBox net_faction_box;
extern struct ScreenBox net_team_box;
extern struct ScreenBox net_benefits_box;
extern struct ScreenBox net_comms_box;
extern struct ScreenBox net_unkn21;
extern struct ScreenBox net_protocol_box;
extern struct ScreenButton unkn10_CALIBRATE_button;
extern struct ScreenButton unkn10_SAVE_button;
extern struct ScreenButton unkn10_CONTROLS_button;

extern struct ScreenBox alert_box;
extern struct ScreenButton alert_OK_button;

bool game_initialise (void);
void game_handle_sdl_events (void);
void game_update (void);
int game_hacky_update(void);
void game_quit (void);
void game_transform_path (const char *file_name, char *result);
const char *game_get_data_path (void);
const char *game_get_user_path (void);

void load_multicolor_sprites(void);

void read_conf_file(void);
void game_setup(void);
void game_process(void);
void game_reset(void);
void host_reset(void);
void free_texturemaps(void);
int joy_grip_shutdown(void);

void my_preprocess_text(char *text);
u16 my_count_lines(const char *text);

#ifdef __cplusplus
};
#endif

#endif
