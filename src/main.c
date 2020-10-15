#include <SDL.h>
#include <getopt.h>
#include <stdlib.h>

#include "bflib_video.h"
#include "bflib_joyst.h"
#include "display.h"
#include "game.h"
#include "util.h"

#if defined _WIN32 && defined main
// Anti SDL
# undef main
#endif

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
"                -F        ?\n"
"                -g        ?\n"
"                -H        Initially enter high resolution mode\n"
"  --help        -h        Display the help message\n"
"                -I <num>  Connect through IPX\n"
"                -m <num>  Load map of given index\n"
"                -N        ?\n"
"                -p <num>  Play replay packets from file of given index\n"
"                -q        ?\n"
"                -r <num>  Record replay packets to file of given index\n"
"  --no-stretch  -S        Don't display 320x200 graphics stretched to 640x480\n"
"                -s <str>  Set session name string\n"
"                -T        ?\n"
"                -u <str>  Set user name string\n"
"  --windowed    -W        Run in windowed mode\n"
"                -w        ?\n",
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

    while ((val = getopt_long (*argc, *argv, "ABCDE:FgHhI:m:Np:qr:Ss:Tu:Ww", options, &index)) >= 0)
    {
        switch (val)
        {
        case 'A':
            unkn01_mode = 1;
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
            unkn01_mode = 3;
            flags_general_unkn01 |= 0x08;
            break;

        case 'H':
            lbDisplay.ScreenMode = 13;
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
            cmdln_param_map_index = atoi(optarg);
            flags_general_unkn01 |= 0x08;
            selected_map_index = cmdln_param_map_index;
            flags_general_unkn02 |= 0x04;
            break;

        case 'N':
            cmdln_param_n = 1;
            break;

        case 'p':
            cmdln_param_mp = 1;
            pktrec_mode = 2; /* playback */
            cmdln_pr_num = atoi(optarg);
            break;

        case 'q':
            flags_general_unkn01 |= 0x080000;
            break;

        case 'r':
            pktrec_mode = 1; /* record */
            cmdln_pr_num = atoi(optarg);
            break;

        case 'S':
            display_set_lowres_stretch (false);
            break;

        case 's':
            sprintf(session_name, "%s", optarg);
            DEBUGMSG(0, "session name %s\n", session_name);
            break;

        case 'T':
            cmdln_param_tf = 1;
            break;

        case 'u':
            sprintf(user_name, "%s", optarg);
            DEBUGMSG(0, "user name %s\n", user_name);
            break;

        case 'W':
            display_set_full_screen (false);
            break;

        case 'w':
            cmdln_param_w = 1;
            break;

        default:
            ERRORMSG("Command line parser error");
            exit (1);
        }
    }
}

int
main (int argc, char **argv)
{
  int retval;

  retval = 0;
  lbDisplay.ScreenMode = 1;
  unkn01_mode = 0;
  cmdln_param_w = 0;
  flags_general_unkn01 = 0;
  /* Gravis Grip joystick driver initialization */
  /* joy_grip_init(); */

  display_set_full_screen (true);
  display_set_lowres_stretch (true);

  process_options (&argc, &argv);

  printf ("Syndicate Wars Port "VER_STRING"\n"
	  "The original by Bullfrog\n"
	  "Ported by Unavowed <unavowed@vexillium.org> "
	  "and Gynvael Coldwind <gynvael@vexillium.org>\n"
	  "Web site: http://swars.vexillium.org/\n");

  if (!game_initialise ())
    return 1;

  read_conf_file();
  game_setup();

  game_process();

  game_reset();
  if ( in_network_game ) {
      /* LbNetworkReset(); */
      asm volatile ("call _LbNetworkReset_\n"
            :  :  : "eax" );
  }
  joy_grip_shutdown();
  /* LbMemoryReset(); */
  asm volatile ("call _LbMemoryReset_\n"
        :  :  : "eax" );

  game_quit ();

  return retval;
}
