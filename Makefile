CC      = gcc
CFLAGS  = -std=c11 -g -fno-common -Wall -Wno-switch

SRCDIR  = src
BUILDDIR= build

SRCS    = $(wildcard $(SRCDIR)/*.c)
OBJS    = $(SRCS:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

TARGET  = $(BUILDDIR)/rvcc

$(TARGET): $(OBJS)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	@rm -rf $(BUILDDIR)

.PHONY: run clean
