/*
 * esi +
 * 00	void *		src_next
 * 04	void *		src_end
 * 08	void *		src_ptr
 * 10	uint32_t	len
 * 34	void *		next
 * 38	SoundSample *	sample
 * 3c	uint32_t	buffer_size
 * 50	uint32_t	total_pos? (Σpos)
 * 54	void *[2]	buffers
 * 68	uint32_t	pos
 * 6c	uint32_t	finish_time (msec)
 */
void
fc210_update_sound_samples (void)
{
  uint32_t len;

  for (esi = 1592cc_list; esi != NULL; esi = esi->next)
    {
      if (esi->len <= *(esi + 0x64)
	  && (*(esi + 0x44) == 0 || esi->len < 4))
	    continue;

      bufidx = ail_sample_buffer_ready (esi->sample);
      if (bufidx > 0x1)
	continue;

      len = MIN (esi->len, esi->buffer_size) & ~3;

      esi->src_ptr = fc340_copy_audio_data (esi->buffer[bufidx],
					    esi->src_ptr, esi->src_next,
					    esi->src_end, len);

      if (esi->finish_time == 0
	  || fc300_sample_finished (esi->sample)
	  || ail_sample_status (esi->sample) == 2 /* DONE */)
	{
	  esi->finish_time = (*data_159780) (); /* SDL_GetTicks () */
	  esi->total_pos += esi->pos;
	  esi->pos = 0;
	}

      ail_load_sample_buffer (esi->sample, bufidx, esi->buffers[bufidx], len);
      esi->pos += len;
      esi->len -= len;
    }
}

bool
fc300_sample_finished (SoundSample *sample)
{
  return (sample->2c_last_buffer >= 0
	  && sample->pos[0] == sample->len[0]
	  && sample->pos[1] == sample->len[1]);
}

/* 
 * NOTE: weird calling convention: arguments pushed in order, own stack
 * unwinding
 *
 * if (src_ptr) holds more than (len) bytes, then:
 *
 *   copy (len) bytes from (src_ptr) to (dst)
 *   return (src_ptr)+(len)
 *
 * else:
 *
 *   copy everything from (src_ptr) to (dst)
 *   copy the rest up to (len) from (src_next) to (dst)
 *   return (src_next)+(n_copied_from_src0)
 */
void *
fc340_copy_audio_data (void *dst, void *src_ptr, void *src_next,
		       void *src_end, uint32_t len)
{
  uint32_t count;
  void *src;

  count = src_end - src_ptr;

  if (count <= len)
    {
      memcpy (dst, src_ptr, count);

      dst += count;
      len -= count;

      src = src_next;
    }
  else
    src = src_ptr;

  memcpy (dst, src, len);
  src += len;

  return src;
}
