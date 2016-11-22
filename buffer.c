/* buffer.c */

#include <stdlib.h>
#include <string.h>

#include "buffer.h"

/* private function definitions */
void gap_move(buffer *b, int index);
void gap_size(buffer *b, int size);

/* public function implementations */

buffer* buffer_alloc()
{
    /* allocate and initialize the buffer object */
    buffer *b = malloc(sizeof(buffer));
    if (b == NULL) return NULL;
    memset(b, 0, sizeof(buffer));

    /* allocate and initialize the actual buffer */
    b->size = 1024;
    b->ptr = malloc(b->size);
    if (b->ptr == NULL) {
	free(b);
	return NULL;
    }
    memset(b->ptr, 0, b->size);

    b->gap_start = 0;
    b->gap_end = b->size - 1;

    return b;
}

void buffer_free(buffer *b)
{
    /* free the actual buffer */
    free(b->ptr);
    /* free the buffer object */
    free(b);
}

void buffer_insertch(buffer *b, char c, int id)
{
    /* prepare the gap */
    gap_move(b, id);
    gap_size(b, 1);
    b->gap_start += 1;

    b->ptr[id] = c;
}

void buffer_insertstr(buffer *b, char *c, int count, int id)
{
    /* prepare the gap */
    gap_move(b, id);
    gap_size(b, count);
    b->gap_start += count;

    memcpy(b->ptr + id, c, count);
}

void buffer_delete(buffer *b, int id, int count)
{
    /* prepare the gap */
    gap_move(b, id);
    b->gap_start -= count;
}

char buffer_getch(buffer *b, int id)
{
    if (b->gap_start > id) return b->ptr[id];
    int gapsize = b->gap_end - b->gap_start;
    if (b->size - gapsize <= id) return '\0';
    return b->ptr[gapsize + id];
}
/* private function implementations */

void gap_move(buffer *b, int index)
{
    if (b->gap_start == index) return;
    if (index >= b->size) return;

    int gap_size = b->gap_end - b->gap_start;
    
    if (index < b->gap_start) {
	int mvsize = b->gap_start - index;
	memmove(b->ptr + b->gap_end - mvsize, b->ptr + index, mvsize);
	b->gap_start = index;
	b->gap_end = index + gap_size;
    } else {
	int mvsize = index - b->gap_end;
	memmove(b->ptr + b->gap_start, b->ptr + b->gap_end, mvsize);
	b->gap_start = index;
	b->gap_end = index + gap_size;
    }
}

/* increase buffer size if needed */
void gap_size(buffer *b, int size)
{
    int gapsize = b->gap_end - b->gap_start;
    if (gapsize >= size) return;

    int oldsize = b->size;
    int othersize = b->size-gapsize;
    int newsize = b->size * 2;
    while (newsize-othersize < size) newsize *= 2;

    char *newptr = realloc(b->ptr, newsize);
    if (newptr == NULL) return;
    b->ptr = newptr;
    b->size = newsize;

    /* adjust the gap */
    int mvsize = oldsize - b->gap_start;
    memmove(b->ptr + b->size - mvsize, b->ptr + b->gap_end, mvsize);
    b->gap_end = b->size - mvsize;
}
