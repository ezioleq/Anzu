#include "save.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

char *get_filename(const char *date_format) {
	time_t current_time = time(0);
	struct tm *time_info;
	
	const char buffer_size = 64;
	char *filename = malloc(buffer_size);

	time_info = localtime(&current_time);
	strftime(filename, buffer_size, date_format, time_info);
	strcat(filename, ".png");

	return filename;
}
