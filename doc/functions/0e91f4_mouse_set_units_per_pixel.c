int
func_e91f4_set_units_per_pixel (uint8_t x, uint8_t y)
{
  if (!mouse_installed)
    return -1;

  if (arg0 < 1 || arg0 > 0x3f)
    return -1;

  if (arg1 < 1 || arg1 > 0x3f)
    return -1;

  1e40ac_x_units_per_pixel = arg0; /* 32bit */
  1e40b0_y_units_per_pixel = arg1;

  return 1;
}
