bool
func_10f2f0_lock_pages (void *ptr0, void *ptr1)
{
  DOS_Registers iregs; /* esp + 0 */
  DOS_Registers oregs; /* esp + 1c */

  start = MIN (ptr0, ptr1);

  if (ptr0 >= ptr1)
    end = ptr0;

  iregs.r32.eax = 0x600;
  iregs.r32.ebx = start >> 16;
  iregs.r32.ecx = start & 0xffff;
  iregs.r32.esi = (end - start + 1) >> 16;
  iregs.r32.edi = (end - start + 1) & 0xffff;

  dos_int386 (0x31, &iregs, &oregs);

  return (oregs.r32.eflags == 0);
}
