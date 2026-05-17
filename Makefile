CC = gcc
CFLAGS = -Wall -Iinclude
SRC_DIR = src
BIN_DIR = bin
TARGET = $(BIN_DIR)/main.exe

# Lấy tất cả file .c trong src/
SRCS = main.c $(wildcard $(SRC_DIR)/*.c)

all: $(TARGET)

$(TARGET): $(SRCS)
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CC) $(SRCS) -o $(TARGET) $(CFLAGS)

clean:
	@if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)

run: all
	./$(TARGET)

.PHONY: all clean run