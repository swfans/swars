#include <SDL.h>
#include <getopt.h>
#include <stdlib.h>

#include "bflib_video.h"
#include "display.h"
#include "game.h"
#include "util.h"

#if defined _WIN32 && defined main
// Anti SDL
# undef main
#endif

#pragma pack(1)

typedef struct {
    char *Name;
    void *BufferPtr;
    int16_t field_8;
    int field_A;
    int field_E;
    char field_12[4];
    char field_16;
} BuffUnknStruct02;

#pragma pack()

extern char session_name[20];
extern char user_name[50];

extern uint8_t in_network_game;
extern uint8_t cmdln_param_mp;
extern uint8_t cmdln_param_tf;
extern uint8_t cmdln_param_bcg;
extern uint8_t cmdln_param_d;
extern uint8_t unkn01_maskarr[28];
extern uint8_t flags_general_unkn02;
extern uint8_t cmdln_param_n;
extern uint8_t pktrec_mode;
extern uint8_t cmdln_pr_num;
extern uint8_t game_perspective;

extern uint16_t unkn01_mode;
extern uint16_t selected_map_index;
extern uint32_t flags_general_unkn01;

extern uint16_t cmdln_param_a;
extern uint16_t cmdln_param_w;
extern uint16_t cmdln_param_map_index;
extern uint32_t mem_alloc_size;
extern BuffUnknStruct02 buffer_allocs[40];

extern uint32_t grip_unkvar080;

void read_conf_file(void);
void game_setup(void);
void game_process(void);
void game_reset(void);
int joy_grip_shutdown(void);

int joy_grip_unknsub_08(int val)
{
  if ( !val )
    return -1;
  grip_unkvar080 = val;
  return 1;
}

static void
print_help (const char *argv0)
{
  printf (
"Usage: %s [OPTIONS]\n\n"
"Available options:\n"
"  --windowed    -w    Run in windowed mode\n"
"  --no-stretch  -S    Don't display 320x200 graphics stretched to 640x480\n"
"  --help        -h    Display the help message\n",
  argv0);
}

static void
process_options (int *argc, char ***argv)
{
  int index;
  int val;
  char *argv0;

  static struct option options[] =
  {
    {"windowed",    0, NULL, 'w'},
    {"no-stretch",  0, NULL, 'S'},
    {"help",	    0, NULL, 'h'},
    {NULL,          0, NULL,  0 },
  };

  argv0 = (*argv)[0];
  index = 0;

  while ((val = getopt_long (*argc, *argv, "wSh", options, &index)) >= 0)
    {
      switch (val)
	{
	case 'w':
	  display_set_full_screen (false);
	  break;

        case 'S':
          display_set_lowres_stretch (false);
          break;

	case 'h':
	  print_help (argv0);
	  exit (0);
	  break;

	default:
	  exit (1);
	}
    }

  *argc -= optind - 1;
  *argv += optind - 1;

  (*argv)[0] = argv0;
}

int remade_main(int argc, char **argv)
{
    char *iarg;
    int i, k;
    int tmpint;
    char params_valid;

    params_valid = 1;
    lbDisplay.ScreenMode = 1;
    unkn01_mode = 0;
    cmdln_param_w = 0;
    flags_general_unkn01 = 0;
    /* Gravis Grip joystick driver initialization */
    /* joy_grip_init(); */
    for (i = 1; i < argc; i++)
    {
        iarg = argv[i];
        if ( iarg[0] != '-' && iarg[0] != '/' )
            continue;
        k = 1;
        while ( 1 )
        {
            if ( iarg[k] == '\0' )
              break;
            switch ( iarg[k++] )
            {
            case 'A':
            case 'a':
                unkn01_mode = 1;
                cmdln_param_a = 3200;
                break;
            case 'B':
            case 'b':
                cmdln_param_bcg = 99;
                break;
            case 'C':
            case 'c':
                cmdln_param_bcg = 100;
                break;
            case 'D':
            case 'd':
                cmdln_param_d = 1;
                break;
            case 'E':
            case 'e':
                tmpint = atoi(&iarg[k + 1]);
                if ( joy_grip_unknsub_08(tmpint) != -1 )
                  unkn01_maskarr[17] = 17;
                while ( iarg[k] != ' ' && iarg[k] != '\0' )
                  ++k;
                break;
            case 'F':
            case 'f':
                cmdln_param_tf = 2;
                while ( 1 )
                {
                  if ( iarg[k] == ' ' || !iarg[k] )
                    break;
                  ++k;
                }
                break;
            case 'G':
            case 'g':
                cmdln_param_bcg = 1;
                unkn01_mode = 3;
                flags_general_unkn01 |= 0x08;
                break;
            case 'H':
            case 'h':
                lbDisplay.ScreenMode = 13;
                break;
            case 'I':
            case 'i':
                tmpint = atoi(&iarg[k + 1]);
                /* LbNetworkSetupIPXAddress(tmpint); */
                asm volatile ("call LbNetworkSetupIPXAddress_\n"
                      :  : "a" (tmpint) );
                while ( iarg[k] != ' ' && iarg[k] != '\0' )
                  ++k;
                break;
            case 'M':
            case 'm':
                cmdln_param_mp = 1;
                cmdln_param_map_index = atoi(&iarg[k]);
                while ( iarg[k] != ' ' && iarg[k] != '\0' )
                  ++k;
                flags_general_unkn01 |= 0x08;
                selected_map_index = cmdln_param_map_index;
                flags_general_unkn02 |= 0x04;
                break;
            case 'N':
            case 'n':
                cmdln_param_n = 1;
                while ( iarg[k] != ' ' && iarg[k] != '\0' )
                  ++k;
                break;
            case 'P':
            case 'p':
                cmdln_param_mp = 1;
                pktrec_mode = 2;
                cmdln_pr_num = atoi(&iarg[k]);
                while ( iarg[k] != ' ' && iarg[k] != '\0' )
                  ++k;
                break;
            case 'Q':
            case 'q':
                flags_general_unkn01 |= 0x080000;
                break;
            case 'R':
            case 'r':
                pktrec_mode = 1;
                cmdln_pr_num = atoi(&iarg[k]);
                while ( iarg[k] != ' ' && iarg[k] != '\0' )
                  ++k;
                break;
            case 'S':
            case 's':
                sprintf(session_name, "%s", &iarg[k]);
                DEBUGMSG(0, " session name %s\n", session_name);
                while ( iarg[k] != ' ' && iarg[k] != '\0' )
                  ++k;
                break;
            case 'T':
            case 't':
                cmdln_param_tf = 1;
                break;
            case 'U':
            case 'u':
                sprintf(user_name, "%s", &iarg[k]);
                DEBUGMSG(0, " user name %s\n", user_name);
                while ( iarg[k] != ' ' && iarg[k] != '\0' )
                  ++k;
                break;
            case 'W':
            case 'w':
                cmdln_param_w = 1;
                break;
            default:
                params_valid = 0;
                break;
            }
        }
    }

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
        mem_alloc_size = 2700000;
        game_perspective = (buffer_allocs[5].field_A >> 8) & 0xff;
    }
    if ( !cmdln_param_mp )
      cmdln_param_bcg = 1;
    if ( params_valid )
    {
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
    }
    return 0;
}

int
main (int argc, char **argv)
{
  int retval;

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

  // Call game main
  retval = remade_main(argc, argv);
  /*asm volatile ("call asm_main\n"
		: "=a" (retval) : "a" (argc), "d" (argv));*/

  game_quit ();

  return retval;
}
