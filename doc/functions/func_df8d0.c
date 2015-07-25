/* 
 * 1. copy filename to result
 * 2. if filename is a relative path (no '/' or '\\'), return
 * 3. if the first component of the path is one of several known components
 *    that are marked for replacement, replace it with (char *) &data_177754;
 */
void
func_df8d0 (const char *filename, char *result)
{
  esi = filename;
  edi = result;

  strcpy (result, filename);

  for (esi = result; *esi != '\0'; esi++)
    {
      if (*esi == '/')
	break;
    }

  if (*esi == '\0')
    esi = NULL;

  if (esi == 0)
    {
      for (esi = result; *esi != '\0'; esi++)
	{
	  if (*esi == '\\')
	    break;
	}

      if (*esi == '\0')
	esi = NULL;
    }

  if (esi == NULL)
    return;

  edx = *(const char **) data_153328;

  if (edx == NULL)
    return;

  esi = 0;

  do
    {
      edi = *(const char **) (data_153328 + esi)
      eax = strncmp (filename, edi, strlen (edi) - 1);

      if (eax == 0)
	{
	  if (*(data_153328 + esi + 4) == '\0')
	    return;

	  strcpy (result, &data_177754);
	  strcpy (result + strlen (result) - 1, filename);
	  return;
	}

      esi += 5;
      ebx = *(const char **) (data_153328 + esi);
    }
  while (ebx != NULL);
}
