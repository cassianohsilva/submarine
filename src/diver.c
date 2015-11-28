/*
 * diver.c
 *
 *  Created on: 26 de nov de 2015
 *      Author: cassiano
 */

#include "diver.h"

Diver * Diver_create(SDL_Window * window, float movement_factor,
		Direction direction, int y_position) {

	Diver * diver = (Diver *) malloc(sizeof(Diver));

	if (diver != NULL) {
		diver->surface = IMG_Load(RES_DIVER);

		if (diver != NULL) {
			diver->window = window;
			diver->movement_factor = movement_factor;
			diver->direction = direction;
			diver->rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
			diver->sprite_rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));

			if (diver->sprite_rect != NULL) {

				if (direction == LEFT) {
					diver->sprite_rect->x = 0;
				} else {
					diver->sprite_rect->x = diver->surface->w >> 1;
				}
				diver->sprite_rect->y = 0;
				diver->sprite_rect->w = diver->surface->w >> 1;
				diver->sprite_rect->h = diver->surface->h;
			}

			if (diver->rect != NULL) {

				diver->rect->x =
						direction == LEFT ? SDL_GetWindowSurface(window)->w : 0;
				diver->rect->y = y_position;
				diver->rect->w = diver->surface->w >> 1;
				diver->rect->h = diver->surface->h;
			}
		}
	}

	return diver;
}

bool Diver_is_visible(Diver * diver) {
	SDL_Surface * surface = SDL_GetWindowSurface(diver->window);
	bool is_visible = true;

	SDL_Rect * enemy_rect = diver->rect;
	SDL_Rect screen_rect = { 0, 0, surface->w, surface->h };

	if (enemy_rect->x < 0 || enemy_rect->x > screen_rect.w || enemy_rect->y < 0
			|| enemy_rect->y > screen_rect.h) {
		is_visible = false;
	}

	return is_visible;
}

void Diver_move(Diver * diver) {
	if (diver != NULL) {
		diver->rect->x += (int) (diver->direction * diver->movement_factor);
	}
}

void Diver_change_direction(Diver * diver) {
	if (diver != NULL) {
		diver->direction = -diver->direction;
	}

	if (diver->direction == LEFT) {
		diver->sprite_rect->x = 0;
	} else {
		diver->sprite_rect->x = diver->surface->w >> 1;
	}
}

void Diver_render(Diver * diver, SDL_Surface * parent) {
	SDL_BlitSurface(diver->surface, diver->sprite_rect, parent, diver->rect);
}

void Diver_destroy(Diver * diver) {
	if (diver != NULL) {
		free(diver->rect);
		free(diver->sprite_rect);
		SDL_FreeSurface(diver->surface);
		free(diver);
	}

	diver = NULL;
}
