void
AIL_start_ (void)
{
  if (data_15aa4c != 0) /* 32bit */
    return;

  VMM_lock_range (&AIL_start_, &AIL_end_);

  func_10f3d0 (&data_1ed6b0, 0x40);
  func_10f3d0 (&data_1ed80c, 0x4c);
  func_10f3d0 (&data_1ed6f0, 0x100);
  func_10f3d0 (&data_1ed7f0, 0x18);
  func_10f3d0 (&data_15aa48, 0x4);
  func_10f3d0 (&data_1ed808, 0x4);

  data_15aa4c = 1; /* 32bit */
}
