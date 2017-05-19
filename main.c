/* main.c */

#include <stdio.h>

#include "ui.h"
#include "buffer.h"

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("wrong number of arguments\n");
		printf("usage: pced <filename>\n");
		return 1;
	}
	char * filename = argv[1];
	
	fclose(stderr);
	stderr = fopen("stderr.log", "w+");
	fprintf(stderr, "\nStarting pced\n");
	
	buffer *buf = buffer_alloc();
	buffer_loadfile(buf, filename);
	ui_init(buf, filename);

	fprintf(stderr, "Entering main event loop\n");

	int state = 1;
	while (state) {
		state = ui_update();
	}

	fprintf(stderr, "Freeing resources\n");

	ui_free();
	buffer_free(buf);

	fprintf(stderr, "Stopping\n\n");
	return 0;
}
