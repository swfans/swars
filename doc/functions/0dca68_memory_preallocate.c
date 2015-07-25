func_dca68 (void)
{
  DOS_Registers regs;   /* %esp + 0x54 */
  DOS_Registers oregs;  /* %esp + 0x70 */
  DOS_Registers regs2;  /* %esp + 0x38 */
  DOS_Registers oregs2; /* %esp + 0x8c */
  uint32_t esp_a8;
  uint32_t esp_ac;

  //push   %ebx
  //push   %ecx
  //push   %edx
  //push   %esi
  //push   %edi
  //push   %ebp
  //sub    $0xb0,%esp

  if (data_1e08dc != 0) /* 32bit */
    return 1;

  regs.r32.eax = 0x100;  /* allocate DOS memory */
  regs.r32.ebx = 0x1000; /* number of paragraphs */
  _int386_ (0x31, &regs, &oregs);

  if (oregs.r32.eflags == 0) /* if success */
    esp_a8 = (oregs.r32.edx << 16) | oregs.r32.eax;
  else
    esp_a8 = 0;

  ecx = &data_1e08d4;

  do
    {
      memset (ecx, 0, 0xc);
      ecx += 0xc;
    }
  while (ecx != &data_1e08d4 + 0xc00);

  ecx = &data_1e14d4;

  do
    {
      memset (ecx, 0, 0x12);
      ecx += 0x12;
    }
  while (ecx != &data_1e14d4 + 0x1200);

  ecx = 0xa0000;
  ebp = 0;
  esi = 0;

  do
    {
      edi = 0;

      if (ecx < 0xa0000)
	{
	  regs2.r32.eax = 0x100; /* DOS alloc */

	  /* number of paragraphs to allocate */
	  regs2.r32.ebx = (ecx + 0xf) >> 4; 

	  _int386_ (0x31, &regs2, &oregs2);

	  if (oregs2.r32.eflags == 0) /* if success */
	    edi = (oregs2.r32.edx << 16) | oregs2.r32.eax;
	}

      if (edi != 0)
	{
	  *(data_1e08dc + esi) = ecx;
	  *(data_1e08cc + esi + 0xc) = (edi >> 0x10); /* selector */
	  *(data_1e08c8 + esi + 0xc) = (edi & 0xffff) << 4; /* DOS address */
	  esi += 0xc;
	  ecx += 0x400;
	  ebp++;
	}

      ecx -= 0x400;
    }
  while (ecx >= 0x400);

  esp_ac = 0xc * ebp;

  ecx = 0x1000000;

  do
    {
      eax = malloc (ecx);
      *(data_1e08d4 + esp_ac) = eax;

      if (eax != NULL)
	{
	  ebx = ecx + 0x1000;
	  edi = ecx;

	  do
	    {
	      /* 
	       * XXX: this is actually nexpand, which means the address stays
	       * the same.
	       */
	      eax = realloc (*(data_1e08d4 + esp_ac), ebx);
	      if (eax != NULL)
		{
		  *(data_1e08dc + esp_ac) = ebx;
		  *(data_1e08d8 + esp_ac) = 0; /* 32bit */
		  break;
		}

	      ebx -= 0x10;
	    }
	  while (ebx > edi);

	  ecx += 0x1000;
	  esp_ac += 0xc;
	  ebp++;
	}

      ecx -= 0x1000;
    }
  while (ecx >= 0x1000);

  ebx = 0xc * ebp;
  ecx = 0x1000;
  
  do 
    {
      eax = malloc (ecx);
      *(data_1e08d4 + ebx) = eax;

      if (eax != NULL)
	{
	  ebx += 0x0c;
	  *(data_1e08d0 + ebx) = ecx;
	  *(data_1e08cc + ebx) = NULL;
	  ecx += 0x1000;
	  ebp++;
	}

      ecx -= 0x10;
    }
  while (ecx >= 0x10);

//    _WCRTLINK void qsort( void *in_base, size_t n, size_t size,
//                          int (*compar)(const void *, const void *) )
  /*
   * Note: func_dceac returns the selector of the first table entry argument.
   * This can either be 0 for malloced memory, or positive for DOS
   * allocations.  The result of this call, at least on glibc qsort, is that
   * malloced entries go first in the table and all the other entries go after
   * them.  (TODO: needs to be checked on watcom's qsort though)
   */
  qsort (&data_1e08d4, ebp, 0x0c, func_dceac);
  eax = 0;

  do
    {
      eax += 0x12;
      *(data_1e14c6 + eax) = 0; /* 32bit */
    }
  while(eax != 0x1200);

  ecx = 0;

  // test   %ebp,%ebp, jle    jump_dcd0c TODO: double check this translation
  if (ebp <= 0)
    goto jump_dcd0c; 

  esi = &data_1e14d4 - 0x12;
  edi = &data_1e14d4 + 0x12;
  ebx = 0;
  eax = 0;

  jump_dccbb:
  
  *(data_1e14d4 + eax) = *(data_1e08d4 + ebx);
  *(data_1e14d8 + eax) = *(data_1e08dc + ebx);

  if (ecx != 0) 
    { // inlined jump_dcd04:
      *(data_1e14e0 + eax) = esi; /* ->prev */
    }
  else
    *(data_1e14e0 + eax) = NULL; /* ->prev */
  
  do
  { //jump_dccdd:
    ebx += 0xc;
    esi += 0x12;
    *(data_1e14dc + eax) = edi; /* ->next */
    *(data_1e14e5 + eax) = (char)ecx; // cl
    eax += 0x12;
    edi += 0x12;
    ecx ++;
    *(data_1e14d2 + eax) = 0; // dl
  }
  while(ebp < ecx); // goto jump_dccbb;
  // cmp    %ebp,%ecx, jge -> goto jump_dcd0c

  jump_dcd0c:
  edx = ebp-1; /* table index */
  eax = edx*8;
  edx += eax;
  *(data_1e14dc + eax * 2) = NULL; /* ->next */
  eax = *esp_a8;
  eax >>= 0x10;
  if(ax != 0)
  {
    /* DOS_Registers is D32A_DDREGS in WATCOM */   
    DOS_Registers lregs, loregs;
    lregs.r32.eax = 0x101;         /* DPMI 0.9+ - FREE DOS MEMORY BLOCK */
    lregs.r32.edx = eax & 0xffff;  /* selector of block */
    
    __int386_(0x31, &lregs, &loregs);
  }

  //jump_dcd51:
  /*
  esp += 0xb0;
  pop    %ebp
  pop    %edi
  pop    %esi
  pop    %edx
  pop    %ecx
  pop    %ebx*/
  return 1; // eax = 1
}
