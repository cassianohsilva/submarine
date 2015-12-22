/*
 * menu.h
 *
 *  Created on: 14 de dez de 2015
 *      Author: cassiano
 */

#include "types.h"
#include "linked_list.h"
#include "button.h"
#include "input.h"

#ifndef SRC_MENU_H_
#define SRC_MENU_H_

typedef struct {
	SDL_Window * window;
	SDL_Surface * surface;
	SDL_Rect * rect;
	SDL_Color background_color;
	List * buttons;
	List * inputs;
	List * labels;
} Menu;

Menu * Menu_create(SDL_Window * window, SDL_Rect * size, SDL_Color background_color);

void Menu_add_button(Menu * menu, Button * button);

// void Menu_add_label(Menu * menu, Button * button);

void Menu_add_input(Menu * menu, Input * input);

void Menu_render(Menu * menu, SDL_Surface* parent);

void Menu_destroy(Menu * menu);

#endif /* SRC_MENU_H_ */
