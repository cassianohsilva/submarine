/*
 * input.c
 *
 *  Created on: 15 de dez de 2015
 *      Author: cassiano
 */

#include "input.h"

#define MAX_INPUT_SIZE 6

Input * Input_create(SDL_Window * window, TTF_Font * font, SDL_Rect * rect,
		SDL_Color background_color, SDL_Color input_color) {

	Input * input = (Input *) malloc(sizeof(Input));

	if (input != NULL) {
		input->window = window;

		if (rect) {
			input->surface = SDL_CreateRGBSurface(0, rect->w, rect->h, 32, 0, 0,
					0, 0);
		} else {
			SDL_Surface * surface = SDL_GetWindowSurface(window);

			Uint32 rmask, gmask, bmask, amask;

			rmask = 0xff000000;
			gmask = 0x00ff0000;
			bmask = 0x0000ff00;
			amask = 0x000000ff;

			input->surface = SDL_CreateRGBSurface(0, surface->w, surface->h, 32,
					rmask, gmask, bmask, amask);
		}

		if (input->surface != NULL) {

			input->rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));

			if (rect) {
				input->rect->x = rect->x;
				input->rect->y = rect->y;
			} else {
				input->rect->x = 0;
				input->rect->y = 0;
			}

			input->rect->w = input->surface->w;
			input->rect->h = input->surface->h;

			input->font = font;
			input->background_color = background_color;
			input->input_color = input_color;
			input->text = (char *) malloc(MAX_INPUT_SIZE * sizeof(char));

			strcpy(input->text, "");

			input->text_surface = TTF_RenderText_Solid(input->font, input->text,
					input->input_color);
		} else {
			printf("Erro ao criar o input: %s\n", SDL_GetError());
		}
	}

	return input;
}

void Input_set_text(Input * input, char * text) {
	if (input) {
		strncpy(input->text, text, MAX_INPUT_SIZE - 1);
	}
}

void Input_insert_text(Input * input, char * text) {
	if (input && text) {
		strncat(input->text, text, MAX_INPUT_SIZE - strlen(input->text) - 1);
	}
}

void Input_render(Input * input, SDL_Surface * parent) {
	if (input) {
		SDL_FillRect(input->surface, NULL,
				SDL_MapRGBA(input->surface->format, input->background_color.r,
						input->background_color.g, input->background_color.b,
						input->background_color.a));

		if (input->text_surface) {
			SDL_FreeSurface(input->text_surface);
			input->text_surface = NULL;
		}

		input->text_surface = TTF_RenderText_Solid(input->font, input->text,
				input->input_color);

		if (input->text_surface) {

			SDL_Rect r = { (input->surface->w - input->text_surface->w) / 2,
					(input->surface->h - input->text_surface->h) / 2,
					input->text_surface->w, input->text_surface->h };

			SDL_BlitSurface(input->text_surface, NULL, input->surface, &r);
		}

		SDL_BlitSurface(input->surface, NULL, parent, input->rect);
	}
}

void Input_destroy(Input * input) {
	if (input) {
		free(input->text);
		free(input->rect);
		free(input->text);
		SDL_FreeSurface(input->surface);

		input = NULL;
	}
}
