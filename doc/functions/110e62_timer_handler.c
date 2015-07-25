void interrupt
func_110e62 (void)
{
  if (15a02a_reentrance_lock > 0) /* 32bit */
    {
      outp (0x20, 0x20);
      return;
    }

  ds = data_15a032; /* 16bit */
  es = data_15a032; /* 16bit */
  15a02a_reentrance_lock++; /* 32bit */
  data_15aa40 = ss;
  data_15aa44 = esp;
  ss = ds;
  esp = &data_15aa40;
  edx = 15a026_period;
  edi = 0;

  for (edi = 0; edi < 0x40; edi += 0x4)
    {
      if (*(159ed8_callback_types + edi) != 2) /* 32bit */
	continue;

      eax = 15a026_period + *(159f18_elapsed_times + edi);
      if (eax >= *(159f58_periods + edi))
	{
	  eax -= *(159f58_periods + edi);
	  (*(159f98_callback_call_counts + edi))++;
	}

      *(159f18_elapsed_times + edi) = eax;
    }

  outp (0x20, 0x20);

  if (15a02e_lock <= 0) /* 32bit */
    {
      for (edi = 0; edi < 0x3c; edi += 0x4)
	{
	  while (*(159f98_callback_call_counts + edi) != 0) /* 32bit */
	    {
	      (*(159f98_callback_call_counts + edi))--;
	      eax = *(159fd8_callback_arguments + edi) /* 32bit */
	      *(159e98_callbacks + edi) (eax); /* indirect function call */
	    }
	}
    }

  if (data_159fd4 != 0) /* 32bit */
    {
      /* 
       * TODO: try to understand what's going on with the lret
       */
      data_159fd4--;
      ss = data_15aa40;
      esp = data_15aa44;
      15a02a_reentrance_lock--
      ecx = data_15a018;
      eax = (uint16_t) data_15a01c;
      /* snipped some magic manipulation */

      long return;
    }

  ss = data_15aa40;
  esp = data_15aa44;
  15a02a_reentrance_lock--;
  return;
}
