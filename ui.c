#include <ncurses.h>

#include "ui.h"
#include "buffer.h"

buffer *b;
int rows, cols;
int curid;

void ui_init(buffer *buf)
{
    b = buf;
    curid = 0;
    
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    getmaxyx(stdscr, rows, cols);
}

int ui_update()
{
    int s = 1;
    /* handle input */
    int ch = getch();

    switch (ch) {
    case 27:
	s = 0;
	break;
    case KEY_RIGHT:
	curid++;
	break;
    case KEY_LEFT:
	curid--;
	break;
    default:
	buffer_insertch(b, (char)ch, curid);
	curid++;
	break;
    }

    /* update the screen */
    int i = 0;
    int cy = 0, cx = 0;
    char c = buffer_getch(b, i);
    while (c != '\0') {
	mvaddch(cy, cx, c);
	cx++;
	if (cx > cols) {
	    cy++;
	    cx = 0;
	}
	i++;
	c = buffer_getch(b, i);
    }

    refresh();
    
    return s;
}

void ui_free()
{
    endwin();
}
