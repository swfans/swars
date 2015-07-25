/*
 * Values interrupt routine is called with:.
 * AX = condition mask 
 * BX = button state.
 * CX = cursor column.
 * DX = cursor row.
 * SI = horizontal mickey count.
 * DI = vertical mickey count
 *
 * Bitfields for condition mask:
 * Bit(s)  Description     (Table 03171)
 * 0      call if mouse moves
 * 1      call if left button pressed
 * 2      call if left button released
 * 3      call if right button pressed
 * 4      call if right button released
 * 5      call if middle button pressed (Mouse Systems/Logitech/Genius mouse)
 * 6      call if middle button released (Mouse Systems/Logitech/Genius mouse)
 * 7-15   unused
 */
void interrupt
func_e9638 (void)
{
  ebp_24 = condition_mask;     /* 32bit */
  ebp_20 = horiz_mickey_count; /* 32bit */
  ebp_1c = vert_mickey_count;  /* 32bit */

  if (!mouse_installed)
    return;

  if (condition_mask != 1)
    {
      ecx = condition_mask;
      if ((condition_mask & 0x2) != 0) /* if left button pressed */
	{
	  1e2ebe_left_pressed = 1; /* 8bit */

	  if (data_1e2ebb == 0) /* 8bit */
	    {
	      data_1e2ebb = 1; /* 8bit */
	      data_1e2ec1 = 0; /* 8bit */
	      1e2e9c_press_x = 1e2ea4_x; /* 32bit */
	      1e2ea0_press_y = 1e2ea8_y; /* 32bit */
	    }
	}

      if ((condition_mask & 0x4) != 0) /* if left button released */
	{
	  1e2ebe_left_pressed = 0; /* 8bit */

	  if (data_1e2ec1 == 0) /* 8bit */
	    {
	      data_1e2ec1 = 1; /* 8bit */
	      1e2eac_release_x = 1e2ea4_x; /* 32bit */
	      1e2eb0_release_y = 1e2ea8_y; /* 32bit */
	    }
	}

      if ((condition_mask & 0x8) != 0) /* if right button pressed */
	{
	  1e2ebf_right_pressed = 1; /* 8bit */

	  if (data_1e2ebc == 0) /* 8bit */
	    {
	      data_1e2ebc = 1; /* 8bit */
	      data_1e2ec3 = 0; /* 8bit */
	      1e2e9c_press_x = 1e2ea4_x; /* 32bit */
	      1e2ea0_press_y = 1e2ea8_y; /* 32bit */
	    }
	}

      if ((condition_mask & 0x10) != 0) /* if right button released */
	{
	  1e2ebf_right_pressed = 0; /* 8bit */

	  if (data_1e2ec3 == 0) /* 8bit */
	    {
	      data_1e2ec3 = 1; /* 8bit */
	      1e2eac_release_x = 1e2ea4_x; /* 32bit */
	      1e2eb0_release_y = 1e2ea8_y; /* 32bit */
	    }
	}

      if ((condition_mask & 0x20) != 0) /* if middle button pressed */
	{
	  1e2ec0_middle_pressed = 1; /* 8bit */
	  
	  if (data_1e2ebd == 0) /* 8bit */
	    {
	      data_1e2ebd = 1; /* 8bit */
	      data_1e2ec2 = 0; /* 8bit */
	      1e2e9c_press_x = 1e2ea4_x; /* 32bit */
	      1e2ea0_press_y = 1e2ea8_y; /* 32bit */
	    }
	}

      if ((condition_mask & 0x40) != 0) /* if middle button released */
	{
	  1e2ec0_middle_pressed = 0; /* 8bit */
	  if (data_1e2ec2 == 0) /* 8bit */
	    {
	      data_1e2ec2 = 1; /* 8bit */
	      1e2eac_release_x = 1e2ea4_x; /* 32bit */
	      1e2eb0_release_y = 1e2ea8_y; /* 32bit */
	    }
	}
    }

  if ((condition_mask & 0x1) == 0)
    return;

  1e307c_prev_x_delta_hires = 1e50c4_x_delta_hires; /* 16bit */
  1e307e_prev_y_delta_hires = 1e50c6_y_delta_hires; /* 16bit */
  1e50c4_x_delta_hires = horiz_mickey_count; /* 16bit */
  1e50c6_y_delta_hires = vert_mickey_count; /* 16bit */
  1e50c8_x_delta = 0; /* 32bit */
  1e50cc_y_delta = 0; /* 32bit */

  /* XXX: hmm, bug? shouldn't data_1e307a be 1e307e_prev_y_delta_hires? */
  hires_delta_x = (int16_t) 1e50c4_x_delta_hires - (data_1e307a >> 16); /* 32bit */
  if (hires_delta_x != 0)
    {
      if (hires_delta_x > 0) /* !(ZF=1 || SF!=0) */
	{
	  hires_delta_x += 1e3080_x_hires_remainder;

	  1e50c8_x_delta = hires_delta_x / 1e40ac_x_units_per_pixel;
	  1e3080_x_hires_remainder = hires_delta_x % 1e40ac_x_units_per_pixel;
	}
      else
	{
	  1e50c8_x_delta = hires_delta_x / 1e40ac_x_units_per_pixel;
	  1e3080_x_hires_remainder += hires_delta_x % 1e40ac_x_units_per_pixel;

	  if (1e3080_x_hires_remainder < 0) /* !(SF=0) */
	    {
	      1e50c8_x_delta--;
	      1e3080_x_hires_remainder =
		  1e40ac_x_units_per_pixel
		  + 1e3080_x_hires_remainder % 1e40ac_x_units_per_pixel;
	    }
	}

      data_1e50d0 = 1; /* 32bit */
    }

  hires_delta_y = (int16_t) 1e50c6_y_delta_hires
                  - (1e307c_prev_x_delta_hires >> 16); /* 32bit */
  if (hires_delta_y != 0)
    {
      if (hires_delta_y > 0)
	{
	  hires_delta_y += 1e3084_y_hires_remainder;

	  1e50cc_y_delta = hires_delta_y / 1e40b0_y_units_per_pixel;
	  1e3084_y_hires_remainder = hires_delta_y % 1e40b0_y_units_per_pixel;
	}
      else
	{
	  1e50cc_y_delta = hires_delta_y / 1e40b0_y_units_per_pixel;
	  1e3084_y_hires_remainder += + hires_delta_y % 1e40b0_y_units_per_pixel;

	  if (1e3084_y_hires_remainder < 0)
	    {
	      1e50cc_y_delta--;
	      1e3084_y_hires_remainder
		  = 1e40b0_y_units_per_pixel
		    + data_1e3084 % 1e40b0_y_units_per_pixel;
	    }
	}

      data_1e50d0 = 1; /* 32bit */
    }

  if (data_1e50bc != 0 || data_1e5e94 != 0) /* 32bit */
    return;

  1e2ea4_x += 1e50c8_x_delta; /* 32bit */
  1e2ea8_y += 1e50cc_y_delta; /* 32bit */
  corrected = e99e4_correct_to_within_bounds (&1e2ea4_x, &1e2ea8_y);
  ebp_c = (int16_t) data_1e5ea8;

  /* these two are responsible for drawing the cursor, i guess */
  func_e9e58 (corrected);
  func_e9ba0 ();

  /* check extended mode flag, whatever it is */
  if (data_1e2eba != 0) /* 8bit */
    func_eee04 ((int16_t) ebp_c);

  data_1e50d0 = 0; /* 32bit */
}
