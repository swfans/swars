SoundPCMDriver *
ailimpl_install_dig_driver_file (const char *fname, IO_PARMS *iop)
{
  //sub    $0x90,%esp
  sprintf (&esp_c, "%s/%s", data_1e5be0_driver_dir, fname);

  drv_img = func_10fb20_load_driver_image (&esp_c, 0);
  if (drv_img == NULL)
    {
      memcpy (&data_1ed6f0, &data_161a40, 0x17);
      return 0;
    }

  drv = ail_install_driver (drv_img, func_10fad0_image_size (&esp_0));
  (*data_159790) (drv_img); /* free() */
  if (drv == NULL)
    return NULL;

  pcmdrv = func_113110_create_pcm_driver (drv, esp_a4); /* whatever */
  if (pcmdrv == NULL)
    ail_uninstall_driver (drv);

  return pcmdrv;
}
