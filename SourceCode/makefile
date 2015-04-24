CFLAGS = `sdl2-config --cflags` -std=c99 -lm -pedantic -Wall
TARGET = turtle
SOURCES = main.c
LIBS = `sdl2-config --libs`
INCLUDES = turtle_helper.c turtle_execute.c turtle_graphics.c
CC = gcc


all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(SOURCES) $(INCLUDES) -o $(TARGET) $(CFLAGS) $(LIBS)

