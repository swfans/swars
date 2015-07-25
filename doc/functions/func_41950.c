void
func_41950 ()
{
  //push   %ebx
  //push   %ecx
  //push   %edx
  //push   %esi
  //push   %edi
  //sub    $0x20,%esp

  if (data_1aa278[0] != 0) /* 32bit, some sort of table */
    {
      for (ecx = 0; ecx < 5 && data_1aa278[ecx] != 0; ecx++)
	{
	  sprintf (esp + 8, "data/tex%02d.dat", ecx);
	  func_ea50c (esp, data_1aa278[ecx]);
	}
      return;
    }

  ebx = 0;

  for (;;)
    {
      sprintf (esp + 8, "data/tex%02d.dat", ebx);

      eax = df9e4_open_file (esp, 0x2); /* does open() and stuff */
      edx = ebx + 1;

      if (eax == -1)
	break;

      func_e8a1c (eax); /* wrapper over close() */
      ebx++;
    }

  if (ebx == 0)
    return;

  eax = func_dc6a0 (edx << 16); /* allocate something */

  if (eax == NULL)
    {
      data_1a71b1 = 1; /* 8bit */
      return;
    }

  data_1aa2f8 = eax;
  data_1810d1 = eax;

  ecx = (eax + 0x10000) & ~0xff;
  data_1810d5 = ecx - eax;
  if (ecx == 0)
    return;

  for (esi = 0; esi < ebx; esi++)
    {
      sprintf (esp + 8, "data/tex%02d.dat", esi);
      func_ea50c (esp, ecx);
      (data_1aa274 + 4 * esi) = ecx;
      ecx += 0x10000;
    }

  data_180cae = bl - 1;
}
