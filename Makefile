CC = gcc

all: dt.c
	gcc - g -Wall -o dt dt.c

clean:
	rm dt
