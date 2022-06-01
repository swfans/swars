#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "bfdata.h"
#include "bfsprite.h"
#include "bfscreen.h"
#include "bfkeybd.h"
#include "bfmouse.h"
#include "bfpalette.h"
#include "bfmemory.h"
#include "bfmemut.h"
#include "bffile.h"
#include "bfgentab.h"
#include "svesa.h"
#include "bflib_render.h"
#include "bflib_fmvids.h"
#include "bflib_snd_sys.h"
#include "bflib_snd_cda.h"
#include "game_data.h"
#include "display.h"
#include "dos.h"
#include "game.h"
#include "keyboard.h"
#include "mouse.h"
#include "sound.h"
#include "unix.h"
#include "util.h"
#include "windows.h"
#include "research.h"
#include "thing.h"
#include "player.h"

#include "timer.h"

#define SAVEGAME_PATH "qdata/savegame/"

#pragma pack(1)

struct UnkStruct7
{
  char field_0;
  char field_1;
  char field_2;
  char field_3;
  char field_4;
  char field_5;
  char field_6;
  char field_7;
  char field_8;
  char field_9;
  char field_A;
  char field_B;
  char field_C;
  char field_D;
  char field_E;
  char field_F;
};

struct Element;
struct Frame;

#pragma pack()

extern uint8_t game_music_track;

extern unsigned char *fade_data;
extern char *fadedat_fname;
extern char *pop_dat_fname_fmt;
extern char *pop_tab_fname_fmt;
extern unsigned short ingame__draw_unknprop_01;
extern unsigned long unkn_buffer_04;

extern struct TbSprite *small_font;
extern struct TbSprite *small_font_end;
extern ubyte *small_font_data;
extern struct TbSprite *pointer_sprites;
extern struct TbSprite *pointer_sprites_end;
extern ubyte *pointer_data;
extern struct TbSprite *pop1_sprites;
extern struct TbSprite *pop1_sprites_end;
extern ubyte *pop1_data;
extern struct TbSprite *m_sprites;
extern struct TbSprite *m_sprites_end;
extern ubyte *m_spr_data;
extern ubyte *m_spr_data_end;

extern struct Element *melement_ani;
extern struct Element *mele_ani_end;
extern ushort *nstart_ani;
extern ushort *nstart_ani_end;
extern struct Frame *frame;
extern struct Frame *frame_end;

extern unsigned short ingame__DisplayMode;
extern unsigned char *display_palette;
extern unsigned short unkn2_pos_x;
extern unsigned short unkn2_pos_y;
extern unsigned short unkn2_pos_z;
extern int data_1c8428;
extern const char *primvehobj_fname;
extern unsigned char data_19ec6f;
extern unsigned char textwalk_data[640];

extern PrimObjectPoint *prim_object_points;
extern PrimObjectFace *prim_object_faces;
extern PrimObjectFace4 *prim_object_faces4;
extern PrimObject *prim_objects;
extern Prim4Texture *prim4_textures;
extern PrimFaceTexture *prim_face_textures;

extern ushort prim_object_points_count;
extern ushort prim_object_faces_count;
extern ushort prim_object_faces4_count;
extern ushort prim_objects_count;
extern ushort prim4_textures_count;
extern ushort prim_face_textures_count;
extern ushort prim_unknprop01;
extern struct UnkStruct7 *game_panel;
extern struct UnkStruct7 game_panel_lo[];
extern struct UnkStruct7 unknstrct7_arr2[];

extern uint8_t execute_commands;
extern long gamep_unknval_10;
extern long gamep_unknval_11;
extern long gamep_unknval_12;
extern long gamep_unknval_13;
extern long gamep_unknval_14;
extern long gamep_unknval_15;
extern long gamep_unknval_16;


extern int8_t ingame__TrenchcoatPreference;
extern int8_t ingame__PanelPermutation;

struct TbLoadFiles unk02_load_files[] =
{
  { "*VESA",			(void **)&lbVesaData,		(void **)NULL,LB_VESA_DATA_SIZE, 1, 0 },
#if 1 // !defined(BFLIB_WSCREEN_CONTROL)
  { "*W_SCREEN",		(void **)&lbDisplay.WScreen,(void **)NULL,MAX_SUPPORTED_SCREEN_WIDTH*(MAX_SUPPORTED_SCREEN_HEIGHT+1), 0, 0 },
#endif
  { "data/pop2-1.dat",	(void **)&pop1_data,		(void **)NULL,			0, 0, 0 },
  { "data/pop2-1.tab",	(void **)&pop1_sprites,		(void **)&pop1_sprites_end, 0, 0, 0 },
  { "data/mspr-0.dat",	(void **)&m_spr_data,		(void **)&m_spr_data_end,0, 0, 0 },
  { "data/mspr-0.tab",	(void **)&m_sprites,		(void **)&m_sprites_end,0, 0, 0 },
  { "data/mele-0.ani",	(void **)&melement_ani,		(void **)&mele_ani_end,	0, 0, 0 },
  { "data/nsta-0.ani",	(void **)&nstart_ani,		(void **)&nstart_ani_end,0, 0, 0 },
  { "data/nfra-0.ani",	(void **)&frame,			(void **)&frame_end,	0, 0, 0 },
  { "data/font0-0.dat",	(void **)&small_font_data,	(void **)NULL,			0, 0, 0 },
  { "data/font0-0.tab",	(void **)&small_font,		(void **)&small_font_end, 0, 0, 0 },
  { "data/pointers.dat",(void **)&pointer_data,		(void **)NULL,			0, 0, 0 },
  { "data/pointers.tab",(void **)&pointer_sprites,	(void **)&pointer_sprites_end, 0, 0, 0 },
  { "qdata/pal.pal",	(void **)&display_palette,	(void **)NULL,			0, 0, 0 },
  { "",					(void **)NULL, 				(void **)NULL,			0, 0, 0 }
};

extern TbFileHandle packet_rec_fh;

char unk_credits_text_s[] = "";
char unk_credits_text_z[] = "";
char unk_credits_text_p[] = "";

unsigned int LbRandomAnyShort(void);

int LbPaletteFade(uint8_t *a1, uint8_t a2, uint8_t a3)
{
    int ret;
    asm volatile ("call ASM_LbPaletteFade\n"
        : "=r" (ret) : "a" (a1), "d" (a2), "b" (a3));
    return ret;
}

void PacketRecord_Close(void)
{
    if (in_network_game)
        return;
    LbFileClose(packet_rec_fh);
}

