ifeq ($(OS),Windows_NT)
	EXEEXT = .exe
endif

PROGNAME := main$(EXEEXT)
SRCDIR := src
LIBDIR := lib
OUTDIR := bin
TARGET := $(OUTDIR)/$(PROGNAME)
HDRS := $(wildcard $(SRCDIR)/*.hpp)
OBJS := $(addprefix $(OUTDIR)/,$(patsubst %.cpp,%.o,$(wildcard $(SRCDIR)/*.cpp)))
DEPS := $(OBJS:%.o=%.d)
#$(warning $(OBJS))

CC = g++
CFLAGS = -Wall -Wextra -Wpedantic -O2 -std=c++17 -I $(SRCDIR)

.PHONY: all clean
all: $(TARGET)

-include $(DEPS)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OUTDIR)/%.o:%.cpp
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) -o $@ -c -MMD $<

clean:
	rm -rf $(OUTDIR)
