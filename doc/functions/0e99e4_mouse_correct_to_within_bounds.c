int
func_e99e4_correct_to_within_bounds (uint32_t *px, uint32_t *py)
{
  int corrected = 0;

  if (*px < 1e2e8c_min_x)
    {
      *px = 1e2e8c_min_x;
      corrected = 1;
    }
  else if (*px >= 1e2e8c_min_x + 1e2e94_width)
    {
      *px = 1e2e8c_min_x + 1e2e94_width - 1;
      corrected = 1;
    }

  if (*py < 1e2e90_min_y)
    {
      *py = 1e2e90_min_y;
      ebx_4 = 1;
    }
  else if (*py >= 1e2e90_min_y + 1e2e98_height)
    {
      *py = 1e2e90_min_y + 1e2e98_height - 1;
      corrected = 1;
    }

  return corrected;
}
