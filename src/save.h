#ifndef SAVE_H
#define SAVE_H

#include "config.h"

/**
 * It have to be freed later.
 * @return Filename string
 */
char *get_filename(const char *date_format);

#endif
