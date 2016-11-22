/* buffer.h */
#ifndef BUFFER_H
#define BUFFER_H

struct buffer
{
    char *ptr;
    int size;
    int gap_start;
    int gap_end;
};

buffer* buffer_alloc();
void buffer_free(buffer *b);

void buffer_insert(buffer *b, int id, char c);
void buffer_delete(buffer *b, int id);

void buffer_move(int id);

#endif /* BUFFER_H */
