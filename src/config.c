#include "config.h"

#include <stdlib.h>

void config_init(Config *c) {
	c->delay = 0;
	c->save_path = NULL;
	c->date_format = NULL;
	c->is_date_format_set = false;
	c->nogui = false;
}

void config_free(Config *c) {
	free(c->save_path);
	free(c->date_format);
}
