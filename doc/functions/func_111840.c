void
func_111840 (void)
{
  if (data_15aa4c != 0) /* 32bit */
    return;

  memory_lock_pages (&func_111840, &func_112280);

  func_10f3d0 (&data_1ed6b0, 0x40);
  func_10f3d0 (&data_1ed80c, 0x4c);
  func_10f3d0 (&data_1ed6f0, 0x100);
  func_10f3d0 (&data_1ed7f0, 0x18);
  func_10f3d0 (&data_15aa48, 0x4);
  func_10f3d0 (&data_1ed808, 0x4);

  data_15aa4c = 1; /* 32bit */
}
