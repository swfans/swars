#ifndef OGGVORBIS_H
#define OGGVORBIS_H

#include <stdbool.h>
#include <vorbis/vorbisfile.h>
#include OPENAL_AL_H

#define SOUND_MUSIC_BUFFERS   3

struct OggVorbisStream
{
  ALuint	 source;
  OggVorbis_File file;
  vorbis_info	 info;
  bool		 playing;
  char		*file_name;
  ALuint	 buffers[SOUND_MUSIC_BUFFERS];
  size_t	 buffer_count;
};

typedef struct OggVorbisStream OggVorbisStream;

bool ogg_vorbis_stream_init (OggVorbisStream *stream);
void ogg_vorbis_stream_free (OggVorbisStream *stream);
void ogg_vorbis_stream_pause (OggVorbisStream *stream);
void ogg_vorbis_stream_play (OggVorbisStream *stream);
void ogg_vorbis_stream_stop (OggVorbisStream *stream);
void ogg_vorbis_stream_clear (OggVorbisStream *stream);
bool ogg_vorbis_stream_open (OggVorbisStream *stream, const char *fname);
bool ogg_vorbis_stream_restart (OggVorbisStream *stream);
void ogg_vorbis_stream_set_gain (OggVorbisStream *stream, float gain);
bool ogg_vorbis_stream_update (OggVorbisStream *stream);

#endif
