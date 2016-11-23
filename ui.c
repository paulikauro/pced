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
	if (buffer_getch(b, curid) == '\0') curid--;
	break;
    case KEY_LEFT:
	curid--;
	break;
    case KEY_BACKSPACE:
	buffer_delete(b, curid, 1);
	curid--;
	break;
    case KEY_ENTER:
	buffer_insertch(b, '\n', curid);
	curid++;
	break;
    default:
	buffer_insertch(b, (char)ch, curid);
	curid++;
	break;
    }

    /* update the screen */
    int i = 0;
    int cy = 0, cx = 0;
    int ccx = 0, ccy = 0;
    char c = buffer_getch(b, i);
    while (c != '\0') {
	switch(c) {
	case '\t':
	    mvaddstr(cy, cx, "    ");
	    cx += 4;
	    break;
	case '\n':
	    clrtoeol();
	    cx = 0;
	    cy++;
	default:
	    mvaddch(cy, cx, c);
	    cx++;
	}

	if (cx > cols) {
	    cy++;
	    cx = 0;
	}

	if (i == curid) {
	    ccx = cx;
	    ccy = cy;
	}
	
	i++;
	c = buffer_getch(b, i);
    }
    move(ccy, ccx - 1);

    refresh();
    
    return s;
}

void ui_free()
{
    endwin();
}
