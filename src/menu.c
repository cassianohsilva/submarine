/*
 * menu.c
 *
 *  Created on: 14 de dez de 2015
 *      Author: cassiano
 */

#include "menu.h"

Menu * Menu_create(SDL_Window * window, SDL_Rect * size,
		SDL_Color background_color) {
	Menu * menu = (Menu *) malloc(sizeof(Menu));

	if (menu != NULL) {
		menu->window = window;

		if (size) {
			menu->surface = SDL_CreateRGBSurface(0, size->w, size->h, 32, 0, 0,
					0, 0);
		} else {
			SDL_Surface * surface = SDL_GetWindowSurface(window);

			menu->surface = SDL_CreateRGBSurface(0, surface->w, surface->h, 32,
					0, 0, 0, 0);
		}

		if (menu->surface != NULL) {

			menu->rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));

			if (size) {
				menu->rect->x = size->x;
				menu->rect->y = size->y;
			} else {
				menu->rect->x = 0;
				menu->rect->y = 0;
			}

			menu->rect->w = menu->surface->w;
			menu->rect->h = menu->surface->h;

			menu->background_color = background_color;

			menu->buttons = List_create();
		} else {
			printf("Erro ao criar o menu: %s\n", SDL_GetError());
		}
	}

	return menu;
}

void Menu_add_button(Menu * menu, Button * button) {
	if (button) {
		List_insert(menu->buttons, button);
	}
}

void Menu_render(Menu * menu, SDL_Surface* parent) {
	if (menu) {
		SDL_FillRect(menu->surface, NULL,
				SDL_MapRGBA(menu->surface->format, menu->background_color.r,
						menu->background_color.g, menu->background_color.b,
						menu->background_color.a));

		Node * node = menu->buttons->begin;

		while (node != NULL) {
			Button * button = (Button *) node->value;

			Button_render(button, menu->surface);

			node = node->next;
		}

		SDL_BlitSurface(menu->surface, NULL, parent, menu->rect);
	}
}

void Menu_destroy(Menu * menu) {
	if (menu) {
		free(menu->rect);
		SDL_FreeSurface(menu->surface);

		Node* actual = menu->buttons->begin;
		Node * aux = NULL;
		while (actual != NULL) {

			aux = actual->next;

			if (actual->value != NULL) {
				Button_destroy((Button *) actual->value);
			}
			actual = aux;
		}

		List_destroy(menu->buttons);

		menu = NULL;
	}
}
