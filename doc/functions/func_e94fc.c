int
func_e94fc (uint32_t arg0, uint32_t arg1, uint32_t width, uint32_t height)
{
  if (!mouse_installed)
    return -1;

  data_1e2e8c = arg0;
  data_1e2e90 = arg1;
  data_1e2e94 = width;
  data_1e2e98 = height;

  func_e99e4 (&data_1e2ea4, &data_1e2ea8);
  func_e99e4 (&data_1e2e9c, &data_1e2ea0);

  return 1;
}
