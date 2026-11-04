CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -O2 -I../../2/2.1
LDLIBS = -lncursesw

vpath %.c ../../2/2.1

BIN_DIR = bin
OBJ_DIR = obj

TARGET = $(BIN_DIR)/phonebook

SRCS = main.c linked_list.c phonebook_wrapper.c \
       ../../2/2.1/contact.c \
       ../../2/2.1/storage.c \
       ../../2/2.1/ui.c

DEPS = linked_list.h phonebook_wrapper.h \
       ../../2/2.1/contact.h \
       ../../2/2.1/phonebook_api.h \
       ../../2/2.1/storage.h \
       ../../2/2.1/ui.h

OBJS = $(addprefix $(OBJ_DIR)/,$(notdir $(SRCS:.c=.o)))

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