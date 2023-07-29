#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stddef.h>

#include "bflib_basics.h"
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

struct Thing;

typedef struct {
    char *Name;
    void *BufferPtr;
    ushort ESize;
    long N;
    long dum1;
    long dum2;
    ubyte dum3;
} MemSystem;

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

struct ScreenButton;

struct ScreenButton {
    short X;
    short Y;
    ushort Width;
    ushort Height;
    const char *Text;
    struct TbSprite *Font;
    ubyte (*DrawFn)(struct ScreenButton *btn);
    ubyte (*DrawTextFn)();
    ubyte (*CallBackFn)(ubyte click);
    ubyte *Radio;
    ushort TextTopLine;
    ushort TextFadePos;
    ushort Flags;
    ubyte DrawSpeed;
    ubyte Timer;
    ubyte TextSpeed;
    ubyte Border;
    ubyte Colour;
    ubyte BGColour;
    ubyte AccelKey;
    ubyte RadioValue;
};

struct ScreenBox;

struct ScreenBox
{
    short X;
    short Y;
    ushort Width;
    ushort Height;
    ubyte DrawSpeed;
    ubyte Timer;
    ushort Flags;
    ubyte (*DrawFn)(struct ScreenBox *box);
    ubyte (*SpecialDrawFn)(struct ScreenBox *box);
    ushort Timer2;
    char field_16;
    char field_17;
};

struct ScreenInfoBox;

struct ScreenInfoBox { // sizeof=43
    short X;
    short Y;
    ushort Width;
    ushort Height;
    char *Text1;
    char *Text2;
	struct TbSprite *Font1;
	struct TbSprite *Font2;
	ubyte (*DrawFn)();
	ubyte (*DrawTextFn)();
    short TextFadePos1;
    short TextFadePos2;
    ushort Flags;
    ubyte DrawSpeed;
    ubyte Timer;
    ubyte TextSpeed;
    ubyte Colour;
    ubyte BGColour;
};

struct ScreenTextBox;

struct ScreenTextBox {
  short X;
  short Y;
  ushort Width;
  ushort Height;
  ubyte DrawSpeed;
  ubyte Timer;
  ubyte TextSpeed;
  ubyte LineSpacing;
  short ScrollBarPos;
  ushort ScrollBarSize;
  short ScrollWindowHeight;
  short ScrollWindowOffset;
  ushort GrabPos;
  ushort Lines;
  const char *Text;
  struct TbSprite *Font;
  ubyte (*DrawFn)(struct ScreenTextBox *box);
  ubyte (*DrawTextFn)(struct ScreenTextBox *box);
  struct ScreenButton *Buttons[2];
  short Infos[2];
  //struct ScreenInfoBox *Infos[2];
  ushort TextTopLine;
  ushort field_36;
  ushort field_38;
  short TextFadePos;
  ushort Flags;
  ushort field_3E;
  ubyte BGColour;
  ubyte LineHeight;
  ubyte field_42;
  ubyte field_43;
};

struct UnknStruct04Sub1 { // sizeof=22
    char field_0[6];
    char text[15];
    char field_15;
};

struct UnknStruct04 { // sizeof=218
    char field_0[40];
    struct UnknStruct04Sub1 field_28[8];
    char field_D8;
    char field_D9;
};

struct SingleObject { // sizeof=36
  ushort StartFace;
  ushort NumbFaces;
  ushort NextObject;
  ushort StartFace4;
  ushort NumbFaces4;
  ushort ZScale;
  short OffsetX;
  short OffsetY;
  short OffsetZ;
  short ObjectNo;
  short MapX;
  short MapZ;
  ushort StartPoint;
  ushort EndPoint;
  ubyte field_1C[7];
  ubyte field_23;
};

