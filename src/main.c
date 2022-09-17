#include <SDL.h>
#include <getopt.h>
#include <stdlib.h>

#include "bfmemory.h"
#include "bffile.h"
#include "bfscreen.h"
#include "bflog.h"
#include "swlog.h"
#include "bflib_joyst.h"
#include "display.h"
#include "game.h"
#include "game_data.h"
#include "network.h"
#include "util.h"

#if defined WIN32 && defined main
// Anti SDL
# undef main
#endif

const char *conf_file_cmnds[] = {
  "CD",
  "InstallDrive",
  "Language",
  "Data",
  "Intro",
  "Anims",
  "Maps",
  "Levels",
  "Sound",
  "DOS",
};

TbBool cmdln_fullscreen = true;
TbBool cmdln_lores_stretch = true;

static void
print_help (const char *argv0)
{
  printf (
"Usage: %s [OPTIONS]\n\n"
"Available options:\n"
"                -A        Enter game mode 1; not sure what this mode should be\n"
"                -B        Test scenario 99?\n"
"                -C        Test scenario 100?\n"
"                -D        Direct keyboard mode; queries kb rather than use\n"
"                          events/interrupts\n"
"                -E <num>  Joystick config\n"
"                -F        Re-compute colour fade tables\n"
"                -g        Enter normal gameplay mode; to be used when playing\n"
"                          the normal campaigns\n"
"                -H        Initially enter high resolution mode\n"
"  --help        -h        Display the help message\n"
"                -I <num>  Connect through IPX\n"
"                -m <num>  Enter single map mode and load map of given index\n"
"                -N        ?\n"
"                -p <num>  Play replay packets from file of given index;\n"
"                          use '-m' to specify map on which to play\n"
"                -q        Affects game quit conditions (but how?)\n"
"                -r        Record replay packets to file in savegame dir;\n"
"                          next unused filename for selected map will be used\n"
"  --no-stretch  -S        Don't stretch 320x200 graphics to high res display\n"
"                -s <str>  Set session name string\n"
"                -T        color tables mode (no effect?)\n"
"  --self-tests  -t        execute build self tests\n"
"                -u <str>  Set user name string\n"
"  --windowed    -W        Run in windowed mode\n"
"                -w        Lower memory use; decreases size of static arrays\n",
  argv0);
}

TbBool test_gpoly(void);

// To be moved to its own file when there are more tests
static void tests_execute(void)
{
    // TODO prpare tests
    if (!test_gpoly())
        exit(51);
    exit(0);
}

static void
process_options (int *argc, char ***argv)
{
    int index;
    int val;
    char *argv0;
    int tmpint;

    static struct option options[] =
    {
      {"windowed",    0, NULL, 'W'},
      {"no-stretch",  0, NULL, 'S'},
      {"self-test",   0, NULL, 't'},
      {"help",        0, NULL, 'h'},
      {NULL,          0, NULL,  0 },
    };

    argv0 = (*argv)[0];
    index = 0;

    while ((val = getopt_long (*argc, *argv, "ABCDE:FgHhI:m:Np:qrSs:Ttu:Ww", options, &index)) >= 0)
    {
        LOGDBG("Command line option: '%c'", val);
        switch (val)
        {
        case 'A':
            ingame.GameMode = GamM_Unkn1;
            ingame.cmdln_param_a = 3200;
            break;

        case 'B':
            cmdln_param_bcg = 99;
            break;

        case 'C':
            cmdln_param_bcg = 100;
            break;

        case 'D':
            keyboard_mode_direct = 1;
            break;

        case 'E':
            tmpint = atoi(optarg);
            if ( joy_grip_unknsub_08(tmpint) != -1 )
              unkn01_maskarr[17] = 17;
            break;

        case 'F':
            cmdln_colour_tables = 2;
            break;

        case 'g':
            cmdln_param_bcg = 1;
            ingame.GameMode = GamM_Unkn3;
            ingame.Flags |= 0x08;
            break;

        case 'H':
            lbDisplay.ScreenMode = Lb_SCREEN_MODE_640_480_8;
            break;

        case 'h':
            print_help (argv0);
            exit (0);
            break;

        case 'I':
            tmpint = atoi(optarg);
            LbNetworkSetupIPXAddress(tmpint);
            break;

        case 'm':
            is_single_game = 1;
            cmdln_param_current_map = atoi(optarg);
            ingame.GameMode = GamM_Unkn2;
            ingame.Flags |= 0x08;
            ingame.CurrentMission = cmdln_param_current_map;
            ingame.Cheats |= 0x04;
            LOGDBG("mission index %d", (int)ingame.CurrentMission);
            break;

        case 'N':
            cmdln_param_n = 1;
            break;

        case 'p':
            is_single_game = 1;
            pktrec_mode = PktR_PLAYBACK;
            cmdln_pr_num = atoi(optarg);
            LOGDBG("packet file play %d", cmdln_pr_num);
            break;

        case 'q':
            ingame.Flags |= 0x080000;
            break;

        case 'r':
            pktrec_mode = PktR_RECORD;
            LOGDBG("packet file record enabled");
            break;

        case 'S':
            cmdln_lores_stretch = false;
            break;

        case 's':
            sprintf(session_name, "%s", optarg);
            LOGDBG("session name '%s'", session_name);
            break;

        case 'T':
            cmdln_colour_tables = 1;
            break;

        case 't':
            tests_execute();
            break;

        case 'u':
            sprintf(user_name, "%s", optarg);
            LOGDBG("user name '%s'", user_name);
            break;

        case 'W':
            cmdln_fullscreen = false;
            break;

        case 'w':
            ingame.LowerMemoryUse = 1;
            break;

        default:
            LOGERR("Command line parser error");
            exit (1);
        }
    }
}

