CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -O2
LDLIBS = -lncursesw

BIN_DIR = bin
OBJ_DIR = obj

TARGET = $(BIN_DIR)/phonebook

SRCS = main.c phonebook.c storage.c ui.c
DEPS = phonebook.h storage.h ui.h

OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

$(OBJ_DIR)/%.o: %.c $(DEPS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

format:
	clang-format --style=file -i $(SRCS) $(DEPS)

.PHONY: all clean format