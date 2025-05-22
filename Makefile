CC       = gcc
CFLAGS   = -std=c11 -g -fno-common -Wall -Wno-switch
SRCDIR   = src
BUILDDIR = build
SRCS     = $(wildcard $(SRCDIR)/*.c)
OBJS     = $(SRCS:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
TARGET   = $(BUILDDIR)/rvcc

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

rvcc: $(TARGET)

run: $(TARGET)
	./$(TARGET)

test: rvcc
	@bash ./tests/test.sh

clean:
	@rm -rf $(BUILDDIR)
	@rm -f *.s a.out ./tests/tmp*

.PHONY: run clean
