/* frequency = 1193180 / divisor */
void
func_110f88_set_divisor (uint32_t divisor)
{
  outb (0x43, 0x36);
  /* 
   * 0x36 == 00110110
   * 00  select counter 0, the divisor
   * 11  write counter bits 0-7 first, then 8-15
   * 011 mode 3 select - square wave generator
   * 0   counting style - binary counter 16 bits
   */

  15a022_divisor = divisor;

  outb (0x40, (uint8_t) (arg0 & 0xff));
  outb (0x40, (uint8_t) ((arg0 >> 8) & 0xff));
}
