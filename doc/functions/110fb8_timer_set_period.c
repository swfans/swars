/*
 * frequency = 1193180 / divisor
 * divisor = interval * 1.1933174224343674
 * 
 * frequency = 1192380 / (interval * 1.1933174224343674)
 *           =  / interval [Hz]
 * period = interval / 999,214 [s] ≈ interval [μs]
 *
 * i.e. interval is in μs
 */
void
func_110fb8_set_timer_period (uint32_t usec)
{
  /* if the interval is any greater, then divisor will exceed 0xffff */
  if (usec < 0xd68d)
    eax = usec * 0x2710 / 0x20bc;
  else
    eax = 0;

  timer_set_divisor (eax)
}
