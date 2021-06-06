/*
 * label.c
 *
 *  Created on: 22 de dez de 2015
 *      Author: cassiano
 */

#include "label.h"

Label * Label_create(SDL_Window * window, TTF_Font * font, char * text, int x,
    int y) {
  Label * label = (Label *) malloc(sizeof(Label));

  if (label) {
    label->surface = TTF_RenderText_Solid(font, text, (SDL_Color ) {0xFF, 0xFF, 00});
    label->rect = (SDL_Rect) {x, y, label->surface->w, label->surface->h};
  }

  return label;
}

void Label_render(Label * label, SDL_Surface * parent) {
  SDL_BlitSurface(label->surface, NULL, parent, &label->rect);
}

void Label_destroy(Label * label) {
  SDL_FreeSurface(label->surface);
}
