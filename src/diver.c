/*
 * diver.c
 *
 *  Created on: 26 de nov de 2015
 *      Author: cassiano
 */

#include "diver.h"

Diver * Diver_create(SDL_Window * window, float movement_factor, Direction direction, int y_position) {

	Diver * diver = (Diver *) malloc(sizeof(Diver));

	if(diver != NULL) {
		diver->surface = IMG_Load(RES_DIVER);

		if(diver != NULL) {
			diver->movement_factor = movement_factor;
			diver->direction = direction;
			diver->rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));

			if(diver->rect != NULL) {

				diver->rect->x = direction == LEFT ? SDL_GetWindowSurface(window)->w : 0;
				diver->rect->y = y_position;
				diver->rect->w = diver->surface->w;
				diver->rect->h = diver->surface->h;
			}
		}
	}

	return diver;
}

void Diver_move(Diver * diver) {
	if(diver != NULL) {
		diver->rect->x += (int) (diver->direction * diver->movement_factor);
	}
}

void Diver_change_direction(Diver * diver) {
	if(diver != NULL) {
		diver->direction = -diver->direction;
	}
}

void Diver_render(Diver * diver, SDL_Surface * parent) {
	SDL_BlitSurface(diver->surface, NULL, parent, diver->rect);
}

void Diver_destroy(Diver * diver) {
	if(diver != NULL) {
		free(diver->rect);
		SDL_FreeSurface(diver->surface);
		free(diver);
	}

	diver = NULL;
}
