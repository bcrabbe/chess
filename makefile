CFLAGS = -O3 -Wall -pedantic -std=c99
TARGET =  chess
SOURCES = move.c $(TARGET).c



LIBS = #-lm -framework SDL2
CC = gcc

all:
	$(CC) $(SOURCES) -o ./$(TARGET) $(CFLAGS) $(LIBS)



clean:
	rm -f $(TARGET)

run: all
	$(TARGET)

	$(TARGET)
