CC		= gcc
CFLAGS		= -g
TARGET		= dt
OBJECTS		= dt.o
.SUFFIXES: .c .o

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

.c.o: dt.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	/bin/rm -f *.o $(TARGET)
