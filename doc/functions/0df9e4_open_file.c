/*
 * Opens a file.  filename is transformed into another filename, and mode can
 * be one of:
 *   0 - create a new truncated file for writing
 *   1 - open an existing file for writing (not appending, not truncating)
 *   2 - open a file for reading
 */
int
func_df9e4(const char *filename, uint8_t mode)
{
  bl = dl;
  func_df8d0 (eax, esp); /* hmm, transform filename,
			    put result in (char *) esp? */
  if (bl == 0)
    {
      eax = creat (esp, O_RDONLY | O_TEXT | O_NOINHERIT | O_TRUNC); /* 0x1c0 */
      dos_setmode (eax, O_BINARY);
      close (eax);
    }

  switch (bl)
    {
    case 0:
      eax = dos_sopen (esp + 8, O_BINARY | O_CREAT | O_RDWR, 0x40); /* 0x222 */
      break;
    case 1:
      eax = dos_sopen (esp + 8, O_BINARY | O_RDWR, 0x40); /* 0x202 */
      break;
    case 2:
      eax = dos_sopen (esp + 8, O_RDONLY | O_BINARY, 0x40); /* 0x200 */
      break;
    default:
      eax = -1;
      break;
    }

  return eax;
}
