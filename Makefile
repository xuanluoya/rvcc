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

docs:
	@if [ ! -d docs ]; then git clone https://github.com/jothepro/doxygen-awesome-css.git ./docs/css; fi
	@doxygen

clean:
	@rm -rf $(BUILDDIR)
	@rm -f *.s a.out ./tests/tmp*
	@rm -rf docs

docker:
	@docker run -u root --volume .:/rvcc --rm -it registry.cn-hangzhou.aliyuncs.com/dailycoding/rvcc

.PHONY: run clean docs docker