void debug_trace_place(int place)
{
}

bool
game_initialise(void)
{
    if (SDL_Init (SDL_INIT_JOYSTICK | SDL_INIT_VIDEO
          | SDL_INIT_NOPARACHUTE) != 0)
    {
        ERRORLOG("SDL_Init(): %s", SDL_GetError());
        return false;
    }

#ifdef __unix__
    /* clean up after SDL messing around where it shouldn't */
    unix_restore_signal_handlers();
#endif

    LbSetUserResourceMapping(SWResourceMapping);
    LbSetTitle(PACKAGE_NAME);
    LbSetIcon(1);

    mouse_initialise();
    sound_initialise();

    // Make sure file names are properly converted before opening
    setup_file_names();

    if ( cmdln_param_w == 1 )
    {
        buffer_allocs[35].field_A = 1;
        buffer_allocs[28].field_A = 1;
        buffer_allocs[36].field_A = 1;
        buffer_allocs[27].field_A = 1000;
        buffer_allocs[26].field_A = 1124;
        buffer_allocs[31].field_A = 2500;
        buffer_allocs[32].field_A = 1000;
        buffer_allocs[33].field_A = 700;
        buffer_allocs[30].field_A = 3000;
        if ( is_single_game || cmdln_param_bcg )
        {
            buffer_allocs[20].field_A = 2000;
            buffer_allocs[21].field_A = 2000;
            buffer_allocs[22].field_A = 2000;
        }
        buffer_allocs[4].field_A = 11000;
        buffer_allocs[9].field_A = 11000;
        buffer_allocs[5].field_A = 1500;
        buffer_allocs[7].field_A = 1000;
        buffer_allocs[13].field_A = 16000;
        buffer_allocs[14].field_A = 9000;
        engine_mem_alloc_size = 2700000;
        game_perspective = (buffer_allocs[5].field_A >> 8) & 0xff;
    }
    if ( !is_single_game )
        cmdln_param_bcg = 1;

    return true;
}

TbResult KEvent(const SDL_Event *ev);

void
game_handle_sdl_events (void)
{
    const size_t max_events = 256;
    size_t event;
    SDL_Event ev;

    for (event = 0; event < max_events; event++)
    {
        if (SDL_PollEvent(&ev) == 0)
            break;

        switch (ev.type)
        {
        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEBUTTONDOWN:
            MEvent(&ev);
            break;

        case SDL_KEYUP:
        case SDL_KEYDOWN:
            KEvent(&ev);
            break;

        case SDL_QUIT:
            game_quit();
        }
    }
}

void ASM_game_setup(void);
void ASM_load_texturemaps(void);
void ASM_load_prim_quad(void);
void ASM_game_setup_sub1(void);
void ASM_init_arrays_1(void);
void ASM_game_setup_sub3(void);
void ASM_game_setup_sub5(void);
void ASM_setup_host(void);
void ASM_read_user_settings(void);
void ASM_setup_color_lookups(void);
void ASM_game_setup_sub8(void);
void ASM_init_engine(void);
void ASM_play_intro(void);
void ASM_init_syndwars(void);
void ASM_setup_host_sub6(void);
int ASM_setup_mele(void);

void *ASM_smack_malloc(int msize);
void ASM_smack_mfree(void *ptr);
void *(*smack_malloc)(int);
void (*smack_free)(void *);


void load_texturemaps(void)
{
    ASM_load_texturemaps();
}

void test_open(int num)
{
    // Empty for production version
}

void read_textwalk(void)
{
    TbFileHandle handle;
    handle = LbFileOpen("data/textwalk.dat", Lb_FILE_MODE_READ_ONLY);
    if ( handle != INVALID_FILE )
    {
        LbFileRead(handle, textwalk_data, 640);
        LbFileClose(handle);
    }
}

void read_primveh_obj(const char *fname, int a2)
{
    long firstval;
    TbFileHandle fh;

    fh = LbFileOpen(fname, Lb_FILE_MODE_READ_ONLY);
    if ( fh == INVALID_FILE )
        return;
    LbFileRead(fh, &firstval, sizeof(long));
    if ( firstval != 1 )
    {
      LbFileRead(fh, &prim_object_points_count, sizeof(ushort));
      LbFileRead(fh, &prim_object_faces_count, sizeof(ushort));
      LbFileRead(fh, &prim_object_faces4_count, sizeof(ushort));
      LbFileRead(fh, &prim_objects_count, sizeof(ushort));
      LbFileRead(fh, &prim4_textures_count, sizeof(ushort));
      LbFileRead(fh, &prim_face_textures_count, sizeof(ushort));
      LbFileRead(fh, &prim_unknprop01, sizeof(ushort));
      LbFileRead(fh, prim_object_points, sizeof(PrimObjectPoint) * prim_object_points_count);
      LbFileRead(fh, prim_object_faces, sizeof(PrimObjectFace) * prim_object_faces_count);
      LbFileRead(fh, prim_object_faces4, sizeof(PrimObjectFace4) * prim_object_faces4_count);
      LbFileRead(fh, prim_objects, sizeof(PrimObject) * prim_objects_count);
      LbFileRead(fh, prim4_textures, sizeof(Prim4Texture) * prim4_textures_count);
      LbFileRead(fh, prim_face_textures, sizeof(PrimFaceTexture) * prim_face_textures_count);
    }
    LbFileClose(fh);
}

void load_prim_quad(void)
{
    unkn2_pos_x = 64;
    unkn2_pos_y = 64;
    unkn2_pos_z = 64;
    data_1c8428 = 0;
    prim_unknprop01 = 1000;
    read_primveh_obj(primvehobj_fname, 1);
    read_textwalk();
    data_19ec6f = 1;
    ingame__DisplayMode = 55;
    if ( cmdln_param_bcg == 99 )
        test_open(99);
    if ( cmdln_param_bcg == 100 )
        test_open(100);
}

void game_setup_sub1(void)
{
    ASM_game_setup_sub1();
}

void init_arrays_1(void)
{
    ASM_init_arrays_1();
}

void game_setup_sub2(int a1)
{
    asm volatile ("call ASM_game_setup_sub2\n"
        : : "a" (a1));
}

void game_setup_sub3(void)
{
    ASM_game_setup_sub3();
}

void game_setup_sub4(int a1)
{
    test_open(0);
}

void game_setup_sub5(void)
{
    ASM_game_setup_sub5();
}

