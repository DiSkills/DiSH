TARGET = dish

SRCDIR = src
OBJDIR = obj

SRCMODULES = str.c line.c
SRCS = $(addprefix $(SRCDIR)/,$(SRCMODULES))

OBJMODULES = $(SRCMODULES:.c=.o)
OBJS = $(addprefix $(OBJDIR)/,$(OBJMODULES))

CC = gcc
CFLAGS = -ggdb -Wall -ansi -pedantic

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(SRCDIR)/main.c $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

deps.mk: $(SRCS)
	$(CC) -MM $^ | sed 's|\(.*\.o:\)|$(OBJDIR)/\1|' > $@

clean:
	rm -rf $(OBJDIR) $(TARGET) deps.mk
