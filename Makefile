CC = gcc

OBJECTS = dt.c

all: $(OBJECTS)
	$(CC) - g -Wall -o dt dt.c

clean:
	rm dt
