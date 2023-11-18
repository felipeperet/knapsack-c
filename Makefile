CC=gcc
CFLAGS=-Wall -g
SRCDIR=src
INCDIR=include
OBJDIR=obj
BINDIR=bin
EXE=$(BINDIR)/knapsack

SOURCES=$(wildcard $(SRCDIR)/*.c)

OBJECTS=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

all: directories $(EXE)

$(EXE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

directories:
	@mkdir -p $(SRCDIR)
	@mkdir -p $(INCDIR)
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o $(EXE)

.PHONY: run
run: all
	./$(EXE)
