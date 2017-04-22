GCC = g++
LFLAGS = -Wall -g
CFLAGS = -Wall -std=c++14 -g -O0 -pedantic-errors

SRCDIR = ./src
OBJDIR = ./obj
TARDIR = ./bin

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
TARGETS = $(SOURCES:$(SRCDIR)/%.cpp=$(TARDIR)/%)

.PHONY: all dir clean
all: $(TARGETS)

$(TARDIR)/%: $(OBJDIR)/%.o
	$(GCC) $(LFLAGS) -o $@ $< 

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | dir
	$(GCC) $(CFLAGS) -c -o $@ $<

dir:
	mkdir -p $(OBJDIR) $(TARDIR)

clean:
	rm -rf $(TARDIR) $(OBJDIR) 
