/*
 * Button.c
 *
 *  Created on: 14 de dez de 2015
 *      Author: cassiano
 */

#include "button.h"

Button * Button_create(SDL_Window * window, const char * filename, void (* on_click)(void * data)) {
	Button * button = (Button *) malloc(sizeof(Button));

	if (button != NULL) {
		button->window = window;
		button->surface = IMG_Load(filename);

		if (button->surface != NULL) {

			button->rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));

			button->rect->x = 0;
			button->rect->y = 0;

			button->rect->w = button->surface->w;
			button->rect->h = button->surface->h;

			button->on_click = on_click;
		} else {
			printf("Erro ao carregar a imagem \'%s\': %s\n", filename,
			IMG_GetError());
		}
	}

	return button;
}

void Button_set_postition(Button * button, int x, int y) {
	if(button) {
		button->rect->x = x;
		button->rect->y = y;
	}
}

void Button_on_click(Button * button, void * data) {
	if(button && button->on_click) {
		(* button->on_click)(data);
	}
}

bool Button_was_click(Button * button, int x, int y) {

	if(button) {
		if((x > button->rect->x && x < button->rect->x + button->rect->w) && (y > button->rect->y && y < button->rect->y + button->rect->h)) {
			return true;
		}
	}

	return false;
}

void Button_render(Button * button, SDL_Surface* parent) {
	SDL_BlitSurface(button->surface, NULL, parent, button->rect);
}

void Button_destroy(Button * button) {

	if(button) {
		free(button->rect);
		SDL_FreeSurface(button->surface);
		button = NULL;
	}
}
