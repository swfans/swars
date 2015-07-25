volatile uint32_t data_1592d8;

uint32_t
func_fbe40_get_1000hz_ticks (void)
{
  do
    {
      ticks = data_1592d8;
      outp (0x43, 0);
      raw = inpw (0x40);
    }
  while (data_1592d8 == ticks);

  edx = (data_1e86bc - raw) * 1000 / 0x1234dd
        + ticks * 1000 / data_1e86c0;

  if (edx >= data_1e86b4)
    {
      data_1e86b4 = edx;
      return data_1e86b4;
    }

  return data_1e86b4;
}