struct MissionStatus { // sizeof=40
    ulong CityDays;
    ulong CityHours;
    ulong Days;
    ulong Hours;
    ushort CivsKilled;
    ushort CivsPersuaded;
    ushort SecurityKilled;
    ushort SecurityPersuaded;
    ushort EnemiesKilled;
    ushort EnemiesPersuaded;
    ulong CashAtStart;
    ulong Expenditure;
    ubyte HitAccuracy;
    ubyte ObjectivesComplete;
    ubyte AgentsLost;
    ubyte AgentsGained;
};

struct EmailItem { // sizeof=5
	ubyte RefNum;
	ubyte RecvDay;
	ubyte RecvMonth;
	ubyte RecvYear;
	ubyte Mission;
};

struct NewMailItem { // sizeof=5
	ubyte RecvDay;
	ubyte RecvMonth;
	ubyte RecvYear;
	ubyte Mission;
	ubyte Flag;
};

struct ScreenMenuBox { // sizeof=42
  ubyte field_0;
  char field_1[8];
  char field_9[8];
  ubyte field_11[2];
  char field_13[8];
  char field_1B[8];
  char field_23[2];
  ubyte field_25;
  ubyte field_26;
  ushort Flags;
  sbyte field_29;
};

struct UnknGroup { // sizeof=40
  long ugfld_0;
  long ugfld_4;
  long ugfld_8;
  struct WarFlag *war_flags;
  ubyte ugfld_16[8];
  long ugfld_24;
  long ugfld_28;
  long ugfld_32;
  long ugfld_36;
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
    ubyte fld_unk7DE;
    ubyte fld_unk7DF;
    struct Scanner Scanner;
    long Credits;
    short fld_unkC4B;
    short fld_unkC4D;
    short fld_unkC4F;
    short MissionStatus;
    long Flags;
    long fld_unkC57;
    short draw_unknprop_01;
    short Rocket1[15];
    short NextRocket;
    short TrainMode;
    short MyGroup;
    short CurrentMission;
    ubyte fld_unk4AF;
    ubyte DetailLevel;
    short UserZoom;
    short cmdln_param_a;
    short LowerMemoryUse;
    short fld_unkC8B[3];
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
    short fld_unkCA6;
    short fld_unkCA8;
    ubyte fld_unkCAA;
    ubyte PalType;
    short FlameCount;
    ubyte LastTmap;
    short SoundThing;
    ubyte fld_unkCB1;
    ubyte fld_unkCB2;
    short fld_unkCB3;
    short fld_unkCB5;
    short fld_unkCB7;
    /** Flags representing active human players, in both net and local games. */
    ubyte InNetGame_UNSURE;
    ubyte fld_unkCBA[5];
    ubyte Cheats;
    ubyte fld_unkCC0;
    long CashAtStart;
    long Expenditure;
};

#pragma pack()

extern char session_name[20];
extern char user_name[50];
extern char unkn2_names[8][16];
extern char login_name[16];

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

extern ubyte cmdln_param_n;
extern ubyte pktrec_mode;
extern ubyte cmdln_pr_num;
extern ubyte game_perspective;
extern ubyte exit_game;
extern ubyte input_char;

extern ulong active_flags_general_unkn01;

extern long unkn01_downcount;
extern long unkn01_pos_x;
extern long unkn01_pos_y;

extern ushort cmdln_param_current_map;
extern short current_level;

extern ulong engine_mem_alloc_size;
extern void *engine_mem_alloc_ptr;
extern MemSystem mem_game[];

extern long navi2_unkn_counter;
extern long navi2_unkn_counter_max;
extern ulong triangulation;

extern ulong smack_malloc_used_tot;

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
extern ubyte *vec_tmap;
extern ubyte *memload;
extern ubyte net_host_player_no;
extern ubyte data_19ec6f;

extern ubyte background_type;
extern ubyte old_screentype;
extern ubyte screentype;
extern long data_155704;
extern ubyte data_1c498d;
extern ubyte data_1c498e;
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
extern ubyte net_agents__FourPacks[8][4][5]; // maybe a part of larger struct, maybe not

