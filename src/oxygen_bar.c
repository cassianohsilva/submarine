/*
 * oxygen_bar.c
 *
 *  Created on: 20 de nov de 2015
 *      Author: william
 */

#include "oxygen_bar.h"

OxygenBar * OxygenBar_create(SDL_Window * window) {
	OxygenBar * oxygenbar = (OxygenBar *) malloc(sizeof(OxygenBar));

	if (oxygenbar != NULL) {
		oxygenbar->window = window;

		if (oxygenbar->surface != NULL) {

			oxygenbar->rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));

			oxygenbar->rect->x = 0;
			oxygenbar->rect->y = 0;

			oxygenbar->rect->w = 5;
			oxygenbar->rect->h = 1;

		} else {
//			printf("Erro ao carregar a barra de oxygenio");
		}
	}

	return oxygenbar;
}

void OxygenBar_destroy(OxygenBar * oxygenbar) {
	free(oxygenbar->rect);
	SDL_FreeSurface(oxygenbar->surface);
	free(oxygenbar);
}

void OxygenBar_render(OxygenBar* oxygenbar, SDL_Surface* parent) {

	SDL_FillRect(game->surface, NULL,SDL_MapRGB(oxygenbar->surface->format, 0x00, 0x00, 0xFF));

}

void OxygenBar_set_oxygen(OxygenBar * oxygenbar, float oxygen) {

	oxygenbar->oxygen_rect.w = oxygenbar->oxygen_rect * oxygen;
	SDL_FillRect(game->surface, NULL, SDL_MapRGB(oxygenbar->surface->format, 0x00, 0x00, 0xFF));

}

