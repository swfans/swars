int
ailimpl_install_dig_ini (SoundPCMDriver **drv)
{
  //sub    $0x118,%esp
  
  if (ail_api_read_ini (&esp_4, &data_1e5a30_filename) == 0)
    {
      memcpy (&data_1ed6f0, &data_161a58, 0x1d);
      return 1;
    }

    *drv = sound_install_dig_driver_file (&esp_84, &esp_100);

    if (*drv == NULL)
      return 2;

    return 0;
}
