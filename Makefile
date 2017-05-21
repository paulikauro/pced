CFLAGS=-Wall -Wextra -Werror -g
LDFLAGS=-lncurses -lm
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

all: $(OBJ)
	gcc $(LDFLAGS) -o pced $^

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) pced
