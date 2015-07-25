int
func_ee1c0_wait_for_vsync (void)
{
  do
    {
      al = inp (0x3da); /* VGA feature control register */
    }
  while ((al & 0x8) == 0); /* 0 = normal vsync, 1 = vsync OR display enable */

  return 1;
}
