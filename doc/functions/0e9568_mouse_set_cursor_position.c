/*
 * This function gets passed display_width/2, display_height/2, perhaps to put
 * the cursor in the centre of the screen.  Thus maybe it is used to position
 * the cursor?
 */
int
e9568_set_pointer_position (uint32_t *x, uint32_t *y)
{
  if (!mouse_installed)
    return -1;

  /* 
   * this is some kind of mutex/lock to prevent the interrupt handler from
   * interfering, i think
   */
  data_1e50bc = 1; /* 32bit */

  /* 
   * This function refers to display_buffer, it may have something to do with
   * drawing the pointer.
   */
  if (data_1e5e94 == 0) /* 32bit */
    func_e9e58 (1);

  e99e4_correct_to_within_bounds (&x, &y);

  1e2ea4_x       = x;
  1e2e9c_press_x = x;
  1e2ea8_y       = y;
  1e2ea0_press_y = y;

  /* 
   * This function refers to display_buffer, it may have something to do with
   * drawing the pointer.
   */
  if (data_1e5e94 == 0) /* 32bit */
    func_e9ba0 ();

  data_1e50bc = 0; /* 32bit */

  return 1;
}
