#define IS_PRESSED(x) ((x & 0x80) == 0)
#define KEY_INDEX(x) (x & 0x7f)

/*
 * This is the interrupt handler that is called whenever IRQ 9 is triggered by
 * the keyboard to signal that data is ready.  It keeps some state:
 *
 * g 1e3074_key          - the currently read key (which can be on or off),
 * l 1c4774_last_input   - the previously read key,
 * g 1e2f70_gray_prefix  - whether the current key is prefixed by 0xe0,
 * g 1e2f74_key_table    - 256-byte table of key states (on/off and flags)
 * l 1e3075_key_flags    - the flags of the current key
 * l 1e3076_prev_key     - the previous pressed key
 * l 1e3077_prev_flags   - the flags of the previous pressed key
 *
 * Also:
 * g 1c4778_key_buffer   - a ring buffer of 16 last pressed, non-prefixed keys,
 * 			   uint32_t
 * g 1c47b8_buffer_index - the current write index of the ring buffer
 * g 1c47bc_buffer_end   - the end of the ring buffer
 */
  
void interrupt
func_8c578 (void)
{
  uint8_t key_index;

  /* read key from keyboard */
  1e3074_key = inp (0x60); /* 8bit */

  /* if prev key is "gray"-prefix */
  if (1c4774_last_input == 0xe0)
    {
      1e2f70_gray_prefix = true; /* 8bit */

      /* if left shift or "self-test complete" */
      if (1e3074_key == 0x2a || 1e3074_key == 0xaa)
	{
	  1e3074_key = 0x80; /* 8bit */
	  key_index = 0x80;
	}
      else
	{
	  key_index = 1e3074_key | 0x80; /* 8bit */
	  1e2f74_key_table[key_index] = IS_PRESSED (1e3074_key);
	}
    }
  else
    {
      1e2f70_gray_prefix = false;
      key_index = KEY_INDEX (1e3074_key);
      1e2f74_key_table[key_index] = IS_PRESSED (1e3074_key);

      if (IS_PRESSED (1e3074_key))
	{
	  /* 
	   * Some sort of (ring?) buffer stuff going on here,
	   * with start-of-buffer at 0x1c4778, current write index in
	   * 1c47b8_buffer_index and buffer end in 1c47bc_buffer_end
	   */

	  *(1c4778_key_buffer + 1c47b8_buffer_index * 4)
	    = key_index; /* 32bit */

	  ebx = (1c47b8_buffer_index + 1) & 0xf;

	  if (ebx != 1c47bc_buffer_end)
	    1c47b8_buffer_index = ebx;
	}
    }

  1c4774_last_input = 1e3074_key; /* 8bit */

  /* 
   * Set flags that will go into the key-table along with the on-off key
   * state, if the key is on.  The variables that are checked here are not
   * understood yet.
   */
  1e3075_key_flags = 0;

  if (data_1e2f9e != 0 || data_1e2faa != 0) /* 8bit */
    1e3075_key_flags |= 0x10; /* 8bit */

  if (data_1e2f91 != 0 || data_1e3011 != 0)
    1e3075_key_flags |= 0x20; /* 8bit */

  if (data_1e2fac != 0 || data_1e302c != 0)
    1e3075_key_flags |= 0x40;

  if (1e2f74_key_table[key_index] != 0) /* if pressed */
    1e2f74_key_table[key_index] |= 1e3075_key_flags;

  if (!IS_PRESSED (1e3074_key) || 1e3076_prev_key != 0)
    return;

  1e3076_prev_key   = 1e3074_key;
  1e3077_prev_flags = 1e3075_key_flags;
}
