CC = gcc
CFLAGS = -ggdb -Wall
CPPFLAGS = -I./ -isystem/usr/local/include

LDFLAGS = -L/usr/local/lib

BIN_DIR = bin
BUILD_DIR = build

TARGET = $(BIN_DIR)/dish

SRC_DIR = src
SRCS = $(filter-out $(SRC_DIR)/main.c, $(wildcard $(SRC_DIR)/*.c))
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)


# dependent directories
$(BUILD_DIR):
	mkdir -p $@

$(OBJS): | $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $@

$(TARGET): | $(BIN_DIR)


# dependencies for objects
$(BUILD_DIR)/deps.mk: $(SRCS) | $(BUILD_DIR)
	$(CC) -MM $^ | sed 's|\(.*\.o:\)|$(BUILD_DIR)/\1|' > $@

ifneq (clean, $(MAKECMDGOALS))
-include $(BUILD_DIR)/deps.mk
endif


# build
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/%.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -c $< -o $@

$(TARGET): $(SRC_DIR)/main.c $(OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o $@


# include tests
include Makefile.test

all: $(TARGET)

clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR)

submodules:
	git submodule update --init --recursive

.PHONY: all unittests clean unity submodules
.DEFAULT_GOAL := all
