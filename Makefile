CC     = gcc
CFLAGS = -Wall -Wextra

%.exe: %.c rbt.c
	$(CC) $(CFLAGS) $< rbt.c -o $@

clean:
	del *.exe *.o