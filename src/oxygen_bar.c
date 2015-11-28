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
		oxygenbar->rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));

		oxygenbar->rect->x = 180;
		oxygenbar->rect->y = 455;

		oxygenbar->rect->w = 300;
		oxygenbar->rect->h = 20;

		oxygenbar->window = window;
		oxygenbar->surface = SDL_CreateRGBSurface(0, oxygenbar->rect->w,
				oxygenbar->rect->h, 32, 0, 0, 0, 0);

		oxygenbar->oxygen_rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));

		oxygenbar->oxygen_rect->x = 0;
		oxygenbar->oxygen_rect->y = 0;

		oxygenbar->oxygen_rect->w = 300;
		oxygenbar->oxygen_rect->h = 20;
		fflush(stdout);

//		} else {
//			printf("Erro ao carregar a barra de oxygenio");
//		}
	}

	return oxygenbar;
}

void OxygenBar_destroy(OxygenBar * oxygenbar) {
	free(oxygenbar->rect);
	SDL_FreeSurface(oxygenbar->surface);
	free(oxygenbar);
}

void OxygenBar_render(OxygenBar* oxygenbar, SDL_Surface * parent) {

	SDL_FillRect(oxygenbar->surface, NULL,
			SDL_MapRGB(oxygenbar->surface->format, 0xFF, 0, 0x00));

	SDL_FillRect(oxygenbar->surface, oxygenbar->oxygen_rect,
			SDL_MapRGB(oxygenbar->surface->format, 0x13, 0x72, 10));

	SDL_BlitSurface(oxygenbar->surface, NULL, parent, oxygenbar->rect);
}

void OxygenBar_set_oxygen(OxygenBar * oxygenbar, float oxygen) {
	oxygenbar->oxygen_rect->w = oxygenbar->rect->w * (oxygen / 100.0);
}

