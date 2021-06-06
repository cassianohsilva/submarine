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

      Uint32 rmask, gmask, bmask, amask;

      rmask = 0xff000000;
      gmask = 0x00ff0000;
      bmask = 0x0000ff00;
      amask = 0x000000ff;

      menu->surface = SDL_CreateRGBSurface(0, surface->w, surface->h, 32,
          rmask, gmask, bmask, amask);
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
      menu->inputs = List_create();
      menu->labels = List_create();
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

void Menu_add_label(Menu * menu, Label * label) {
  if (label) {
    List_insert(menu->labels, label);
  }
}

void Menu_add_input(Menu * menu, Input * input) {
  if (input) {
    List_insert(menu->inputs, input);
  }
}

void render_buttons(Menu * menu, SDL_Surface* parent) {
  Node * node = menu->buttons->begin;

  while (node != NULL) {
    Button * button = (Button *) node->value;

    Button_render(button, menu->surface);

    node = node->next;
  }
}

void render_inputs(Menu * menu, SDL_Surface* parent) {
  Node * node = menu->inputs->begin;

  while (node != NULL) {
    Input * input = (Input *) node->value;

    Input_render(input, menu->surface);

    node = node->next;
  }
}

void render_labels(Menu * menu, SDL_Surface* parent) {
  Node * node = menu->labels->begin;

  while (node != NULL) {
    Label * label = (Label *) node->value;

    Label_render(label, menu->surface);

    node = node->next;
  }
}

void Menu_render(Menu * menu, SDL_Surface* parent) {
  if (menu) {
    SDL_FillRect(menu->surface, NULL,
        SDL_MapRGBA(menu->surface->format, menu->background_color.r,
            menu->background_color.g, menu->background_color.b,
            menu->background_color.a));

    render_buttons(menu, parent);
    render_inputs(menu, parent);
    render_labels(menu, parent);

    SDL_BlitSurface(menu->surface, NULL, parent, menu->rect);
  }
}

void Menu_destroy_labels(Menu * menu) {
  if (menu && menu->labels->begin) {
    Node* actual = menu->labels->begin;
    Node * aux = NULL;
    while (actual != NULL) {

      aux = actual->next;

      if (actual->value != NULL) {
        Label_destroy((Label *) actual->value);
      }
      actual = aux;
    }

    menu->labels->begin = NULL;
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

    actual = menu->inputs->begin;
    aux = NULL;
    while (actual != NULL) {

      aux = actual->next;

      if (actual->value != NULL) {
        Input_destroy((Input *) actual->value);
      }
      actual = aux;
    }

    Menu_destroy_labels(menu);

    List_destroy(menu->buttons);
    List_destroy(menu->inputs);
    List_destroy(menu->labels);

    menu = NULL;
  }
}
