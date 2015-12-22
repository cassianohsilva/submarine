/*
 * life_surface.h
 *
 *  Created on: 22 de dez de 2015
 *      Author: cassiano
 */

#include "types.h"
#include "player.h"
#include "label.h"

#ifndef SRC_LIFE_SURFACE_H_
#define SRC_LIFE_SURFACE_H_

typedef struct {
	SDL_Window * window;
	SDL_Surface * surface;

	Label * text_label;
	SDL_Surface * icon_surface;

	SDL_Rect rect;
	Uint8 lifes_remaining;
} LifeSurface;

LifeSurface * LifeSurface_create(SDL_Window * window, const char * filename, int x, int y);

void LifeSurface_destroy(LifeSurface * life_surface);

void LifeSurface_set_lifes(LifeSurface * life_surface, Uint8 lifes);

void LifeSurface_render(LifeSurface * life_surface, SDL_Surface * parent);

void LifeSurface_destroy(LifeSurface * life_surface);

#endif /* SRC_LIFE_SURFACE_H_ */
