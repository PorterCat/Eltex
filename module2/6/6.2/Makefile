CC = gcc

CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -O2
CPPFLAGS = -I../../4/4.1 -I../../2/2.1 -I../../2/2.1/ui
LDLIBS = -lncursesw

LIST_DIR = ../../4/4.1
CORE_DIR = ../../2/2.1
UI_DIR = $(CORE_DIR)/ui

BIN_DIR = bin
OBJ_DIR = obj
LIB_DIR = lib

TARGET = $(BIN_DIR)/phonebook
LIB = $(LIB_DIR)/liblinked_list.so

SRCS = main.c phonebook_wrapper.c contact.c storage.c \
       ui.c contact_list.c contact_form.c \
       contact_card.c contact_properties.c

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
LIST_OBJ = $(OBJ_DIR)/linked_list.o

HEADERS = $(wildcard $(LIST_DIR)/*.h) \
          $(wildcard $(CORE_DIR)/*.h) \
          $(wildcard $(UI_DIR)/*.h)

vpath %.c $(LIST_DIR) $(CORE_DIR) $(UI_DIR)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS) $(LIB) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIB_DIR) \
		-Wl,-rpath,'$$ORIGIN/../lib' \
		-llinked_list -o $@ $(LDLIBS)

$(LIB): $(LIST_OBJ) | $(LIB_DIR)
	$(CC) -shared -o $@ $^

$(OBJ_DIR)/%.o: %.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(LIST_OBJ): $(LIST_DIR)/linked_list.c $(LIST_DIR)/linked_list.h | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -c $< -o $@

$(BIN_DIR) $(OBJ_DIR) $(LIB_DIR):
	mkdir -p $@

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR) $(LIB_DIR)