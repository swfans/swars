/* Initially called with:
 *
 * arg0 = 0x08270d0e (dynamically-allocated memory?)
 * units_per_pixel_x = 0x2
 * units_per_pixel_y = 0x2
 */
int
func_e926c (uint32_t arg0,
	    uint8_t units_per_pixel_x, uint8_t units_per_pixel_y)
{
  uint32_t ebp_c;
  uint8_t ebp_8;
  uint8_t ebp_4;
  DOS_Registers ebp_50;
  DOS_Registers ebp_34;
  DOS_SegmentRegisters ebp_18;

  ebp_c = arg0;
  ebp_8 = units_per_pixel_x;
  ebp_4 = units_per_pixel_y;

  if (1e3088_mouse_installed != 0) /* 32bit */
    return -1;

  if (1e50c0_mouse_handler_installed == 0) /* 32bit */
    {
      segread (&ebp_18);

      /* MS MOUSE - RESET DRIVER AND READ STATUS */
      ebp_50.r32.eax = 0;
      dos_int386 (0x33, &ebp_50, &ebp_34);

      if (ebp_34.r16.ax != 0xffff) /* if driver not installed */
	return -1;
    }

  1e3088_mouse_installed = 1; /* 32bit */
  1e40ac_x_units_per_pixel = 1; /* 32bit */
  1e40b0_y_units_per_pixel = 1; /* 32bit */
  data_1e40b4 = 0; /* 32bit */
  data_1e40b8 = 0; /* 32bit */

  memset (&data_1e40bc, 0xfe, 4096);

  data_1e2e68 = 0; /* 32bit */
  data_1e50bc = 0; /* 32bit */

  memset (&data_1e308c, 0, 4128);

  if (e94fc_set_bounds (0, 0, display_width, display_height) != 1)
    {
      1e3088_mouse_installed = 0; /* 32bit */
      return -1;
    }

  if (e91f4_set_units_per_pixel (units_per_pixel_x, units_per_pixel_y) != 1)
    {
      1e3088_mouse_installed = 0; /* 32bit */
      return -1;
    }

  /* set pointer position? */
  if (e9568_set_pointer_position (display_width / 2, display_height / 2) != 1)
    {
      1e3088_mouse_installed = 0; /* 32bit */
      return -1;
    }

  /* set cursor animation/image? */
  if (func_e906c (arg0) != 1)
    {
      1e3088_mouse_installed = 0; /* 32bit */
      return -1;
    }

  if (1e50c0_mouse_handler_installed == 0) /* 32bit */
    {
      /* MS MOUSE v1.0+ - DEFINE INTERRUPT SUBROUTINE PARAMETERS */
      ebp_50.r16.ax = 0xc;

      /*
       * bit	description
       * 0      call if mouse moves
       * 1      call if left button pressed
       * 2      call if left button released
       * 3      call if right button pressed
       * 4      call if right button released
       * 5      call if middle button pressed (Mouse Systems/Logitech/Genius mouse)
       * 6      call if middle button released (Mouse Systems/Logitech/Genius mouse)
       */
      ebp_50.r16.cx = 0x7f; 

      /* interrupt handler */
      ebp_50.r16.dx = &func_e9638;
      ebp_18.es = cs; /* 16bit */
      dos_int386x (0x33, &ebp_50, &ebp_34, &ebp_18);
      1e50c0_mouse_handler_installed = 1;
    }

  return 1;
}