void flic_unkn03(ubyte a1)
{
    asm volatile ("call ASM_flic_unkn03\n"
        : : "a" (a1));
}

void play_smacker(int vid_type)
{
    asm volatile ("call ASM_play_smacker\n"
        : : "a" (vid_type));
}

void play_intro(void)
{
    char fname[FILENAME_MAX];
    //ASM_play_intro(); return;

    lbDisplay.LeftButton = 0;
    lbKeyOn[KC_ESCAPE] = 0;
    if ( (cmdln_param_bcg || is_single_game) && !(ingame__Flags & GamF_Unkn80000) )
    {
        setup_screen_mode(Lb_SCREEN_MODE_320_200_8);
        LbMouseChangeSprite(NULL);
        if (game_dirs[DirPlace_Sound].use_cd == 1)
            sprintf(fname, "%slanguage/%s/intro.smk", cd_drive, language_3str);
        else
            sprintf(fname, "intro/intro.smk");
        play_smk(fname, 13, 0);
        LbMouseChangeSprite(&pointer_sprites[1]);
        smack_malloc_used_tot = 0;
    }
    if (cmdln_param_bcg)
        setup_screen_mode(Lb_SCREEN_MODE_640_480_8);
    flic_unkn03(1u);
}

void reset_heaps(void)
{
}

size_t setup_heaps(int a1)
{
    size_t ret;
    asm volatile ("call ASM_setup_heaps\n"
        : "=r" (ret) : "a" (a1));
    return ret;
}

char func_cc638(const char *text, const char *fname)
{
    char ret;
    asm volatile ("call ASM_func_cc638\n"
        : "=r" (ret) : "a" (text), "d" (fname));
    return ret;
}

void screen_dark_curtain_down(void)
{
    asm volatile ("call ASM_screen_dark_curtain_down\n"
        :  :  : "eax" );
}

void load_outro_sprites(void)
{
    asm volatile ("call ASM_load_outro_sprites\n"
        :  :  : "eax" );
}

void fill_floor_textures(void)
{
    asm volatile ("call ASM_fill_floor_textures\n"
        :  :  : "eax" );
}

void load_mad_console(ushort mapno)
{
    asm volatile ("call ASM_load_mad_console\n"
        : : "a" (mapno));
}

void change_current_map(ushort mapno)
{
    cmdln_param_current_map = mapno;
    init_things();
    load_mad_console(mapno);
    fill_floor_textures();
}

void change_brightness(ushort val)
{
    asm volatile ("call ASM_change_brightness\n"
        : : "a" (val));
}

void traffic_unkn_func_01(void)
{
    asm volatile ("call ASM_traffic_unkn_func_01\n"
        :  :  : "eax" );
}

void process_engine_unk1(void)
{
    asm volatile ("call ASM_process_engine_unk1\n"
        :  :  : "eax" );
}

void process_sound_heap(void)
{
    asm volatile ("call ASM_process_sound_heap\n"
        :  :  : "eax" );
}

void func_2e440(void)
{
    asm volatile ("call ASM_func_2e440\n"
        :  :  : "eax" );
}

void func_cc0d4(char **str)
{
    asm volatile ("call ASM_func_cc0d4\n"
        : : "a" (str));
}

void init_outro(void)
{
#if 0
    asm volatile ("call ASM_init_outro\n"
        : : );
    return;
#else
    TbClockMSec last_loop_time;
    const char *fname;
    const char *text;
    int fh;
    int i;

    gamep_unknval_01 = 0;
    StopAllSamples();
    StopCD();
    show_black_screen();
    swap_wscreen();
    setup_screen_mode(Lb_SCREEN_MODE_320_200_8);
    LbMouseChangeSprite(0);
    lbKeyOn[KC_SPACE] = 0;
    lbKeyOn[KC_RETURN] = 0;
    lbKeyOn[KC_ESCAPE] = 0;
    if (background_type == 1)
        fname = "data/outro-z.smk";
    else
        fname = "data/outro-s.smk";
    play_smk(fname, 13, 0);
    data_155704 = -1;
    memset(lbDisplay.WScreen, 0, 320*200);

    switch (background_type)
    {
    case 0:
    default:
        fname = "data/outro-s.raw";
        break;
    case 1:
        fname = "data/outro-z.raw";
        break;
    case 2:
        fname = "data/outro-p.raw";
        break;
    }
    fh = LbFileOpen(fname, Lb_FILE_MODE_READ_ONLY);
    if (fh != -1)
    {
        for (i = 24; i != 174; i++)
        {
            ubyte *buf;
            buf = &lbDisplay.WScreen[i*320 + 10];
            LbFileRead(fh, buf, 300);
        }
    }
    LbFileClose(fh);

    LbFileLoadAt("qdata/pal.pal", display_palette);
    LbScreenWaitVbi();

    {
        char str[FILENAME_MAX];
        sprintf(str, "qdata/pal%d.dat", 0);
        LbFileLoadAt(str, display_palette);
        LbPaletteSet(display_palette);
    }
    swap_wscreen();
    screen_dark_curtain_down();
    init_things();
    change_current_map(51);
    load_outro_sprites();

    switch (background_type)
    {
    case 0:
    default:
        text = unk_credits_text_s;
        fname = outro_text_s;
        break;
    case 1:
        text = unk_credits_text_z;
        fname = outro_text_z;
        break;
    case 2:
        text = unk_credits_text_p;
        fname = outro_text_z;
        break;
    }
    func_cc638(text, fname);

    // Sleep for up to 10 seconds
    last_loop_time = LbTimerClock();
    for (i = 10*GAME_FPS; i != 0; i--)
    {
        if ( lbKeyOn[KC_SPACE] )
          break;
        if ( lbKeyOn[KC_ESCAPE] )
          break;
        if ( lbKeyOn[KC_RETURN] )
          break;
        TbClockMSec sleep_end = last_loop_time + 1000/GAME_FPS;
        LbSleepUntil(sleep_end);
        last_loop_time = LbTimerClock();
    }
    lbKeyOn[KC_SPACE] = 0;
    lbKeyOn[KC_ESCAPE] = 0;
    lbKeyOn[KC_RETURN] = 0;

    LbPaletteFade(0, 0xC8u, 1);
    memset(lbDisplay.WScreen, 0, 320*200);
    swap_wscreen();
    StopAllSamples();
    reset_heaps();
    setup_heaps(100);
    play_sample_using_heap(0, 1, 127, 64, 100, -1, 3u);

    data_197150 = 1;
    data_1dd91c = 0;
    unkn_flags_01 = 1;
    overall_scale = 40;
    palette_brightness = 0;
    change_brightness(-64);

    for (i = 0; i < 128; i++)
    {
        if (i & 1)
          change_brightness(1);
        traffic_unkn_func_01();
        process_engine_unk1();
        process_sound_heap();
        func_2e440();
        swap_wscreen();
        memset(lbDisplay.WScreen, 0, lbDisplay.PhysicalScreenHeight * lbDisplay.PhysicalScreenWidth);
    }

    while (1)
    {
        if ( lbKeyOn[KC_SPACE] )
          break;
        if ( lbKeyOn[KC_ESCAPE] )
          break;
        if ( lbKeyOn[KC_RETURN] )
          break;
        gameturn++;
        traffic_unkn_func_01();
        process_engine_unk1();
        if ( !(LbRandomAnyShort() & 0xF) && (data_155704 == -1 || !IsSamplePlaying(0, data_155704, 0)) )
        {
            play_sample_using_heap(0, 7 + (LbRandomAnyShort() % 5), 127, 64, 100, 0, 3u);
        }
        process_sound_heap();
        func_2e440();
        if (outro_unkn01)
        {
            outro_unkn02++;
            func_cc0d4((char **)&people_credits_groups[2 * outro_unkn03]);
            if (data_1ddb68 + 50 < outro_unkn02)
            {
              outro_unkn02 = 0;
              if (++outro_unkn03 == people_groups_count)
                  outro_unkn03 = 0;
            }
          }
          swap_wscreen();
          memset(lbDisplay.WScreen, 0, lbDisplay.PhysicalScreenHeight * lbDisplay.PhysicalScreenWidth);
    }
    StopAllSamples();
    reset_heaps();
    setup_heaps(2);
#endif
}

