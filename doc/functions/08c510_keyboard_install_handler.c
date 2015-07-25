int
func_8c510 (void)
{
  memset (&data_1e2f74, 0, 256);
  data_1c47c0 = dos_getvect (9); /* keyboard data ready IRQ */
  data_1c47c4 = 0;
  dos_setvect (9, func_8c578);
  return 1;
}
