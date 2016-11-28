/* buffer.h */
#ifndef BUFFER_H
#define BUFFER_H

typedef struct buffer
{
	char *ptr;
	int size;
	int gap_start;
	int gap_end;
} buffer;

buffer* buffer_alloc();
void buffer_free(buffer *b);

int buffer_loadfile(buffer *b, char *filename);
int buffer_savefile(buffer *b, char *filename);

void buffer_insertch(buffer *b, char c, int id);
void buffer_insertstr(buffer *b, char *c, int count, int id);
void buffer_delete(buffer *b, int id, int count);

char buffer_getch(buffer *b, int id);

#endif /* BUFFER_H */