int setup_host_sub5(BuffUnknStruct02 *a1)
{
    int ret;
    asm volatile ("call ASM_setup_host_sub5\n"
        : "=r" (ret) : "a" (a1));
    return ret;
}

int LoadSounds(unsigned char a1)
{
    int ret;
    asm volatile ("call ASM_LoadSounds\n"
        : "=r" (ret) : "a" (a1));
    return ret;
}

int LoadMusic(unsigned char a1)
{
    int ret;
    asm volatile ("call ASM_LoadMusic\n"
        : "=r" (ret) : "a" (a1));
    return ret;
}

int alt_at_point(ushort x, ushort z)
{
    int ret;
    asm volatile ("call ASM_alt_at_point\n"
        : "=r" (ret) : "a" (x), "d" (z));
    return ret;
}

void load_pop_sprites_lo(void)
{
    asm volatile ("call ASM_load_pop_sprites_lo\n"
        :  :  : "eax" );
}

void load_pop_sprites_hi(void)
{
    asm volatile ("call ASM_load_pop_sprites_hi\n"
        :  :  : "eax" );
}

void video_mode_switch_to_next(void)
{
    int i;
    if (lbDisplay.ScreenMode == Lb_SCREEN_MODE_320_200_8)
    {
        StopCD();
        setup_screen_mode(Lb_SCREEN_MODE_640_480_8);
        overall_scale = 400;
        load_pop_sprites_hi();
        render_area_a = 30;
        render_area_b = 30;
        ingame__Scanner.X1 = 1;
        game_high_resolution = 1;
        ingame__Scanner.Y1 = 341;
        ingame__Scanner.Y2 = 460;
        ingame__Scanner.X2 = 130;

        for (i = 0; i + ingame__Scanner.Y1 <= ingame__Scanner.Y2; i++)
        {
          ingame__Scanner.Width[i] = min(105 + i, 129);
        }
    }
    else
    {
        StopCD();
        setup_screen_mode(Lb_SCREEN_MODE_320_200_8);
        load_pop_sprites_lo();
        overall_scale = 150;
        render_area_a = 24;
        render_area_b = 24;
        ingame__Scanner.X1 = 1;
        ingame__Scanner.Y1 = 127;
        game_high_resolution = 0;
        ingame__Scanner.X2 = 65;
        ingame__Scanner.Y2 = 189;

        for (i = 0; i + ingame__Scanner.Y1 <= ingame__Scanner.Y2; i++)
        {
          ingame__Scanner.Width[i] = min(40 + i, 64);
        }
    }
}

void teleport_current_agent(PlayerInfo *p_locplayer)
{
    int dctrl_thing;
    dctrl_thing = p_locplayer->DirectControl[mouser];
    delete_node(&things[dctrl_thing]);
    things[dctrl_thing].X = mouse_map_x << 8;
    things[dctrl_thing].Z = mouse_map_z << 8;
    things[dctrl_thing].Y = alt_at_point(mouse_map_x, mouse_map_z);
    add_node_thing(dctrl_thing);
}

void person_resurrect(struct Thing *p_person)
{
    ulong person_anim;
    p_person->Flag &= ~0x0002;
    p_person->Flag &= ~0x02000000;
    p_person->State = 5;
    p_person->U.UPerson.AnimMode = 1;
    person_anim = people_frames[p_person->SubType][p_person->U.UPerson.AnimMode];
    p_person->StartFrame = person_anim - 1;
    p_person->Frame = nstart_ani[person_anim + p_person->U.UPerson.Angle];
}

void person_give_all_weapons(struct Thing *p_person)
{
    p_person->U.UPerson.WeaponsCarried = 0x3FFBDFFF;
    do_weapon_quantities1(p_person);
}

void person_give_best_mods(struct Thing *p_person)
{
    p_person->U.UPerson.UMod.Mods &= ~0x0007;
    p_person->U.UPerson.UMod.Mods |= 0x0003;
    p_person->U.UPerson.UMod.Mods &= ~0x0038;
    p_person->U.UPerson.UMod.Mods |= 0x0018;
    p_person->U.UPerson.UMod.Mods &= ~0x0E00;
    p_person->U.UPerson.UMod.Mods |= 0x0600;
    p_person->U.UPerson.UMod.Mods &= ~0x01C0;
    p_person->U.UPerson.UMod.Mods |= 0x00C0;
}

