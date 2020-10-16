#include <SDL.h>
#include <getopt.h>
#include <stdlib.h>

#include "bflib_video.h"
#include "bflib_joyst.h"
#include "bflib_fileio.h"
#include "bflib_memory.h"
#include "display.h"
#include "game.h"
#include "util.h"

#if defined _WIN32 && defined main
// Anti SDL
# undef main
#endif

#pragma pack(1)

typedef struct {
  char *directory;
  uint8_t use_cd;
} PathInfo;

#pragma pack()

extern char *conf_file_cmnds[10];
extern PathInfo game_dirs[8];
extern char *game_text_str;
extern char language_3str[4];
extern char cd_drive[52];

void ASM_read_conf_file(void);

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
"                -p <num>  Play replay packets from file of given index\n"
"                -q        Affects game quit conditions (but how?)\n"
"                -r <num>  Record replay packets to file of given index\n"
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
            DEBUGLOG(0, "map index %d\n", cmdln_param_map_index);
            break;

        case 'N':
            cmdln_param_n = 1;
            break;

        case 'p':
            cmdln_param_mp = 1;
            pktrec_mode = 2; /* playback */
            cmdln_pr_num = atoi(optarg);
            DEBUGLOG(0, "packet file play %d\n", cmdln_pr_num);
            break;

        case 'q':
            flags_general_unkn01 |= 0x080000;
            break;

        case 'r':
            pktrec_mode = 1; /* record */
            cmdln_pr_num = atoi(optarg);
            DEBUGLOG(0, "packet file record %d\n", cmdln_pr_num);
            break;

        case 'S':
            display_set_lowres_stretch (false);
            break;

        case 's':
            sprintf(session_name, "%s", optarg);
            DEBUGLOG(0, "session name %s\n", session_name);
            break;

        case 'T':
            cmdln_param_tf = 1;
            break;

        case 'u':
            sprintf(user_name, "%s", optarg);
            DEBUGLOG(0, "user name %s\n", user_name);
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
    unsigned int conf_fh;
    unsigned int i, n;
    char ch;
    int text_len;
    char locbuf[1024];
    char prop_name[44];

    conf_fh = FileOpenInclCD("config.ini", 2);
    if (conf_fh != (unsigned int)-1)
    {
        text_len = LbFileRead(conf_fh, locbuf, 1024);
        LbFileClose(conf_fh);
        curptr = locbuf;
        locbuf[text_len] = '\0';
        while ( *curptr != 26 && *curptr != '\0' )
        {
            for (i=0; (*curptr != '='); i++)
            {
                ch = *curptr++;
                prop_name[i] = ch;
            }
            curptr += 2;
            prop_name[i] = '\0';
            for (n = 0; n < sizeof(conf_file_cmnds)/sizeof(conf_file_cmnds[0]); n++)
            {
                if (strcmp(prop_name, conf_file_cmnds[n]) != 0)
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
                DEBUGLOG("CDDRIVE >%s<\n", cd_drive);
                break;
            case 2:
                for (i = 0; i < 3; i++) {
                  ch = *curptr++;
                  language_3str[i] = tolower(ch);
                }
                language_3str[i] = '\0';
                break;
            case 3:
                if (curptr[1] == 'a') { /* "Max" */
                  game_dirs[0].use_cd = 0;
                }
                break;
            case 4:
                if ( curptr[1] == 'a' ) {
                  game_dirs[5].use_cd = 0;
                }
                break;
            case 5:
                if ( curptr[1] == 'a' ) {
                  game_dirs[7].use_cd = 0;
                }
                break;
            case 6:
                if ( curptr[1] == 'a' ) {
                  game_dirs[2].use_cd = 0;
                }
                break;
            case 7:
                if ( curptr[1] == 'a' ) {
                  game_dirs[3].use_cd = 0;
                }
                break;
            case 8:
                if ( curptr[1] == 'a' ) {
                  game_dirs[6].use_cd = 0;
                }
                break;
            }
            while ( *curptr != 10 ) {
              curptr++;
            }
        }
    }
    /* Read file with all the language-specific texts */
    if ( game_dirs[0].use_cd )
      sprintf(locbuf, "%slanguage/%s/text.dat", cd_drive, language_3str);
    else
      sprintf(locbuf, "data/text.dat");
    text_len = LbFileLength(locbuf);
    game_text_str = LbMemoryAlloc(text_len);
    if (game_text_str != NULL)
      LbFileLoadAt(locbuf, game_text_str);
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

  display_set_full_screen(true);
  display_set_lowres_stretch(true);

  process_options(&argc, &argv);

  printf("Syndicate Wars Port "VER_STRING"\n"
	  "The original by Bullfrog\n"
	  "Ported by Unavowed <unavowed@vexillium.org> "
	  "and Gynvael Coldwind <gynvael@vexillium.org>\n"
	  "Web site: http://swars.vexillium.org/\n");

  if (!game_initialise())
    return 1;

  ASM_read_conf_file();
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

  game_quit();

  return retval;
}
