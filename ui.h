/* ui.h */

#ifndef UI_H
#define UI_H

#include "buffer.h"

void ui_init(buffer *b, char *filename);

int ui_update();

void ui_free();

#endif /* UI_H */