void beefup_all_agents(PlayerInfo *p_locplayer)
{
    int i;
    for (i = 0; i < playable_agents; i++)
    {
        struct Thing *p_agent;
        p_agent = p_locplayer->MyAgent[i];
        if (p_agent->Flag & 0x0002)
            person_resurrect(p_agent);
        person_give_all_weapons(p_agent);
        if (lbShift & KMod_SHIFT)
        {
            person_give_best_mods(p_agent);

            p_agent->Health = 32000;
            p_agent->U.UPerson.MaxHealth = 32000;
            p_agent->U.UPerson.Energy = 32000;
            p_agent->U.UPerson.MaxEnergy = 32000;
        }
    }
    research.WeaponsCompleted = 0x3FFFFFFF;
}

void game_graphics_inputs(void)
{
#if 0
    asm volatile ("call ASM_game_graphics_inputs\n"
        : : );
    return;
#else
    PlayerInfo *p_locplayer;

    p_locplayer = &players[local_player_no];
    if (ingame__DisplayMode != 50 && ingame__DisplayMode != 59)
        return;
    if (in_network_game && p_locplayer->PanelState[mouser] != 17)
        return;

    if (lbKeyOn[KC_F] && (lbShift == KMod_NONE))
    {
        lbKeyOn[KC_F] = 0;
        if (game_perspective == 5)
            game_perspective = 0;
        else
            game_perspective = 5;
    }

    if ((ingame__Cheats & 0x04) && lbKeyOn[KC_T] && (lbShift & KMod_ALT))
    {
        lbKeyOn[KC_T] = 0;
        teleport_current_agent(p_locplayer);
    }

    if ((ingame__Cheats & 0x04) && !in_network_game)
    {
        if (lbKeyOn[KC_Q] && (lbShift == KMod_SHIFT || lbShift == KMod_NONE))
        {
            lbKeyOn[KC_Q] = 0;
            beefup_all_agents(p_locplayer);
        }
    }

    if (lbKeyOn[KC_F8])
    {
        lbKeyOn[KC_F8] = 0;
        video_mode_switch_to_next();
    }
#endif
}

void init_syndwars(void)
{
    ASM_init_syndwars();
}

void setup_host_sub6(void)
{
    ASM_setup_host_sub6();
}

int setup_mele(void)
{
    return ASM_setup_mele();
}

void set_smack_malloc(void *(*cb)(int))
{
    smack_malloc = cb;
}

void set_smack_free(void (*cb)(void *ptr))
{
    smack_free = cb;
}

void BAT_unknsub_20(int a1, int a2, int a3, int a4, unsigned long a5)
{
    asm volatile (
      "push %4\n"
      "call ASM_BAT_unknsub_20\n"
        : : "a" (a1), "d" (a2), "b" (a3), "c" (a4), "g" (a5));
}

void setup_host(void)
{
    //ASM_setup_host(); return;
    char fname[DISKPATH_SIZE];
    BAT_unknsub_20(0, 0, 0, 0, unkn_buffer_04 + 41024);
    set_smack_malloc(ASM_smack_malloc);
    set_smack_free(ASM_smack_mfree);
    DEBUGLOG(0,"&setup_host() = 0x%lx", (ulong)setup_host);
    lbDisplay.ScreenMode = Lb_SCREEN_MODE_320_200_8;
    LbScreenSetup(lbDisplay.ScreenMode, 320, 200, display_palette);
    LbSpriteSetup(pointer_sprites, pointer_sprites_end, pointer_data);
    { // Make mouse pointer sprite 1 an empty (zero size) sprite
        struct TbSprite *spr;
        spr = &pointer_sprites[1];
        spr->SWidth = 0;
        spr->SHeight = 0;
    }
    if ( cmdln_param_d )
        LbKeyboardOpen();
    else
        LbIKeyboardOpen();
    init_buffered_keys();
    lbMouseAutoReset = false;
    LbMouseSetup(&pointer_sprites[1], 2, 2);

    setup_mele();
    LbSpriteSetup(m_sprites, m_sprites_end, m_spr_data);
    ingame__PanelPermutation = -2;
    {
        int file_len;
        sprintf(fname, pop_dat_fname_fmt, 1);
        LbFileLoadAt(fname, pop1_data);
        sprintf(fname, pop_tab_fname_fmt, -ingame__PanelPermutation - 1);
        file_len = LbFileLoadAt(fname, pop1_sprites);
        pop1_sprites_end = &pop1_sprites[file_len/sizeof(struct TbSprite)];
        LbSpriteSetup(pop1_sprites, pop1_sprites_end, pop1_data);
    }
    ingame__TrenchcoatPreference = 0;
    game_panel = game_panel_lo;
    LbGhostTableGenerate(display_palette, 50, "data/synghost.tab");
    setup_host_sub5(buffer_allocs);
    init_syndwars();
    LoadSounds(0);
    LoadMusic(0);
    setup_host_sub6();
    if ( pktrec_mode == 1 )
    {
      if ( !in_network_game )
      {
          int file_no;
          file_no = get_new_packet_record_no(selected_map_index);
          get_packet_record_fname(fname, selected_map_index, file_no+1);
          DEBUGLOG(0,"%s: Opening for packet save", fname);
          packet_rec_fh = LbFileOpen(fname, Lb_FILE_MODE_NEW);
          LbFileWrite(packet_rec_fh, &cmdln_param_current_map, 2);
      }
    }
    if ( pktrec_mode == 2 )
    {
        ushort pktrec_head;
        get_packet_record_fname(fname, selected_map_index, cmdln_pr_num);
        DEBUGLOG(0,"%s: Opening for packet input", fname);
        packet_rec_fh = LbFileOpen(fname, Lb_FILE_MODE_READ_ONLY);
        LbFileRead(packet_rec_fh, &pktrec_head, sizeof(pktrec_head));
    }
    play_intro();
    if ( cmdln_param_bcg )
    {
        lbDisplay.ScreenMode = Lb_SCREEN_MODE_640_480_8;
        LbScreenSetup(lbDisplay.ScreenMode, 640, 480, display_palette);
    }
    LbMouseSetup(&pointer_sprites[1], 2, 2);
    if ( cmdln_param_bcg )
      LbMouseChangeSprite(NULL);
}

void read_user_settings(void)
{
    ASM_read_user_settings();
}

void setup_color_lookups(void)
{
    ASM_setup_color_lookups();
}

void game_setup_sub8(void)
{
    ASM_game_setup_sub8();
}

void load_mission_file(int num)
{
    asm volatile ("call ASM_load_mission_file\n"
        : : "a" (num));
}

void init_engine(void)
{
    ASM_init_engine();
}

