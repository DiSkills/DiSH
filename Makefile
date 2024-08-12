TARGET = dish

SRCDIR = src
SRCMODULES = str.c line.c
SRCS = $(addprefix $(SRCDIR)/,$(SRCMODULES))

OBJDIR = obj
OBJS = $(addprefix $(OBJDIR)/,$(SRCMODULES:.c=.o))

CC = gcc
CFLAGS = -ggdb -Wall -ansi -pedantic

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(SRCDIR)/main.c $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

deps.mk: $(SRCS)
	$(CC) -MM $^ | sed 's|\(.*\.o:\)|$(OBJDIR)/\1|' > $@

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR) $(TARGET) deps.mk $(TESTBUILDDIR)

include Makefile.test
