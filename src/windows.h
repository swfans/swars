#ifndef WINDOWS_H
#define WINDOWS_H

#ifdef _WIN32
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>

#ifndef HAVE_GETTIMEOFDAY
struct timezone
{
  int  tz_minuteswest;
  int  tz_dsttime;
};

int gettimeofday (struct timeval *tp, struct timezone *tzp);
#endif

bool sys_get_user_path (char *buffer, size_t size);
bool sys_get_data_path (char *buffer, size_t size);

#endif /* _WIN32 */

#endif /* WINDOWS_H */