void load_mission_map_lvl(unsigned char num)
{
    asm volatile ("call ASM_load_mission_map_lvl\n"
        : : "a" (num));
}

char *gui_strings_data_end;

TbBool create_strings_list(char **strings, char *strings_data, char *strings_data_end)
{
  int text_idx;
  char *text_ptr;
  char **text_arr;
  text_arr = strings;
  text_idx = STRINGS_MAX;
  text_ptr = strings_data;
  while (text_idx >= 0)
  {
      if (text_ptr >= strings_data_end) {
          break;
      }
      *text_arr = text_ptr;
      text_arr++;
      char chr_prev;
      do {
          chr_prev = *text_ptr;
          text_ptr++;
      } while ((chr_prev != '\0') && (text_ptr < strings_data_end));
      text_idx--;
  }
  return (text_idx < STRINGS_MAX);
}

void create_tables_file(void)
{
    int i, k;
    unsigned char *curbuf;
    LbFileLoadAt(fadedat_fname, fade_data);
    curbuf = fade_data;
    for (i = 0; i < 256; i++) {
        for (k = 0; k < 64; k++) {
          fade_table[256 * k + i] = *(curbuf + 320 * k + i);
        }
    }
    fade_data = curbuf;
    LbFileSaveAt("data/tables.dat", fade_table, 84224);
}

void game_setup(void)
{
    //ASM_game_setup();return;

    engine_mem_alloc_ptr = LbMemoryAlloc(engine_mem_alloc_size);
    load_texturemaps();
    LbDataLoadAll(unk02_load_files);
    game_setup_sub1();
    init_arrays_1();
    game_setup_sub2(0);
    game_setup_sub3();
    ingame__draw_unknprop_01 = 0;
    game_setup_sub4(-4);
    game_setup_sub5();
    create_strings_list(gui_strings, gui_strings_data, gui_strings_data_end);
    setup_host();
    read_user_settings();
    game_setup_sub4(-3);
    setup_color_lookups();
    init_things();
    game_setup_sub4(-2);
    LbSpriteSetup(small_font, small_font_end, small_font_data);
    game_setup_sub8();
    load_mission_file(0);
    players[local_player_no].MissionAgents = 0x0f;
    game_setup_sub4(-1);
    if ( is_single_game || cmdln_param_bcg )
    {
        load_prim_quad();
    }
    game_setup_sub4(0);
    init_engine();
    if ( !cmdln_param_bcg )
    {
        LbMemorySet(lbDisplay.WScreen, 0, lbDisplay.PhysicalScreenWidth * lbDisplay.PhysicalScreenHeight);
        swap_wscreen();
        LbPaletteSet(display_palette);
    }
    test_open(15);
    game_setup_sub4(1);
    if ( is_single_game && cmdln_param_current_map )
      load_mission_map_lvl(0);
    if ( in_network_game || cmdln_param_bcg )
      ingame__DisplayMode = 55;
    game_setup_sub4(2);
    if ( cmdln_param_tf == 2 )
    {
        create_tables_file();
    }
}

void game_process_sub01(void)
{
    unsigned long tick_time = clock();
    tick_time = tick_time / 100;
    curr_tick_time = tick_time;
    if (tick_time != prev_tick_time)
    {
        unsigned long tmp;
        tmp = gameturn - prev_gameturn;
        prev_gameturn = gameturn;
        turns_delta = tmp;
    }
    if ( turns_delta != 0 ) {
        fifties_per_gameturn = 800 / turns_delta;
    } else {
        fifties_per_gameturn = 50;
    }
    if ( in_network_game )
        fifties_per_gameturn = 80;
    if ( fifties_per_gameturn > 400 )
        fifties_per_gameturn = 400;
    prev_tick_time = curr_tick_time;
}

void game_process_sub08(void);

void game_process_sub09(void)
{
    int i;
    switch ( gamep_unknval_01 )
    {
    case 1:
        game_process_sub08();
        break;
    case 2:
        for (i = 0; i < 10; i++) {
            ushort pos;
            uint8_t *ptr;
            pos = LbRandomAnyShort() + (gameturn >> 2);
            ptr = &vec_tmap[pos];
            *ptr = fade_table[0x2800 + *ptr];
        }
        break;
    }
}

void debug_m_sprite(int idx)
{
    int i;
    char strdata[100];
    char *str;
    struct TbSprite *spr;
    unsigned char *ptr;
    spr = &m_sprites[idx];
    str = strdata;
    sprintf(str, "spr %d width %d height %d ptr 0x%lx data",
      idx, (int)spr->SWidth, (int)spr->SHeight, (ulong)spr->Data);
    ptr = spr->Data;
    for (i = 0; i < 10; i++)
    {
        str = strdata + strlen(strdata);
        sprintf(str, " %02x", (int)*ptr);
        ptr++;
    }
    DEBUGLOG(0,"m_sprites: %s", strdata);
}

void mapwho_unkn01(int a1, int a2)
{
    asm volatile ("call ASM_mapwho_unkn01\n"
        : : "a" (a1), "d" (a2));
}

void new_bang(int a1, int a2, int a3, int a4, int a5, int a6)
{
    asm volatile (
      "push %5\n"
      "push %4\n"
      "call ASM_new_bang\n"
        : : "a" (a1), "d" (a2), "b" (a3), "c" (a4), "g" (a5), "g" (a6));
}

void new_bang_3(int a1, int a2, int a3, int a4)
{
    new_bang(a1, a2, a3, a4, 0, 0);
}

void input(void)
{
    uint16_t n;
    n = lbShift;
    if ( lbKeyOn[KC_LSHIFT] || lbKeyOn[KC_RSHIFT] )
        n |= KMod_SHIFT;
    else
        n &= ~KMod_SHIFT;
    if ( lbKeyOn[KC_LCONTROL] || lbKeyOn[KC_RCONTROL] )
        n |= KMod_CONTROL;
    else
        n &= ~KMod_CONTROL;
    if ( lbKeyOn[KC_RALT] || lbKeyOn[KC_LALT] )
        n |= KMod_ALT;
    else
        n &= ~KMod_ALT;
    lbShift = n;
}

short PlayCDTrack(int a1)
{
    int ret;
    asm volatile ("call ASM_PlayCDTrack\n"
        : "=r" (ret) : "a" (a1));
    return ret;
}

void gproc3_unknsub3(int a1)
{
    // Empty
}

