#include <ncurses.h>

#include "ui.h"
#include "buffer.h"

/* private function definitions */
int ui_input();
void ui_refresh();

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
    int s = ui_input();
    ui_refresh();
    return s;
}

void ui_free()
{
    endwin();
}

/* private function implementations */

int ui_input()
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
	if (buffer_getch(b, curid - 1)  == '\0') curid--;
	break;
    case KEY_LEFT:
	if (curid == 0) break;
	curid--;
	break;
    case KEY_BACKSPACE:
	if (curid == 0) break;
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
    return s;
}

void ui_refresh()
{
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
	    break;
	default:
	    mvaddch(cy, cx, c);
	    cx++;
	    break;
	}

	if (cx > cols) {
	    cy++;
	    cx = 0;
	}

	if (i == curid - 1) {
	    ccx = cx;
	    ccy = cy;
	}
	
	i++;
	c = buffer_getch(b, i);
    }

    /* status bar */
    clrtobot();
    attron(A_STANDOUT);
    mvprintw(rows - 1, 0, "gap_start: %d gap_end: %d x: %d y: %d curid: %d", b->gap_start, b->gap_end, ccx, ccy, curid);
    clrtoeol();
    attroff(A_STANDOUT);

    move(ccy, ccx);
    
    refresh();
}
