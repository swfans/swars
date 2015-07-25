/*
 * Closes a file.
 * Returns 1 on success, -1 on error
 */
int
func_e8a1c (int fd)
{
  close (fd);

  if (eax != 0)
    return -1;

  return 1;
}
