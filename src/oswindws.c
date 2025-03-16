#ifdef WIN32

#include <windef.h>
#include <winbase.h>
#include <winreg.h>
#include <stdio.h>
#include <stdlib.h>

#include "oswindws.h"

#define REGISTRY_KEY "Software\\Syndicate Wars\\CurrentVersion"
#define REGISTRY_VALUE "InstallPath"

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

  /* Use Windows folders only if related reg keys are set.
   * If there are no keys, then both functions should refuse
   * to return a folder.
   */
  if (!sys_get_data_path(buffer, size))
    return false;

  app_data = getenv ("APPDATA");

  if (app_data == NULL)
    return false;

  snprintf (buffer, size, "%s\\Syndicate Wars", app_data);
  return true;
}
#endif  /* WIN32 */
