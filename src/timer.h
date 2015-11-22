/*
 * timer.h
 *
 *  Created on: 22 de nov de 2015
 *      Author: cassiano
 */

#include "types.h"

#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_

typedef struct {
	Uint32 start_ticks;
	Uint32 paused_ticks;
	bool is_paused;
	bool is_started;
} Timer;

Timer * Timer_create();

void Timer_start(Timer * timer);

void Timer_stop(Timer * timer);

void Timer_pause(Timer * timer);

void Timer_unpause(Timer * timer);

Uint32 Timer_get_ticks(Timer * timer);

bool Timer_is_started(Timer * timer);

bool Timer_is_paused(Timer * timer);

void Timer_destroy(Timer * timer);

#endif /* SRC_TIMER_H_ */
