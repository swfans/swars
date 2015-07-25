#ifndef UNIX_H
#define UNIX_H

#ifdef __unix__

#include <stdbool.h>

void unix_restore_signal_handlers (void);
bool sys_get_user_path (char *buffer, size_t size);
bool sys_get_data_path (char *buffer, size_t size);

#endif

#endif
