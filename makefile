//aqui vai criar o jogo propriamente dito em um arquivo so
CC = gcc
CFLAGS = -Wall -Iinclude
SRCDIR = src
OBJDIR = obj
BINDIR = bin
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
EXECUTABLE = $(BINDIR)/LUNO

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
    @mkdir -p $(BINDIR)
    $(CC) $(OBJECTS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
    @mkdir -p $(OBJDIR)
    $(CC) $(CFLAGS) -c $< -o $@

clean:
    rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean
