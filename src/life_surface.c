/*
 * life_surface.c
 *
 *  Created on: 22 de dez de 2015
 *      Author: cassiano
 */

#include "life_surface.h"

LifeSurface * LifeSurface_create(SDL_Window * window, const char * filename,
    int x, int y) {
  LifeSurface * life_surface = (LifeSurface *) malloc(sizeof(LifeSurface));

  if (life_surface != NULL) {
    life_surface->window = window;

    life_surface->text_label = Label_create(window,
        TTF_OpenFont(RES_DEFAULT_FONT, 15), "LIFES", 0, 0);
    life_surface->icon_surface = IMG_Load(filename);

    int w = life_surface->text_label->rect.w + 20
        + MAX_LIFES * life_surface->icon_surface->w;
//    int h = life_surface->text_label->rect.h > life_surface->icon_surface->h ?
//        life_surface->text_label->rect.h : life_surface->icon_surface->h;
    int h = 20;

    Uint32 rmask, gmask, bmask, amask;

    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;

    life_surface->surface = SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask,
        bmask, amask);

    life_surface->lifes_remaining = MAX_LIFES;
    life_surface->rect = (SDL_Rect ) {x, y, w, h};

    SDL_Rect r_temp;
    r_temp.x = 0;
    r_temp.y = (life_surface->rect.h - life_surface->text_label->surface->h)
        / 2;
    r_temp.w = life_surface->text_label->surface->w;
    r_temp.h = life_surface->text_label->surface->h;

    SDL_BlitSurface(life_surface->text_label->surface, NULL,
        life_surface->surface, &r_temp);

    LifeSurface_set_lifes(life_surface, MAX_LIFES);
  } else {
    printf("Erro ao criar o life_surface: %s\n", SDL_GetError());
  }

  return life_surface;
}

void LifeSurface_destroy(LifeSurface * life_surface) {
  Label_destroy(life_surface->text_label);
  SDL_FreeSurface(life_surface->surface);
  SDL_FreeSurface(life_surface->icon_surface);
}

void LifeSurface_set_lifes(LifeSurface * life_surface, Uint8 lifes) {
  life_surface->lifes_remaining = lifes;

  int x = life_surface->text_label->rect.w + 20;
  int y = (life_surface->rect.h - life_surface->icon_surface->h) / 2
      + life_surface->rect.y;

  int i = 0;
  SDL_Rect r = (SDL_Rect ) {x, y, life_surface->icon_surface->w, life_surface->icon_surface->h};

  SDL_Rect clear_r = (SDL_Rect ) {x, y, life_surface->icon_surface->w * MAX_LIFES, life_surface->icon_surface->h};

  SDL_FillRect(life_surface->surface, &clear_r,
        SDL_MapRGBA(life_surface->surface->format, 0x00, 0x00, 0x00, 0x00));

  for (i = 0; i < lifes; ++i) {
    SDL_BlitSurface(life_surface->icon_surface, NULL, life_surface->surface,
        &r);

    r.x += life_surface->icon_surface->w;
  }
}

void LifeSurface_render(LifeSurface * life_surface, SDL_Surface * parent) {
  SDL_BlitSurface(life_surface->surface, NULL, parent, &(life_surface->rect));
}
