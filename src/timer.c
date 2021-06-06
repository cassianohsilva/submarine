/*
 * timer.c
 *
 *  Created on: 22 de nov de 2015
 *      Author: cassiano
 */

#include "timer.h"

Timer * Timer_create() {

  Timer * timer = (Timer *) malloc(sizeof(Timer));

  if (timer) {
    timer->start_ticks = 0;
    timer->paused_ticks = 0;
    timer->is_paused = false;
    timer->is_started = false;
  }

  return timer;
}

void Timer_start(Timer * timer) {
  if (timer != NULL) {
    timer->is_started = true;
    timer->is_paused = false;
    timer->start_ticks = SDL_GetTicks();
    timer->paused_ticks = 0;
  }
}

void Timer_stop(Timer * timer) {
  if (timer != NULL) {
    timer->is_started = false;
    timer->is_paused = false;
    timer->start_ticks = 0;
    timer->paused_ticks = 0;
  }
}

void Timer_pause(Timer * timer) {
  if (timer != NULL) {
    if (timer->is_started && !timer->is_paused) {
      timer->is_paused = true;
      timer->paused_ticks = SDL_GetTicks() - timer->start_ticks;
      timer->start_ticks = 0;
    }
  }
}

void Timer_unpause(Timer * timer) {
  if (timer) {
    if (timer->is_started && timer->is_paused) {
      timer->is_paused = false;
      timer->start_ticks = SDL_GetTicks() - timer->paused_ticks;
      timer->paused_ticks = 0;
    }
  }
}

Uint32 Timer_get_ticks(Timer * timer) {
  Uint32 time = 0;
  if (timer && timer->is_started) {
    if (timer->is_paused) {
      time = timer->paused_ticks;
    } else {
      time = SDL_GetTicks() - timer->start_ticks;
    }
  }
  return time;
}

bool Timer_is_started(Timer * timer) {
  bool b = timer && timer->is_started;

  return b;
}
bool Timer_is_paused(Timer * timer) {
  bool b = timer && timer->is_paused && timer->is_started;

  return b;
}

void Timer_destroy(Timer * timer) {
  free(timer);
}
