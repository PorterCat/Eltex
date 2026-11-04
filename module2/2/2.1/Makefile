CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -O2
CPPFLAGS = -I.
LDLIBS = -lncursesw

BIN_DIR = bin
OBJ_DIR = obj
TARGET = $(BIN_DIR)/phonebook

SRCS = main.c contact.c phonebook.c storage.c \
       ui/ui.c ui/contact_list.c ui/contact_form.c \
       ui/contact_card.c ui/contact_properties.c

DEPS = contact.h phonebook_api.h phonebook.h storage.h \
          ui/ui.h ui/contact_list.h ui/contact_form.h \
          ui/contact_card.h ui/contact_properties.h

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
