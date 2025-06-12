CC = gcc
CFLAGS = -Wall -mwindows
TARGET = brawlhalla_screen_lock.exe
SRC = brawlhalla_screen_lock.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	del /Q $(TARGET)