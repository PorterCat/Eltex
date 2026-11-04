CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -O2
CPPFLAGS = -I.
LDLIBS = -lncursesw

BIN_DIR = bin
OBJ_DIR = obj
UI_DIR = ../../2/2.1/ui

TARGET = $(BIN_DIR)/phonebook

SRCS = main.c linked_list.c phonebook_wrapper.c \
       ../../2/2.1/contact.c \
       ../../2/2.1/storage.c \
       $(wildcard $(UI_DIR)/*.c)

DEPS = linked_list.h phonebook_wrapper.h \
       ../../2/2.1/contact.h \
       ../../2/2.1/phonebook_api.h \
       ../../2/2.1/storage.h \
       $(wildcard $(UI_DIR)/*.h)

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
	clang-format --style=file -i $(SRCS) $(DEPS)

.PHONY: all clean format
