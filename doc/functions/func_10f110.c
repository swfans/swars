void
func_10f110 (void)
{
  data_159780 = &func_10f160;
  /* number of times IRQ0 has been called */
  data_1ed5e0 = -(*((uint32_t *) 0x46c));
  data_1ed5e8 = &__nullsub_12;

  outp (0x43, 0x34); /* port, value */
  /*
   * 0x34 == 00110100
   * 00  - counter 0 (divisor)
   * 11  - write counter bits 0-7 first, then 8-15
   * 010 - mode 2 select - rate generator
   * 0   - binary counter 16 bits
   */
  outp (0x40, 0);
  outp (0x40, 0);
}
