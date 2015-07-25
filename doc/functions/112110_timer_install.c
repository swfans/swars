void
func_112110 (void)
{
  func_111840 ();
  func_101ae0 (0x0, 0xc8);
  func_101ae0 (0x1, 0x1);
  func_101ae0 (0x2, 0x8000);
  func_101ae0 (0x3, 0x64);
  func_101ae0 (0x4, 0x10);
  func_101ae0 (0x5, 0x64);
  func_101ae0 (0x6, 0x28f);
  func_101ae0 (0x7, 0x0);
  func_101ae0 (0x8, 0x0);
  func_101ae0 (0x9, 0x1);
  func_101ae0 (0xa, 0x0);
  func_101ae0 (0xb, 0x78);
  func_101ae0 (0xc, 0x8);
  func_101ae0 (0xd, 0x7f);
  func_101ae0 (0xe, 0x1);
  func_101ae0 (0xf, 0x0);
  func_101ae0 (0x10, 0x2);
  func_101ae0 (0x11, 0x1);
  func_101ae0 (0x12, 0x1);

  for (eax = 0x4; eax < 0x40; eax += 0x4)
    *(data_1ed6ac + eax) = 0; /* 32bit */

  data_1ed6f0 = 0; /* 8bit */

  disable_interrupts ();

  data_15a032 = (uint16_t) ds;
  (uint16_t) es = data_15032;
  func_11181a (); /* locks some memory pages */

  15a02a_lock = 0; /* 32bit */
  data_15a02e = 0; /* 32bit */
  15a026_period = 0xffffffff; /* 32bit */
  data_15a034 = 0xffffffff; /* 32bit */

  memset (&159ed8_callback_types, 0, 0x40);
  memset (&159f18_elapsed_times, 0, 0x40);
  memset (&159f58_periods, 0, 0x40);
  memset (&159f98_callback_call_counts, 0, 0x40);

  ecx = dos_getvect (0x8); /* via int 0x31 */
  eax = 0x8;

  es, ebx = dos_getvect (0x8); /* via int 0x21 */
  dx = es;

  15a018_old_int8_handler = ebx; /* DOS IRQ0 handler offset */
  15a01c_old_int8_handler_segment = dx;  /* DOS IRQ0 handler segment */
  data_15a01e = ecx; /* DPMI IRQ0 handler */

  dos_setvect (0x8, func_110e62); /* DOS IRQ0 handler via int 0x21 */
  
  data_159f14 = 0x2; /* 32bit */
  func_1026c0 (0x3c, 0xd68d);

  /* enable or disable interrupts here based on some variable*/
}
