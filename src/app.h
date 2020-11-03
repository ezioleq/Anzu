#ifndef APP_H
#define APP_H

#include <stdbool.h>
#include "config.h"

typedef struct App {
	int argc;
	char **argv;
	Config cfg;
	bool running;
	/* To be continued */
} App;

void app_run(App *app, int argc, char** argv);
void app_init(App *app);
void app_update(App *app);
void app_draw(App *app);
void app_close(App *app);

#endif
