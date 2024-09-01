CC = gcc
CFLAGS = -ggdb -Wall -ansi -pedantic

BUILD_DIR = build
BIN_DIR = bin

SRC_DIR = src
SRC_MODULES = str.c wordlist.c line.c message.c
SRCS = $(SRC_MODULES:%=$(SRC_DIR)/%)

clean:
	rm -rf $(BIN_DIR)

include Makefile.test
