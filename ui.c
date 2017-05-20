#include <ncurses.h>

#include "ui.h"
#include "buffer.h"

/* private function definitions */
int ui_input();
void ui_refresh();

buffer *b;
int rows, cols;
int curid;
char * fn;
int ss_id;

void ui_init(buffer *buf, char * filename)
{
	b = buf;
	curid = 0;
	fn = filename;
	ss_id = 0;
	
	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();

	getmaxyx(stdscr, rows, cols);
	ui_refresh();
}

int ui_update()
{
	int s = ui_input();
	ui_refresh();
	return s;
}

void ui_free()
{
	clear();
	refresh();
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
	case KEY_F(1):
		buffer_savefile(b, fn);
		break;
	case KEY_RIGHT:
		if (buffer_getch(b, curid) != '\0') curid++;
		break;
	case KEY_LEFT:
		if (curid == 0) break;
		curid--;
		break;
	case KEY_DOWN:
		while (buffer_getch(b, curid) != '\n' && buffer_getch(b, curid) != '\0') {
			curid++;
		}
		if (buffer_getch(b, curid) != '\0') curid++;
		break;
	case KEY_UP:
		while (curid > 0 && buffer_getch(b, curid - 1) != '\n') {
			curid--;
		}
		if (curid > 0) curid--;
		break;
	case KEY_BACKSPACE:
		if (curid == 0) break;
		buffer_delete(b, curid, 1);
		curid--;
		break;
	case KEY_DC:
		if (buffer_getch(b, curid + 1) != '\0') {
			buffer_delete(b, curid + 1, 1);
		}
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
	int i = ss_id;
	int cy = 0, cx = 1;
	int ccx = cx, ccy = cy;
	
	char c = buffer_getch(b, i);
	while (c != '\0') {
		switch(c) {
		case '\t':
			mvaddstr(cy, cx, "	  ");
			cx += 4;
			break;
		case '\n':
			move(cy, cx);
			clrtoeol();
			cx = 1;
			cy++;
			break;
		default:
			mvaddch(cy, cx, c);
			cx++;
			break;
		}

		if (cx == cols) {
			cy++;
			cx = 1;
			mvaddch(cy, 0, '\\');
		}

		if (i == curid - 1) {
			ccx = cx;
			ccy = cy;
		}
		
		i++;
		c = buffer_getch(b, i);
	}

	if (curid < ss_id) {
		ss_id = curid;
		while (buffer_getch(b, ss_id - 1) != '\n' && ss_id > 0) {
			ss_id--;
		}
	}
	if (ccy == rows - 2) ss_id = curid;
	
	/* status bar */
	clrtobot();
	attron(A_STANDOUT);
	mvprintw(rows - 1, 0, "ESC=exit (no save) F1=save");
	clrtoeol();
	attroff(A_STANDOUT);

	move(ccy, ccx);
	
	refresh();
}
