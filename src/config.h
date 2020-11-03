#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

typedef struct Config {
	int delay;
	char *save_path;
	char *date_format;
	bool is_save_path_set;
	bool is_date_format_set;
	bool nogui;
} Config;

void config_init(Config *c);
void config_free(Config *c);

#endif
