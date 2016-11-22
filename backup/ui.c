#include <ncurses.h>

#include "ui.h"
#include "buffer.h"

buffer *b;

int ui_init(buffer *buf)
{
    b = buf;
    initscr();
    raw();
    keypad(stdstr, TRUE);
    noecho();
}

void ui_update()
{
    /* handle input */
    int ch = getch();

    
    /* update the screen */
    refresh();
}

void ui_free()
{
    endwin();
}