void ASM_show_game_engine(void);
void show_game_engine(void)
{
    ASM_show_game_engine();
}

void ASM_gproc3_unknsub2(void);
void gproc3_unknsub2(void)
{
    ASM_gproc3_unknsub2();
}

void ASM_BAT_play(void);
void BAT_play(void)
{
    ASM_BAT_play();
}

TbResult init_read_all_sprite_files(void)
{
    TbResult ret;
    asm volatile ("call ASM_init_read_all_sprite_files\n"
        : "=r" (ret) : );
    return ret;
}

TbPixel LbPaletteFindColour(ubyte *pal, ubyte rval, ubyte gval, ubyte bval)
{
    TbResult ret;
    asm volatile ("call ASM_LbPaletteFindColour\n"
        : "=r" (ret) : "a" (pal), "d" (rval), "b" (gval), "c" (bval));
    return ret;
}

void init_screen_boxes(void)
{
    asm volatile ("call ASM_init_screen_boxes\n"
        :  :  : "eax" );
}

void update_menus(void)
{
    asm volatile ("call ASM_update_menus\n"
        :  :  : "eax" );
}

void load_city_txt(void)
{
    asm volatile ("call ASM_load_city_txt\n"
        :  :  : "eax" );
}

void load_city_data(ubyte type)
{
    TbFileHandle handle;

    handle = LbFileOpen("data/cities.dat", Lb_FILE_MODE_READ_ONLY);
    if (handle != (TbFileHandle)-1)
    {
        LbFileRead(handle, &num_cities, 1);
        LbFileRead(handle, cities, sizeof(struct City) * num_cities);
        LbFileClose(handle);
    }
}

void reload_background(void)
{
    asm volatile ("call ASM_reload_background\n"
        :  :  : "eax" );
}

void players_init_control_mode(void)
{
    int player;
    for (player = 0; player < 8; player++) {
      players[player].UserInput[0].ControlMode = 1;
    }
}

void srm_scanner_reset(void)
{
    int i;

    ingame__Scanner.X1 = 8;
    ingame__Scanner.Y2 = 270;
    ingame__Scanner.MX = 127;
    ingame__Scanner.MZ = 127;
    ingame__Scanner.Angle = 0;
    ingame__Scanner.X2 = 325;
    ingame__Scanner.Zoom = 256;
    ingame__Scanner.Y1 = 73;

    for (i = 0; i + ingame__Scanner.Y1 <= ingame__Scanner.Y2; i++)
    {
        ingame__Scanner.Width[i] = 318;
    }
    for (i = 0; i != 16; i++)
    {
        ingame__Scanner.BigBlip[i].Period = 0;
    }
    SCANNER_width = ingame__Scanner.Width;
    SCANNER_init();
}

void show_menu_screen_st0(void)
{
    debug_trace_place(16);

    lbInkeyToAscii[KC_OEM_102] = '\\';
    lbInkeyToAsciiShift[KC_OEM_102] = '|';

    players_init_control_mode();

    login_control__State = 6;
    sprintf(net_unkn2_text, "01234567890");

    hotspot_buffer = scratch_malloc_mem;
    mission_briefing_text = (char *)scratch_malloc_mem + 512;
    mem_unkn03 = (char *)scratch_malloc_mem + 16896;
    weapon_text = (char *)scratch_malloc_mem + 33280;
    memload = (ubyte *)scratch_malloc_mem + 66048;
    purple_draw_list = (struct PurpleDrawItem *)((ubyte *)scratch_malloc_mem + 82432);

    init_read_all_sprite_files();
    ingame__Credits = 50000;

    debug_trace_place(17);
    LbFileLoadAt("data/bgtables.dat", &fade_table);
    LbGhostTableGenerate(display_palette, 66, "data/startgho.dat");
    init_screen_boxes();
    update_menus();
    players[local_player_no].MissionAgents = 0x0f;
    load_city_data(0);
    load_city_txt();

    debug_trace_place(18);
    if ( in_network_game )
      screentype = SCRT_PAUSE;
    else
      screentype = SCRT_MAINMENU;
    data_1c498d = 1;

    debug_trace_place(19);
    LbFileLoadAt("data/s-proj.pal", display_palette);
    show_black_screen();
    show_black_screen();
    LbPaletteSet(display_palette);
    reload_background();

#if 0
    global_date.Day = 2;
    global_date.Year = 74;
    global_date.Month = 6;

    srm_scanner_reset();

    save_game_buffer = unkn_buffer_05;

    LbNetworkSetSessionCreateFunction(my_net_session_callback);
    LbNetworkSetSessionJoinFunction(my_net_session_callback);
    LbNetworkSetSessionUnk2CFunction(my_net_session_callback);
    LbNetworkSetSessionInitFunction(my_net_session_callback);
    LbNetworkSetSessionDialFunction(my_net_session_callback);
    LbNetworkSetSessionAnswerFunction(my_net_session_callback);
    LbNetworkSetSessionHangUpFunction(my_net_session_callback);
    if ( LbNetworkReadConfig("modem.cfg") != -1 )
      byte_1C4A70 = 1;
#endif
}

void ASM_show_menu_screen(void);
void show_menu_screen(void)
{
    ASM_show_menu_screen();
}

void draw_game(void)
{
    //ASM_draw_game(); return;
    switch (ingame__DisplayMode)
    {
    case 1:
        // No action
        break;
    case 50:
        PlayCDTrack(game_music_track);
        if ( !(ingame__Flags & 0x20) || !(gameturn & 0xF) )
        {
            show_game_engine();
            if ( ingame__Flags & 0x800 )
              gproc3_unknsub2();
            BAT_play();
            if ( execute_commands )
            {
                long tmp;
                gamep_unknval_16 = gamep_unknval_13;
                gamep_unknval_13 = 0;
                ++gamep_unknval_12;
                gamep_unknval_10 += gamep_unknval_16;
                gamep_unknval_15 = gamep_unknval_14;
                tmp = gamep_unknval_14 + gamep_unknval_11;
                gamep_unknval_14 = 0;
                gamep_unknval_11 = tmp;
            }
        }
        break;
    case 55:
        show_menu_screen();
        break;
    case 58:
        gproc3_unknsub3(0);
        break;
    default:
        ERRORLOG("DisplayMode %d empty\n", (int)ingame__DisplayMode);
        break;
    }
}

void load_packet(void);
void game_process_sub02(void);
void process_packets(void);
void joy_input(void);
void game_process_sub04(void);
void process_packets(void);

