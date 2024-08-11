SRCMODULES = str.c line.c
OBJMODULES = $(SRCMODULES:.c=.o)

CC = gcc
CFLAGS = -ggdb -Wall -ansi -pedantic

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

dish: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

deps.mk: $(SRCMODULES)
	$(CC) -MM $^ > $@

clean:
	rm -f *.o dish deps.mk
