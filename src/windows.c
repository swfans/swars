#ifdef _WIN32

#include <windows.h>
#include <stdio.h>

#include "windows.h"

#define REGISTRY_KEY "Software\\Syndicate Wars\\CurrentVersion"
#define REGISTRY_VALUE "InstallPath"

#ifndef HAVE_GETTIMEOFDAY

int
gettimeofday (struct timeval *tp, struct timezone *tzp)
{
  SYSTEMTIME st;

  /* timezone is obsolete according to UNIX man pages */
  (void) tzp;

  /* Set time */
  tp->tv_sec = (long) time (NULL);

  /* Get time info from the system */
  GetLocalTime (&st);
  tp->tv_usec = (long) st.wMilliseconds * 1000;

  /* Anti race condition sec fix
   * When the milliseconds are at 999 at the time of call to time(), and at
   * 999+1 = 0 at the time of the GetLocalTime call, then the tv_sec and
   * tv_usec would be set to one second in the past. To correct this, just
   * check if the last decimal digit of the seconds match, and if not, add
   * a second to the tv_sec.
   * See below for a race condition PoC (after 5-10 seconds it breaks the
   * loop if the below is commented out)
   */
  if (tp->tv_sec % 10 != st.wSecond % 10)
    tp->tv_sec++;

  /* return success, nothing there to fail */
  return 0;
}
#endif

bool
sys_read_string_registry_key (HKEY top_key, const char *key_name, 
                              const char *value_name,
                              char *buffer, size_t size)
{
  HKEY hkey;
  DWORD type;
  DWORD ksize = size;

  if (RegOpenKeyEx (top_key, key_name, 0, KEY_READ, &hkey) 
      != ERROR_SUCCESS)
    {
      return false;
    }

  if (RegQueryValueEx (hkey, value_name, NULL, &type, (LPBYTE) buffer,
                       &ksize) != ERROR_SUCCESS)
    {
      RegCloseKey (hkey);
      return false;
    }

  RegCloseKey (hkey);

  if (type != REG_SZ)
    return false;

  return true;
}

bool
sys_get_data_path (char *buffer, size_t size)
{
  /* Check HKCU first, we don't want to force Windows users to
   * install the game with administator privileges. */
  if (sys_read_string_registry_key (HKEY_CURRENT_USER, REGISTRY_KEY,
                                    REGISTRY_VALUE, buffer, size))
    {
      return true;
    }

  /* Fallback to default HKLM setting if HKCU is not found */
  if (sys_read_string_registry_key (HKEY_LOCAL_MACHINE, REGISTRY_KEY,
                                    REGISTRY_VALUE, buffer, size))
    {
      return true;
    }

  /* Return false */
  return false;
}

bool
sys_get_user_path (char *buffer, size_t size)
{
  const char *app_data;

  if (!sys_get_data_path (buffer, size))
    return false;

  app_data = getenv ("APPDATA");

  if (app_data == NULL)
    return false;

  snprintf (buffer, size, "%s\\Syndicate Wars", app_data);
  return true;
}
#endif
