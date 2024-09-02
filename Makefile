CC = gcc
CFLAGS = -ggdb -Wall -ansi -pedantic

BIN_DIR = bin
LIB_DIR = lib
BUILD_DIR = build

TARGET = $(BIN_DIR)/dish

SRC_DIR = src
SRC_MODULES = str.c wordlist.c line.c message.c
SRCS = $(SRC_MODULES:%=$(SRC_DIR)/%)
OBJS = $(SRC_MODULES:%.c=$(BUILD_DIR)/%.o)


# dependent directories
$(BUILD_DIR):
	mkdir -p $@

$(OBJS): | $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $@

$(TARGET): | $(BIN_DIR)

$(LIB_DIR):
	mkdir -p $@


# dependencies for objects
$(BUILD_DIR)/deps.mk: $(SRCS) | $(BUILD_DIR)
	$(CC) -MM $^ | sed 's|\(.*\.o:\)|$(BUILD_DIR)/\1|' > $@

ifneq (clean, $(MAKECMDGOALS))
-include $(BUILD_DIR)/deps.mk
endif


# build
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(SRC_DIR)/main.c $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@


# include tests
include Makefile.test

all: $(TARGET)

clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR)

.PHONY: all unittests tests clean
.DEFAULT_GOAL := all
