#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "bflib_fileio.h"
#include "bflib_video.h"
#include "bflib_filelst.h"
#include "bflib_memory.h"
#include "bflib_sprite.h"
#include "bflib_video.h"
#include "bflib_dernc.h"
#include "bflib_keybrd.h"
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

#include "timer.h"

#define SAVEGAME_PATH "qdata/savegame/"

extern uint8_t game_music_track;

extern struct TbLoadFiles unk02_load_files[15];

unsigned int LbRandomAnyShort(void);

int LbPaletteFade(uint8_t *a1, uint8_t a2, uint8_t a3)
{
    int ret;
    asm volatile ("call ASM_LbPaletteFade\n"
        : "=r" (ret) : "a" (a1), "d" (a2), "b" (a3));
    return ret;
}

void PacketRecord_Close(void);

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

    keyboard_initialise();
    mouse_initialise();
    display_initialise();
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
        if ( cmdln_param_mp || cmdln_param_bcg )
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
    if ( !cmdln_param_mp )
        cmdln_param_bcg = 1;

    return true;
}

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
            mouse_handle_event(&ev);
            break;

        case SDL_KEYUP:
        case SDL_KEYDOWN:
            keyboard_handle_event(&ev);
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
void ASM_game_setup_sub7(void);
void ASM_game_setup_sub8(void);
void ASM_setup_drawlist(void);
void ASM_swap_wscreen(void);

extern unsigned char *fade_data;
extern char *fadedat_fname;
extern unsigned short draw_unknprop_01;
extern struct TbSprite *font0_sprites;
extern struct TbSprite *font0_sprites_end;
extern unsigned char *font0_data;
extern unsigned short displaymode;
extern unsigned char *display_palette;
extern unsigned short data_1c8406;
extern unsigned short data_1c8408;
extern unsigned short data_1c840a;
extern int data_1c8428;
extern unsigned short data_197224;
extern const char *primvehobj_fname;
extern unsigned char data_19ec6f;
extern unsigned char textwalk_data[640];
extern TbPixel _fade_table[256*64];

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
    if ( handle != -1 )
    {
        LbFileRead(handle, textwalk_data, 640);
        LbFileClose(handle);
    }
}

void read_primveh_obj(const char *fname, int a2)
{
    asm volatile ("call ASM_read_primveh_obj\n"
        : : "a" (fname), "d" (a2));
}

void load_prim_quad(void)
{
    data_1c8406 = 64;
    data_1c8408 = 64;
    data_1c840a = 64;
    data_1c8428 = 0;
    data_197224 = 1000;
    read_primveh_obj(primvehobj_fname, 1);
    read_textwalk();
    data_19ec6f = 1;
    displaymode = 55;
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
    asm volatile ("call ASM_game_setup_sub4\n"
        : : "a" (a1));
}

void game_setup_sub5(void)
{
    ASM_game_setup_sub5();
}

void setup_host(void)
{
    ASM_setup_host();
}

void read_user_settings(void)
{
    ASM_read_user_settings();
}

void setup_color_lookups(void)
{
    ASM_setup_color_lookups();
}

