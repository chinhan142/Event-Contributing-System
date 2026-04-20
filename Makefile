CC = gcc
TARGET = main

all:
	$(CC) *.c -o $(TARGET) -Wall

clean:
	rm -f $(TARGET).exe

run:
	make && ./$(TARGET)