extern ubyte game_high_resolution;
extern char *mission_briefing_text;
extern short mission_open[50];
extern short mission_state[50];
extern char *mem_unkn03;
extern sbyte mission_result;
extern char *weapon_text;
extern struct PurpleDrawItem *purple_draw_list;
extern ubyte *save_game_buffer;
extern char save_active_desc[28];
extern ubyte *unkn_buffer_05;
extern ubyte *data_1c6de4;
extern ubyte *data_1c6de8;
extern ubyte scientists_lost;
extern ulong new_mods_researched;
extern ulong new_weapons_researched;
extern ubyte redraw_screen_flag;
extern sbyte data_15516c;
extern sbyte data_15516d;
extern ubyte data_1c6d48;
extern ubyte unkn_flags_08;
extern long dword_153194;
extern ulong starting_cash_amounts[4];
extern sbyte unkn_city_no;
extern sbyte selected_weapon;
extern sbyte selected_mod;
extern struct UnknStruct04 unkstruct04_arr[20];
extern ubyte data_1c4aa3;
extern ubyte net_unkn_pos_02;
extern ubyte data_1c498f;
extern ubyte data_1c4990;
extern ubyte data_1c4991;
extern ubyte data_1c4aa2;
extern ubyte start_into_mission;
extern ulong text_buf_pos;
extern ubyte edit_flag;
extern ubyte change_screen;
extern ubyte restore_savegame;
extern ubyte game_projector_speed;
extern ubyte current_drawing_mod;
extern ubyte mod_draw_states[4];
extern char equip_cost_text[20];
extern ubyte new_current_drawing_mod;
extern ubyte refresh_equip_list;
extern ubyte flic_mods[5];
extern ubyte reload_background_flag;
extern ushort data_1811ae;

extern struct LevelDef level_def;

extern struct Element *melement_ani;
extern struct Element *mele_ani_end;
extern ushort *nstart_ani;
extern ushort *nstart_ani_end;
extern struct Frame *frame;
extern struct Frame *frame_end;

extern long dword_17710C;
extern long dword_177110;
extern ubyte mouser;
extern ubyte *dword_1AA280;
extern long dword_1AA5C4;
extern long dword_1AA5C8;

extern ubyte num_cities;
extern struct City cities[];
extern sbyte city_id;
extern struct SynTime global_date;
extern struct MissionStatus mission_status[120];
extern short old_mission_brief;
extern short open_brief;
extern short next_brief;
extern short next_ref;
extern ushort next_mission;
extern ushort replay_intro_timer;
extern struct EmailItem email_store[20];
extern struct EmailItem brief_store[10];
extern ubyte show_alert;
extern sbyte mo_weapon;
extern ubyte reload_background_flag;

extern ubyte selected_agent;

extern ushort mouse_map_x;
extern ushort mouse_map_z;
extern ushort render_area_a;
extern ushort render_area_b;
extern void *scratch_malloc_mem;
extern struct StartScreenPoint *hotspot_buffer;

extern ubyte unkn_gfx_option_2;
extern ubyte unkn_option_3;
extern ubyte unkn_option_4;

extern char unkn39_text[];
extern char net_unkn40_text[];
extern char unkn41_text[];
extern char net_unkn2_text[];
extern char equip_name_text[];
extern char unkn_opt_number_text[];
extern char options_title_text[];
extern char brief_netscan_cost_text[];
extern const char *misc_text[5];

extern ubyte game_system_screen;
extern ubyte byte_197160;
extern ushort text_window_x1;
extern ushort text_window_y1;
extern ushort text_window_x2;
extern ushort text_window_y2;

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

extern ubyte new_mail;
extern char net_players[25*5];

bool game_initialise (void);
void adjust_memory_use(void);
void game_handle_sdl_events (void);
void game_update (void);
int game_hacky_update(void);
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