void game_setup_sub7(void)
{
    ASM_game_setup_sub7();
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

void setup_drawlist(void)
{
    ASM_setup_drawlist();
}

void load_mission_map_lvl(unsigned char num)
{
    asm volatile ("call ASM_load_mission_map_lvl\n"
        : : "a" (num));
}

void swap_wscreen(void)
{
    ASM_swap_wscreen();
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
          _fade_table[256 * k + i] = *(curbuf + 320 * k + i);
        }
    }
    fade_data = curbuf;
    LbFileSaveAt("data/tables.dat", _fade_table, 14900);
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
    draw_unknprop_01 = 0;
    game_setup_sub4(-4);
    game_setup_sub5();
    create_strings_list(gui_strings, gui_strings_data, gui_strings_data_end);
    setup_host();
    read_user_settings();
    game_setup_sub4(-3);
    setup_color_lookups();
    game_setup_sub7();
    game_setup_sub4(-2);
    LbSpriteSetup(font0_sprites, font0_sprites_end, font0_data);
    game_setup_sub8();
    load_mission_file(0);
    players[local_player_no].field_BB = 15;
    game_setup_sub4(-1);
    if ( cmdln_param_mp || cmdln_param_bcg )
    {
        load_prim_quad();
    }
    game_setup_sub4(0);
    setup_drawlist();
    if ( !cmdln_param_bcg )
    {
        LbMemorySet(lbDisplay.WScreen, 0, lbDisplay.PhysicalScreenWidth * lbDisplay.PhysicalScreenHeight);
        swap_wscreen();
        LbPaletteSet(display_palette);
    }
    test_open(15);
    game_setup_sub4(1);
    if ( cmdln_param_mp && cmdln_param_map_index )
      load_mission_map_lvl(0);
    if ( in_network_game || cmdln_param_bcg )
      displaymode = 55;
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
        msecs_per_frame = 800 / turns_delta;
    } else {
        msecs_per_frame = 50;
    }
    if ( in_network_game )
        msecs_per_frame = 80;
    if ( msecs_per_frame > 400 )
        msecs_per_frame = 400;
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
            *ptr = unknoise_tmap[*ptr];
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
    sprintf(str, "m_sprites: spr %d width %d height %d ptr %lx ",
      idx, (int)spr->SWidth, (int)spr->SHeight, (ulong)spr->Data);
    ptr = spr->Data;
    for (i = 0; i < 10; i++)
    {
        str = strdata + strlen(strdata);
        sprintf(str, " %d ", (int)*ptr);
        ptr++;
    }
    DEBUGLOG(0,"%s", str);
}

void process_sound_heap(void);
void input(void);
void game_process_sub01(void);
void game_process_sub03(void);

void game_process(void)
{
    debug_m_sprite(193);
    DEBUGLOG(0,"WSCREEN 0x%lx", (ulong)lbDisplay.WScreen);
    ASM_game_process();//FIXME get rid of this
    /*
    while ( !exit_game )
    {
      process_sound_heap();
      if ( lbKeyOn[66] && lbShift & 2 )
      {
          lbKeyOn[66] = 0;
          LbScreenSetup(lbDisplay.ScreenMode, 640, 480, display_palette);
      }
      navi2_unkn_counter -= 2;
      if (navi2_unkn_counter < 0)
          navi2_unkn_counter = 0;
      if (navi2_unkn_counter > navi2_unkn_counter_max)
          navi2_unkn_counter_max = navi2_unkn_counter;
      if (cmdln_param_d)
          input_char = LbKeyboard();
      if (displaymode == 55)
          DEBUGLOG(0,"id=%d  trial alloc = %d turn %lu", 0, trial_alloc, gameturn);
      input();
      game_process_sub01();
      game_process_sub03();
      game_setup_sub4(gameturn + 100);
      load_packet();
      if ( ((dword_1A7324 & 0x8000) != 0) != ((flags_general_unkn01 & 0x8000) != 0) )
          LbPaletteSet(v10);
      dword_1A7324 = flags_general_unkn01;
      if ( displaymode == 50 || displaymode == 1 || displaymode == 59 )
          game_process_sub02();
      if ( displaymode != 55 )
          process_packets();
      joy_input();
      if ( displaymode == 55 ) {
          swap_wscreen();
      }
      else if ( !(flags_general_unkn01 & 0x20) || ((gameturn & 0xF) == 0) ) {
          LbScreenSwapClear(0);
      }
      game_process_sub04();
      if ( unkn01_downcount > 0 )
      {
        unkn01_downcount--;
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
    */
    PacketRecord_Close();
    LbPaletteFade(NULL, 0x10u, 1);
}

void
game_quit (void)
{
    sound_finalise ();
    display_finalise ();
    SDL_Quit ();
    exit (0);
}

void
game_transform_path_full (const char *file_name, char *buffer, size_t size)
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
game_transform_path (const char *file_name, char *result)
{
    game_transform_path_full (file_name, result, FILENAME_MAX);
}

void
game_play_music (void)
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
    display_update();
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

                    display_update_mouse_pointer();

                    total_sleep_time -= SDL_GetTicks() - ticks;
                }
            }
        }
    }

    last_frame_ticks = SDL_GetTicks();

    display_lock();
}

int
game_wait_for_vsync (void)
{
    game_update_full(false);
    return 1;
}

void
game_update (void)
{
    game_update_full(true);
}

void
game_reset(void)
{
    host_reset();
    free_texturemaps();
    LbDataFreeAll(unk02_load_files);
}

