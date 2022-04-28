#include <SDL.h>
#include <getopt.h>
#include <stdlib.h>

#include "bflib_video.h"
#include "bflib_joyst.h"
#include "bfmemory.h"
#include "bffile.h"
#include "display.h"
#include "game.h"
#include "game_data.h"
#include "util.h"

#if defined WIN32 && defined main
// Anti SDL
# undef main
#endif

extern char *conf_file_cmnds[10];

static void
print_help (const char *argv0)
{
  printf (
"Usage: %s [OPTIONS]\n\n"
"Available options:\n"
"                -A        ?\n"
"                -B        ?\n"
"                -C        ?\n"
"                -D        ?\n"
"                -E <num>  Joystick config\n"
"                -F        re-compute colour fade tables\n"
"                -g        no idea, but you should use it - original game did\n"
"                -H        Initially enter high resolution mode\n"
"  --help        -h        Display the help message\n"
"                -I <num>  Connect through IPX\n"
"                -m <num>  Load map of given index (use without -g)\n"
"                -N        ?\n"
"                -p <num>  Play replay packets from file of given index;\n"
"                          use '-m' to specify map on which to play\n"
"                -q        Affects game quit conditions (but how?)\n"
"                -r        Record replay packets to file in savegame dir;\n"
"                          next unused filename for selected map will be used\n"
"  --no-stretch  -S        Don't display 320x200 graphics stretched to 640x480\n"
"                -s <str>  Set session name string\n"
"                -T        color tables mode (no effect?)\n"
"                -u <str>  Set user name string\n"
"  --windowed    -W        Run in windowed mode\n"
"                -w        adjust memory allocations for less RAM(?)\n",
  argv0);
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
      {"help",        0, NULL, 'h'},
      {NULL,          0, NULL,  0 },
    };

    argv0 = (*argv)[0];
    index = 0;

    while ((val = getopt_long (*argc, *argv, "ABCDE:FgHhI:m:Np:qrSs:Tu:Ww", options, &index)) >= 0)
    {
        DEBUGLOG(1,"Command line option: '%c'", val);
        switch (val)
        {
        case 'A':
            ingame__GameMode = 1;
            cmdln_param_a = 3200;
            break;

        case 'B':
            cmdln_param_bcg = 99;
            break;

        case 'C':
            cmdln_param_bcg = 100;
            break;

        case 'D':
            cmdln_param_d = 1;
            break;

        case 'E':
            tmpint = atoi(optarg);
            if ( joy_grip_unknsub_08(tmpint) != -1 )
              unkn01_maskarr[17] = 17;
            break;

        case 'F':
            cmdln_param_tf = 2;
            break;

        case 'g':
            cmdln_param_bcg = 1;
            ingame__GameMode = 3;
            flags_general_unkn01 |= 0x08;
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
            /* LbNetworkSetupIPXAddress(tmpint); */
            asm volatile ("call LbNetworkSetupIPXAddress_\n"
                  :  : "a" (tmpint) );
            break;

        case 'm':
            cmdln_param_mp = 1;
            cmdln_param_current_map = atoi(optarg);
            flags_general_unkn01 |= 0x08;
            selected_map_index = cmdln_param_current_map;
            ingame__Cheats |= 0x04;
            DEBUGLOG(0, "map index %d", cmdln_param_current_map);
            break;

        case 'N':
            cmdln_param_n = 1;
            break;

        case 'p':
            cmdln_param_mp = 1;
            pktrec_mode = 2; /* playback */
            cmdln_pr_num = atoi(optarg);
            DEBUGLOG(0, "packet file play %d", cmdln_pr_num);
            break;

        case 'q':
            flags_general_unkn01 |= 0x080000;
            break;

        case 'r':
            pktrec_mode = 1; /* record */
            DEBUGLOG(0, "packet file record enabled");
            break;

        case 'S':
            display_set_lowres_stretch (false);
            break;

        case 's':
            sprintf(session_name, "%s", optarg);
            DEBUGLOG(0, "session name '%s'", session_name);
            break;

        case 'T':
            cmdln_param_tf = 1;
            break;

        case 'u':
            sprintf(user_name, "%s", optarg);
            DEBUGLOG(0, "user name '%s'", user_name);
            break;

        case 'W':
            display_set_full_screen (false);
            break;

        case 'w':
            cmdln_param_w = 1;
            break;

        default:
            ERRORLOG("Command line parser error");
            exit (1);
        }
    }
}

void read_conf_file(void)
{
    char *curptr;
    TbFileHandle conf_fh;
    unsigned int i, n;
    char ch;
    int text_len;
    char locbuf[1024];
    char prop_name[44];
    char *conf_fname = "config.ini";
    char *text_fname = "data/text.dat";

    conf_fh = LbFileOpen(conf_fname, Lb_FILE_MODE_READ_ONLY);
    if (conf_fh != -1)
    {
        text_len = LbFileRead(conf_fh, locbuf, sizeof(locbuf));
        LbFileClose(conf_fh);
    } else {
        ERRORLOG("Could not open installation config file, going with defaults.");
        text_len = 0;
    }
    locbuf[text_len] = '\0';
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
        DEBUGLOG(2,"%s: option '%s'", conf_fname, prop_name);
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
            DEBUGLOG(0,"%s: Dir with CD data '%s'", conf_fname, cd_drive);
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
    // Read file with all the language-specific texts
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
    int retval;

    retval = 0;
    lbDisplay.ScreenMode = Lb_SCREEN_MODE_320_200_8;
    ingame__GameMode = 0;
    cmdln_param_w = 0;
    flags_general_unkn01 = 0;
    /* Gravis Grip joystick driver initialization */
    /* joy_grip_init(); */

    display_set_full_screen(true);
    display_set_lowres_stretch(true);

    process_options(&argc, &argv);

    printf("Syndicate Wars Port "VERSION"\n"
        "The original by Bullfrog\n"
        "Ported by Unavowed <unavowed@vexillium.org> "
        "and Gynvael Coldwind <gynvael@vexillium.org>\n"
        "Web site: http://swars.vexillium.org/\n");

    if (!game_initialise())
        return 1;

    read_conf_file();
    game_setup();

    game_process();

    game_reset();
    if ( in_network_game ) {
        /* LbNetworkReset(); */
        asm volatile ("call LbNetworkReset_\n"
              :  :  : "eax" );
    }
    joy_grip_shutdown();
    LbMemoryReset();
    game_quit();

    return retval;
}
