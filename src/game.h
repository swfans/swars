#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stddef.h>

#include "bflib_basics.h"
#include "bffile.h"
#include "globals.h"
#include "scanner.h"
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
    GamF_Unkn8000     = 0x8000,
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

enum PurpleDrawType {
  PuDT_NONE = 0,
  PuDT_BOX = 1,
  PuDT_TEXT = 2,
  PuDT_UNK03 = 3,
  PuDT_COPYBOX = 4,
  PuDT_SPRITE = 5,
  PuDT_POTRIG = 6, /* Textured triangle from projector origin point to given line */
  PuDT_FLIC = 7,
  PuDT_SLANTBOX = 8,
  PuDT_LINE = 9,
  PuDT_HVLINE = 10,
  PuDT_TRIANGLE = 11,
  PuDT_HOTSPOT = 12,
};

struct Thing;

struct ScreenPoint {
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
	struct TbSprite *Sprite;
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
	struct TbSprite *Font;
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

struct ColVect { // sizeof=14
  short X1;
  short Y1;
  short Z1;
  short X2;
  short Y2;
  short Z2;
  short Face;
};

/** Collision vectors list.
 *
 * Contains a list of references to boundary vectors used for stepping
 * between ground faces and object faces (buildings).
 */
struct ColVectList { // sizeof=6
  ushort Vect; /**< Index of the ColVect with geometry vector. */
  ushort NextColList; /**< Index of the next ColVectList entry in a chain list, top bit is passability. */
  short Object; /**< Index of a Thing containing the object whose geometry has that vector. */
};

struct ColColumn { // sizeof=16
    uint QBits[4];
};

struct WalkHeader { // sizeof=4
    ushort Count;
    ushort StartItem;
};

struct BezierPt { // sizeof=28
    ubyte field_0[26];
    ushort field_2A;
};

struct FloorTile { // sizeof=39
    short X[4];
    short Y[4];
    struct SingleFloorTexture *Texture;
    ubyte V[4];
    short Shade[4];
    ubyte Col;
    ubyte Flags;
    ubyte Flags2;
    ubyte Flags2b;
    ubyte Page;
    short Offset;
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
    struct Scanner Scanner;
    long Credits;
    short fld_unkC4B;
    short fld_unkC4D;
    short fld_unkC4F;
    short MissionStatus;
    long Flags;
    ushort fld_unkC57;
    short fld_unkC59;
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
    short fld_unkCA6;
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

struct Animation {
  ubyte *OutBuf;
  long field_4;
  short PosX;
  short PosY;
  short field_C;
  ushort Flags;
  long field_10;
  long field_14;
  short field_18;
  short field_1A[1];
  short field_1C[1];
  short field_1E[1];
  long field_20;
  short field_24;
  short field_26[5];
  long field_30;
  long field_34;
  long FileHandle;
  char Filename[48];
  short anonymous_15;
  short field_6E;
};

struct Frame {
    ushort FirstElement;
    ubyte SWidth;
    ubyte SHeight;
    ubyte FX;
    ubyte Flags;
    ushort Next;
};

struct Element { // sizeof=0x0A
    ushort ToSprite;
    short X;
    short Y;
    ushort Flags;
    ushort Next;
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
extern ubyte byte_181183;
extern ubyte byte_181189;

extern ubyte cmdln_param_n;
extern ubyte pktrec_mode;
extern ushort packet_rec_no;
extern ubyte game_perspective;
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

extern ulong smack_malloc_used_tot;
extern ubyte anim_slots[];
extern struct Animation animations[2];

extern ubyte *scratch_buf1;

extern void *dword_1810D1;
extern ulong dword_1810D5;

extern ushort word_1810E4;
extern ubyte byte_1810E6[40];
extern ubyte byte_18110E[40];

extern ushort word_1531E0;
extern struct ColVectList *game_col_vects_list;
extern ushort next_vects_list;
extern struct ColVect *game_col_vects;
extern ushort next_col_vect;

/** Header linking a face to a list of walk items.
 *
 * Some SingleObjectFace* instances have a WalkHeader assigned.
 * If they do, that defines a range of walk items containing
 * neighbor faces, to which it is possible to walk.
 *
 */
extern struct WalkHeader *game_walk_headers;
extern ushort next_walk_header;

/** List of faces which can be walked to from a specific face.
 *
 * Each entry is a face index which has at least corner very close
 * to a corner of current face, allowing walking between the faces.
 */
extern short *game_walk_items;
extern ushort next_walk_item;
extern struct ColColumn *game_col_columns;
extern ushort next_col_column;
extern struct SingleObjectFace3 *game_special_object_faces;
extern struct SingleObjectFace4 *game_special_object_faces4;
extern struct FloorTile *game_floor_tiles;
extern ubyte *game_user_heap;
extern struct SpecialPoint *game_screen_point_pool;
extern struct UnknBezEdit *bezier_pts;
extern ushort next_bezier_pt;
extern ubyte *spare_map_buffer;
extern struct Objective *game_used_lvl_objectives;
extern ushort next_used_lvl_objective;
extern struct LevelMisc *game_level_miscs;
extern long target_old_frameno;
extern ushort word_176E38;

extern struct TbSprite *pop1_sprites;

extern struct TbSprite *unk2_sprites;

extern struct TbSprite *m_sprites;

extern ubyte byte_1C4A7C;
extern ubyte byte_1C4A9F;
extern ubyte linear_vec_pal[256];
extern ulong nsta_size;
extern TbPixel colour_grey1;
extern TbPixel colour_grey2;
extern TbPixel colour_brown2;

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
extern ulong save_mortal_salt;

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
extern ushort overall_scale;
extern ubyte unkn_flags_01;
extern ushort palette_brightness;
extern long outro_unkn01;
extern long outro_unkn02;
extern long outro_unkn03;
extern long people_groups_count;
extern long data_1ddb68;
extern ubyte byte_1DDC40;

extern ubyte debug_hud_collision;

extern void *dword_177750;
extern void *unkn_mech_arr7;

extern char *people_credits_desc[];
extern char *people_credits_groups[];

extern ubyte playable_agents;

extern ubyte save_crypto_tables_state[3];
extern ubyte save_crypto_data_state[3];

extern char *mission_briefing_text;
#define mission_briefing_text_len 16384

extern sbyte mission_result;

extern char *weapon_text;
#define weapon_text_len 32768

extern struct PurpleDrawItem *purple_draw_list;
extern ubyte *save_game_buffer;
extern char save_active_desc[28];
extern ubyte *unkn_buffer_05;
extern ubyte *dword_1C6DE4;
extern ubyte *dword_1C6DE8;
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
extern ubyte new_current_drawing_mod;
extern ubyte refresh_equip_list;
extern ubyte flic_mods[5];
extern ubyte old_flic_mods[5];
extern ubyte reload_background_flag;
extern ushort word_1811AE;

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

extern struct MissionStatus mission_status[120];
extern short old_mission_brief;
extern short open_brief;
extern short next_brief;
extern short next_ref;
extern ushort next_mission;
extern ushort replay_intro_timer;
extern ubyte show_alert;
extern sbyte mo_weapon;

extern ubyte selected_agent;

extern uint mouse_map_x;
extern uint mouse_map_y;
extern uint mouse_map_z;
extern void *scratch_malloc_mem;

extern struct ScreenPoint *hotspot_buffer;
#define hotspot_buffer_len 512

extern ubyte game_gfx_advanced_lights;
extern ubyte game_billboard_movies;
extern ubyte game_gfx_deep_radar;
extern ubyte byte_1C4A6F;

extern char net_unkn2_text[];
extern char brief_netscan_cost_text[];
extern const char *misc_text[5];

extern ubyte game_system_screen;
extern char alert_text[200];
extern ubyte byte_197160;
extern ushort text_window_x1;
extern ushort text_window_y1;
extern ushort text_window_x2;
extern ushort text_window_y2;

// To be replaced by LbArcTanAngle()
short arctan(int dx, int dz);

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
int xdo_next_frame(ubyte a1);

void flic_unkn03(ubyte a1);

void draw_text_purple_list2(int x, int y, const char *text, ushort line);
void draw_sprite_purple_list(int x, int y, struct TbSprite *sprite);
void draw_trig_purple_list(long x2, long y2, long x3, long y3);
void copy_box_purple_list(long x, long y, ulong width, ulong height);
void draw_box_purple_list(int x, int y, ulong width, ulong height, int colour);
void draw_line_purple_list(int x1, int y1, int x2, int y2, int colour);
void draw_triangle_purple_list(int x1, int y1, int x2, int y2, int x3, int y3, TbPixel colour);
void draw_hotspot_purple_list(int x, int y);
void draw_purple_screen(void);
ubyte flashy_draw_purple_shape(struct ScreenShape *shape);
void draw_flic_purple_list(void (*fn)());

void reload_background(void);

void my_preprocess_text(char *text);
ushort my_count_lines(const char *text);
void read_user_settings(void);

TbBool player_try_spend_money(long cost);
TbBool is_unkn_current_player(void);
void campaign_new_game_prepare(void);

void process_sound_heap(void);
void update_danger_music(ubyte a1);
ushort my_draw_text(short x, short y, const char *text, ushort startline);
void draw_text_transformed_at_ground(int a1, int a2, const char *text);

void draw_new_panel(void);

void update_map_thing_and_traffic_refs(void);
void unkn_lights_processing(void);
void bang_set_detail(int a1);
void init_free_explode_faces(void);
int sub_73C64(char *a1, ubyte a2);
void func_6fe80(int a1, int a2, int a3, int a4, int a5, int a6, ubyte a7);
void func_6fd1c(int a1, int a2, int a3, int a4, int a5, int a6, ubyte a7);

#ifdef __cplusplus
};
#endif

#endif
