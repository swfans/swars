#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

#define TIMER_MAX_CALLBACKS 16

typedef void (*TimerCallback) (void *user_data);
typedef int32_t TimerId;

TimerId timer_register_callback (TimerCallback fn);
void *timer_set_user_data (TimerId timer, void *user_data);

#endif
