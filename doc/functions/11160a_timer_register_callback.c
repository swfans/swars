int32_t
func_11160a_register_callback (void *function)
{
  101c00_lock_push (); /* lock mutex */

  for (offset = 0; offset < 0x3c; offset += 4)
    {
      if (*(159ed8_callback_types + offset) == 0)
	{
	  *(159ed8_callback_types + offset) = 1; /* 32bit */
	  *(159e98_callbacks + offset) = function;
	  goto end;
	}
    }

  offset = -1;

end:
  101c00_lock_pop (); /* unlock mutex */

  return offset;
}
