CC		= gcc
CFLAGS		= -g

OBJECTS = dt.c

all: $(OBJECTS)
	$(CC) $(CFLAGS) -Wall -o dt dt.c

clean:
	rm dt
