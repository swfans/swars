bool
10f360_unlock_pages (void *p0, void *p1)
{
  esp_0.r32.eax = 0x601;
  esp_0.r32.ebx = MIN (p0, p1) >> 16;
  esp_0.r32.ecx = MIN (p0, p1) & 0xffff;
  esp_0.r32.esi = (abs (p0 - p1) + 1) >> 16;
  esp_0.r32.edi = (abs (p0 - p1) + 1) & 0xffff;

  dos_int386 (0x31, &esp_0, &esp_1c);

  return (esp_1c.regs32.eflags == 0);
}
