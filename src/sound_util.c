#include <assert.h>
#include <stdio.h>

#include OPENAL_AL_H

#include "sound_util.h"

#define check_al(source) sound_check_al (source)

bool
sound_check_al_line (const char *source, int line)
{
  ALenum err;

  err = alGetError ();
  if (err != AL_NO_ERROR)
    {
      fprintf (stderr,
	       "Error: %s: 0x%x at "__FILE__":%i.\n", source, err, line);
      return false;
    }

  return true;
}

size_t
sound_unqueue_buffers (ALuint source, SoundNameCallback callback,
		       void *user_data)
{
  ALint count;
  ALuint buf;
  size_t removed = 0;

  alGetSourcei (source, AL_BUFFERS_PROCESSED, &count);
  if (!check_al ("alGetSourcei (AL_BUFFERS_PROCESSED)"))
    return 0;

  while (count-- > 0)
    {
      alSourceUnqueueBuffers (source, 1, &buf);
      if (!check_al ("alSourceUnqueueBuffers"))
	return removed;

      if (callback != NULL)
	callback (buf, user_data);

      removed++;
    }

  return removed;
}

static void
delete_queued_buffer (ALuint buf)
{
  alDeleteBuffers (1, &buf);
  check_al ("alDeleteBuffers");
}

void
sound_delete_source_and_buffers (ALuint source)
{
  alSourceStop (source);
  check_al ("alSourceStop");

  sound_unqueue_buffers (source,
			 (SoundNameCallback) delete_queued_buffer, NULL);

  alDeleteSources (1, &source);
  check_al ("alDeleteSources");
}
