#ifndef SAVE_H
#define SAVE_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "config.h"

/* It have to be freed later.
 * @return Filename string
 */
char *get_filename(const char *date_format);

/* It have to be freed later.
 * @return Full file path string
 */
char *get_full_path(const char *save_path, const char *filename);

/* It have to be freed later.
 * @return Default save path string
 */
char *get_default_save_path();

bool directory_exists(const char *path);

SDL_Surface *get_cropped_surface(SDL_Surface *src, int x, int y, int w, int h);

#endif
