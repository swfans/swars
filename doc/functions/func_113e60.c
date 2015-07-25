void
ailimpl_sample_buffer_ready (SoundSample *sample)
{
  if (sample == NULL)
    return -1;

  if (sample->last_buffer == -2)
    {
      sample->done[1] = 0;
      sample->last_buffer = -1;
      return 0;
    }
  else if (sample->last_buffer == -1)
    {
      sample->last_buffer = sample->current_buffer;
      return 1;
    }

  if (sample->last_buffer == sample->current_buffer)
    return -1;

  sample->last_buffer = sample->current_buffer;

  return (sample->current_buffer ^ 1);
}
