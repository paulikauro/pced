CFLAGS=-Wall -Wextra -Werror
LDFLAGS=-lncurses
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

all: $(OBJ)
	gcc $(LDFLAGS) -o pe $^

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) pe
