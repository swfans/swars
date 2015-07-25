int
func_e9e58 (uint32_t arg0)
{
  if (!mouse_installed)
    return -1;

  if (data_1e409c == mouse_x && data_1e40a0 == mouse_y)
    {
      if (arg0 == NULL)
	return 1;
    }

  if (data_1e308c == 0) /* 32bit */
    return 1;

  src_ptr = &data_1e309c;

  if (!display_extended_mode) /* 8bit */
    {
      bufp = display_buffer + data_1e3098;	/* cursor location in memory */
      cur_height = data_1e3094;			/* guess: cursor height */

      for (;;)
	{
	  cur_height--; /* one row less */
	  if (cur_height == -1)
	    {
	      data_1e308c = 0; /* 32bit */
	      return 1;
	    }

	  ebp_18 = bufp;	/* beginning of cursor row */

	  for (size_t n = 0; n < data_1e3090; n++)
	    {
	      src_ptr++;
	      ebp_18++;
	      *ebp_18 = *src_ptr;		/* copy */
	    }

	  bufp += 1e2e6c_display_builtin_width;	/* screen pitch */
	}
    }
  else
    {
      ebp_4 = data_1e3098 >> 16;
      func_eee04 ((int16_t) ebp_4);
      bufp = display_buffer + (data_1e3098 & 0xffff);
      cur_height = data_1e3094;	/* cursor height? */

      for (;;)
	{
	  cur_height--;
	  if (cur_height == -1)
	    {
	      data_1e308c = 0; /* 32bit */
	      return 1;
	    }

	  ebp_24 = data_1e3090; /* cursor width */
	  ebp_18 = bufp;

	  if (ebp_18 + data_1e3090 >= 0xb000)
	    {
	      ebp_20 = 0xb0000 - ebp_18;

	      if (ebp_18 < 0xb0000)
		{
		  for (ebp_c = 0; ebp_c < ebp_20; ebp_c++)
		    {
		      src_ptr++;
		      ebp_18++;
		      *ebp_18 = *src_ptr; /* 8bit */
		    }

		  ebp_24 -= ebp_20;
		}

	      ebp_4++;
	      func_eee04 ((int16_t) ebp_4);
	      ebp_18 -= 0x10000;
	      bufp -= 0x10000;
	    }

	  for (size_t n = 0; n < ebp_24; n++)
	    {
	      src_ptr++;
	      ebp_18++;
	      *ebp_18 = *src_ptr;
	    }

	  bufp += 1e2e6c_display_builtin_width;
	}

      data_1e308c = 0; /* 32bit */
    }

  return 1;
}
