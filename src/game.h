#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stddef.h>

bool game_initialise (void);
void game_handle_sdl_events (void);
void game_update (void);
void game_quit (void);
void game_transform_path (const char *file_name, char *result);
const char *game_get_data_path (void);
const char *game_get_user_path (void);

#endif