void game_process(void)
{
    debug_m_sprite(193);
    DEBUGLOG(0,"WSCREEN 0x%lx", (ulong)lbDisplay.WScreen);
    while ( !exit_game )
    {
      process_sound_heap();
      if ( lbKeyOn[KC_F8] && (lbShift & KMod_CONTROL))
      {
          lbKeyOn[KC_F8] = 0;
          LbScreenSetup(lbDisplay.ScreenMode, 640, 480, display_palette);
      }
      navi2_unkn_counter -= 2;
      if (navi2_unkn_counter < 0)
          navi2_unkn_counter = 0;
      if (navi2_unkn_counter > navi2_unkn_counter_max)
          navi2_unkn_counter_max = navi2_unkn_counter;
      if (cmdln_param_d)
          input_char = LbKeyboard();
      if (ingame__DisplayMode == 55)
          DEBUGLOG(0,"id=%d  trial alloc = %d turn %lu", 0, triangulation, gameturn);
      if (!LbScreenIsLocked()) {
          while (LbScreenLock() != Lb_SUCCESS)
              ;
      }
      input();
      game_process_sub01();
      draw_game();
      game_setup_sub4(gameturn + 100);
      load_packet();
      if ( ((active_flags_general_unkn01 & 0x8000) != 0) !=
        ((ingame__Flags & 0x8000) != 0) )
          LbPaletteSet(display_palette);
      active_flags_general_unkn01 = ingame__Flags;
      if ( ingame__DisplayMode == 50 || ingame__DisplayMode == 1 || ingame__DisplayMode == 59 )
          game_process_sub02();
      if ( ingame__DisplayMode != 55 )
          process_packets();
      joy_input();
      if ( ingame__DisplayMode == 55 ) {
          swap_wscreen();
      }
      else if ( !(ingame__Flags & 0x20) || ((gameturn & 0xF) == 0) ) {
          LbScreenSwapClear(0);
      }
      game_process_sub04();
      if ( unkn01_downcount > 0 ) /* orbital station explosion code */
      {
        unkn01_downcount--;
        DEBUGLOG(0,"unkn01_downcount = %ld", unkn01_downcount);
        if ( unkn01_downcount == 40 ) {
            mapwho_unkn01(unkn01_pos_x, unkn01_pos_y);
        }
        else if ( unkn01_downcount < 40 ) {
            unsigned short stl_y;
            unsigned short stl_x;
            stl_y = unkn01_pos_y + (LbRandomAnyShort() & 0xF) - 7;
            stl_x = unkn01_pos_x + (LbRandomAnyShort() & 0xF) - 7;
            new_bang_3(stl_x << 16, 0, stl_y << 16, 95);
            stl_y = unkn01_pos_y + (LbRandomAnyShort() & 0xF) - 7;
            stl_x = unkn01_pos_x + (LbRandomAnyShort() & 0xF) - 7;
            new_bang_3(stl_x << 16, 0, stl_y << 16, 95);
        }
      }
      gameturn++;
      game_process_sub09();
    }
    PacketRecord_Close();
    LbPaletteFade(NULL, 0x10u, 1);
}

void
game_quit(void)
{
    sound_finalise ();
    SDL_Quit ();
    exit (0);
}

void
game_transform_path_full(const char *file_name, char *buffer, size_t size)
{
    if (strncasecmp (file_name, SAVEGAME_PATH,
             sizeof (SAVEGAME_PATH) - 1) == 0)
    {
        snprintf (buffer, size, "%s" FS_SEP_STR "%s", GetDirectoryUser(),
          file_name + sizeof (SAVEGAME_PATH) - 1);
        return;
    }

    /* abort on absolute paths */
    if (file_name[0] == '\\' || file_name[0] == '/'
        || (strlen (file_name) >= 2 && file_name[1] == ':'))
    {
        snprintf (buffer, size, "%s", file_name);
        return;
    }

    snprintf (buffer, size, "%s" FS_SEP_STR "%s", GetDirectoryHdd(), file_name);
}

void
game_transform_path(const char *file_name, char *result)
{
    game_transform_path_full (file_name, result, FILENAME_MAX);
}

void
game_play_music(void)
{
    char file_name[FILENAME_MAX];

    snprintf (file_name, sizeof (file_name),
          "%s" FS_SEP_STR "music" FS_SEP_STR "track_%i.ogg",
          GetDirectoryHdd(), game_music_track - 1);

    sound_open_music_file (file_name);
    sound_play_music ();
}

static void
game_update_full(bool wait)
{
    const int max_fps = 16;
    const int32_t frame_duration = 1000 / max_fps;

    static int32_t last_frame_ticks;
    int32_t start_ticks;

    display_unlock();

    sound_update();
    game_handle_sdl_events();

    start_ticks = SDL_GetTicks();

    if (wait && last_frame_ticks != 0)
    {
        int32_t last_frame_duration;

        last_frame_duration = (int32_t)(start_ticks - last_frame_ticks + 2);

        if (last_frame_duration < frame_duration)
        {
            int32_t total_sleep_time = frame_duration - last_frame_duration;
            const int32_t min_sleep_time = 1000 / 40;
            const int32_t max_sleep_time = 1000 / 20;

            total_sleep_time = frame_duration - last_frame_duration;

            if (total_sleep_time > 0)
            {
                float f = (float) total_sleep_time
                    * (min_sleep_time + max_sleep_time)
                    / (2 * min_sleep_time * max_sleep_time);
                int32_t base_sleep_time = (int32_t)(total_sleep_time / f + .5f);

                while (total_sleep_time > 0)
                {
                    int32_t sleep_time = MIN(base_sleep_time, total_sleep_time);
                    int32_t ticks = SDL_GetTicks();

                    SDL_Delay(sleep_time);

                    display_lock();
                    game_handle_sdl_events();
                    sound_update();
                    display_unlock();

                    total_sleep_time -= SDL_GetTicks() - ticks;
                }
            }
        }
    }

    last_frame_ticks = SDL_GetTicks();

    display_lock();
}

int
LbScreenWaitVbi(void)
{
    game_update_full(false);
    return 1;
}

void
game_update(void)
{
    game_update_full(true);
}

void host_reset(void)
{
    asm volatile ("call ASM_host_reset\n"
        :  :  : "eax" );
}

void
game_reset(void)
{
    host_reset();
    free_texturemaps();
    LbDataFreeAll(unk02_load_files);
}

