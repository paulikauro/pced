/* ui.h */

#ifndef UI_H
#define UI_H

struct buffer;

int ui_init(buffer *b);

int ui_update();

void ui_free();

#endif /* UI_H */
