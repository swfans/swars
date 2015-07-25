uint16_t
func_1184d0 (uint16_t size)
{
  data_1edb2c.r16.ax = 0x100;
  data_1edb30.r16.bx = size; /* number of paragraphs */

  dos_int386 (0x31, &data_1edb2c, &data_1edb2c);

  if (data_1edb2c.r32.eflags != 0) /* 32bit */
    return 0;

  return data_1edb2c.r16.ax;
}
