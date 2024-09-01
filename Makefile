CC = gcc
CFLAGS = -ggdb -Wall -ansi -pedantic

BIN_DIR = bin
TARGET = $(BIN_DIR)/dish

SRC_DIR = src
SRC_MODULES = str.c wordlist.c line.c message.c
SRCS = $(SRC_MODULES:%=$(SRC_DIR)/%)

PROD_BUILD_DIR = build/prod
PROD_OBJS = $(SRC_MODULES:%.c=$(PROD_BUILD_DIR)/%.o)

all: $(TARGET)

# dependent directories for production
$(PROD_BUILD_DIR):
	mkdir -p $@

$(PROD_OBJS): | $(PROD_BUILD_DIR)

$(BIN_DIR):
	mkdir -p $@

$(TARGET): | $(BIN_DIR)

# production build
$(PROD_BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(SRC_DIR)/main.c $(PROD_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# dependencies for objects
$(PROD_BUILD_DIR)/deps.mk: $(SRCS) | $(PROD_BUILD_DIR)
	$(CC) -MM $^ | sed 's|\(.*\.o:\)|$(PROD_BUILD_DIR)/\1|' > $@

ifneq (clean, $(MAKECMDGOALS))
-include $(PROD_BUILD_DIR)/deps.mk
endif

clean:
	rm -rf $(PROD_BUILD_DIR) $(BIN_DIR)