static void fixup_options(void)
{
    if (!is_single_game) {
        cmdln_param_bcg = 1;
    }
    // If game mode was not selected, set it to normal gameplay
    if (ingame.GameMode == GamM_None) {
        ingame.GameMode = GamM_Unkn3;
        ingame.Flags |= 0x08;
    }
}

void read_conf_file(void)
{
    char *curptr;
    TbFileHandle conf_fh;
    unsigned int i, n;
    char ch;
    char locbuf[1024];
    char prop_name[44];
    char *conf_fname = "config.ini";
    int conf_len;

    conf_fh = LbFileOpen(conf_fname, Lb_FILE_MODE_READ_ONLY);
    if (conf_fh != INVALID_FILE)
    {
        conf_len = LbFileRead(conf_fh, locbuf, sizeof(locbuf));
        LbFileClose(conf_fh);
    } else {
        LOGERR("Could not open installation config file, going with defaults.");
        conf_len = 0;
    }
    locbuf[conf_len] = '\0';
    // Parse the loaded file
    curptr = locbuf;
    while ( *curptr != 26 && *curptr != '\0' )
    {
        while (*curptr == '\n' || *curptr == '\r' || *curptr == '\t' || *curptr == ' ') {
          curptr++;
        }
        for (i=0; (*curptr != '=' && *curptr != '\0'); i++)
        {
            ch = *curptr++;
            prop_name[i] = ch;
        }
        curptr += 2;
        prop_name[i] = '\0';
        LOGDBG("%s: option '%s'", conf_fname, prop_name);
        for (n = 0; n < sizeof(conf_file_cmnds)/sizeof(conf_file_cmnds[0]); n++)
        {
            if (strcmp(prop_name, conf_file_cmnds[n]) == 0)
                break;
        }
        switch ( n )
        {
        case 0:
            for (i = 0; *curptr != '"'; i++) {
                ch = *curptr++;
                cd_drive[i] = ch;
            }
            cd_drive[i] = 0;
            LOGDBG("%s: Dir with CD data '%s'", conf_fname, cd_drive);
            break;
        case 2:
            for (i = 0; i < 3; i++) {
              ch = *curptr++;
              language_3str[i] = tolower(ch);
            }
            language_3str[i] = '\0';
            break;
        case 3:
            if (curptr[1] == 'a') { // "Max"
              game_dirs[DirPlace_Data].use_cd = 0;
            }
            break;
        case 4: // FIXME implementing original error - 'Intro' sets wrong option
            if ( curptr[1] == 'a' ) {
              game_dirs[DirPlace_Sound].use_cd = 0;
            }
            break;
        case 5: // FIXME implementing original error - 'Anims' sets wrong option
            if ( curptr[1] == 'a' ) {
              game_dirs[7].use_cd = 0;
            }
            break;
        case 6:
            if ( curptr[1] == 'a' ) {
              game_dirs[DirPlace_Maps].use_cd = 0;
            }
            break;
        case 7:
            if ( curptr[1] == 'a' ) {
              game_dirs[DirPlace_Levels].use_cd = 0;
            }
            break;
        case 8: // FIXME implementing original error - 'Sound' sets wrong option
            if ( curptr[1] == 'a' ) {
              game_dirs[DirPlace_Equip].use_cd = 0;
            }
            break;
        }

        while (*curptr != '\n'  && *curptr != '\0') {
          curptr++;
        }
    }
}

/** Read file with all the language-specific texts.
 */
void read_strings_file(void)
{
    char *text_fname = "data/text.dat";
    int text_len;

    text_len = LbFileLength(text_fname);
    gui_strings_data = (char *)LbMemoryAlloc(text_len);
    if (gui_strings_data != NULL) {
        gui_strings_data_end = gui_strings_data + text_len;
        LbFileLoadAt(text_fname, gui_strings_data);
    }
}

int
main (int argc, char **argv)
{
    lbDisplay.ScreenMode = Lb_SCREEN_MODE_320_200_8;
    ingame.GameMode = GamM_None;
    ingame.LowerMemoryUse = 0;
    ingame.Flags = 0;
    if (LbErrorLogSetup(NULL, NULL, Lb_ERROR_LOG_NEW) != Lb_SUCCESS)
            printf("Execution log setup failed\n");
    /* Gravis Grip joystick driver initialization */
    /* joy_grip_init(); */

    process_options(&argc, &argv);

    printf("Syndicate Wars Port "VERSION"\n"
        "The original by Bullfrog\n"
        "Ported by Unavowed <unavowed@vexillium.org> "
        "and Gynvael Coldwind <gynvael@vexillium.org>\n"
        "Web site: http://swars.vexillium.org/\n");

    fixup_options();
    adjust_memory_use();

    if (!game_initialise())
        return 1;

    display_set_full_screen(cmdln_fullscreen);
    display_set_lowres_stretch(cmdln_lores_stretch);

    read_conf_file();
    read_strings_file();
    game_setup();

    game_process();

    game_reset();
    if ( in_network_game ) {
        LbNetworkReset();
    }
    joy_grip_shutdown();
    LbErrorLogReset();
    LbMemoryReset();
    game_quit();
    // the above function never returns
    return 0;
}
