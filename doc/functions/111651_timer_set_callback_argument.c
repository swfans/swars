void *
func_111651_set_callback_argument (uint32_t index, void *arg)
{
  void *old_arg;

  timer_lock_push ();

  if (index != -1)
    {
      old_arg = *(data_159fd8 + index);
      *(data_159fd8 + index) = arg;
    }

  timer_lock_pop ();

  return old_arg;
}
