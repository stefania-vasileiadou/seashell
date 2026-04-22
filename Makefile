# Explicit gcc compiler
CC = gcc

# The CFLAGS variable sets compile flags for gcc:
#  -g        compile with debug information
#  -Wall     give verbose compiler warnings
#  -O0       do not optimize generated code
#  -std=c99  use the C99 standard language definition
CFLAGS = -g -Wall -O0 -std=c99

# Project files
SOURCES = shell.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = shell

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Note that the "clean" target should not create a new file

.PHONY: clean

clean:
	@rm -f $(TARGET) $(OBJECTS) core