uint32_t
func_10f160 (void)
{
  eax = *((uint32_t *) 0x46c);

  if (eax < data_1ed5e4)
    data_1ed5e0 += data_1ed5e4 - eax;

  data_1ed5e4 = eax;

  return ((eax + data_1ed5e0) * 0xaba / 0x32);
}
