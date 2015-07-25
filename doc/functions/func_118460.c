bool
func_118460 (void)
{
  if (data_1edb7a != 0) /* 16bit */
    return 1;

  eax = 0x31;
  data_1edb2c.r32.eax = 0x100;
  data_1edb2c.r32.ebx = 0x1000; /* 1edb30 */
  dos_int386 (0x31, &data_1edb2c, &data_1edb2c);
  data_1edb7a = data_1edb2c.r16.ax; /* 16bit, segment */
  data_1edb7c = data_1edb2c.r32.edx; /* 32bit, 1edb38, selector */

  return (data_1edb44 != 0);
}
