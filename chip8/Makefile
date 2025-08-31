CC      = gcc
CFLAGS  = -Wall -std=c11 $(shell pkg-config --cflags sdl2)
LDFLAGS = $(shell pkg-config --libs sdl2)

SRC     = main.c chip8.c drawSDL.c emulator.c loader.c
TARGET  = chip8

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

