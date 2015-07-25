void
11113e_restore_old_handler (void)
{
  timer_set_divisor (0);
  dos_setvect (0x8, 15a01c_old_int8_handler_segment:15a018_old_int8_handler);
}
