CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -O2
CPPFLAGS = -I.
LDLIBS = -lncursesw

BIN_DIR = bin
OBJ_DIR = obj
UI_DIR = ../../2/2.1/ui
VPATH = ../4.1:../../2/2.1

TARGET = $(BIN_DIR)/phonebook

SRCS = main.c priority_queue.c linked_list.c contact.c

DEPS = priority_queue.h ../4.1/linked_list.h ../../2/2.1/contact.h

OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

$(OBJ_DIR)/%.o: %.c $(DEPS)
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

format:
	clang-format --style=file -i main.c priority_queue.c ../4.1/linked_list.c \
		../../2/2.1/contact.c $(DEPS)

.PHONY: all clean format
