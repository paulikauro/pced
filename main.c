/* main.c */

#include <stdio.h>

#include "ui.h"
#include "buffer.h"

int main()
{
    fclose(stderr);
    stderr = fopen("stderr.log", "w+");
    fprintf(stderr, "\nStarting pced\n");
    
    buffer *buf = buffer_alloc();
    ui_init(buf);

    int state = 1;
    while (state) {
	state = ui_update();
    }

    ui_free();
    buffer_free(buf);
}