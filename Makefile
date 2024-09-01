CC = gcc
CFLAGS = -ggdb -Wall -ansi -pedantic

BUILD_DIR = build
BIN_DIR = bin

SRC_DIR = src
SRC_MODULES = str.c wordlist.c line.c message.c
SRCS = $(SRC_MODULES:%=$(SRC_DIR)/%)

include Makefile.prod
include Makefile.test

all: $(TARGET) $(TEST_TARGET)

clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR)

.PHONY: all unittests tests clean
.DEFAULT_GOAL := all
