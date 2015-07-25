void
func_110feb (void)
{
  101c00_lock_push ();

  ecx = 0xffffffff;

  for (edi = 0; edi < 0x40; edi += 4)
    {
      if (*(159ed8_callback_types + edi) != 0) /* 32bit */
	{
	  eax = *(159f58_periods + edi);
	  ecx = MIN (eax, ecx);
	}
    }

  if (ecx != 15a026_period)
    {
      15a026_period = ecx;
      110fb8_set_period (15a026_period);
      memset (&159f18_elapsed_times, 0, 0x40);
    }

  101c00_lock_pop ();
}
