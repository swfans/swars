#ifndef SOUND_UTIL_H
#define SOUND_UTIL_H

#include OPENAL_AL_H
#include <stdbool.h>
#include <stddef.h>

typedef void (*SoundNameCallback) (ALuint name, void *user_data);

#define sound_check_al(source) sound_check_al_line ((source), __LINE__)
bool sound_check_al_line (const char *source, int line);
void sound_delete_source_and_buffers (ALuint source);
size_t sound_unqueue_buffers (ALuint source, SoundNameCallback callback,
			      void *user_data);

#endif
