void *
func_111651_set_callback_argument (uint32_t index, void *arg)
{
  void *old_arg;

  AIL_lock ();

  if (index != -1)
    {
      old_arg = *(data_159fd8 + index);
      *(data_159fd8 + index) = arg;
    }

  AIL_unlock ();

  return old_arg;
}
