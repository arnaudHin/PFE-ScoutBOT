#
# @file  Makefile
# @author ESEOBOT (inspired by Matthias Brun's work)
# @version 0.1
# @date 29/04/2020
# @brief Defined set of tasks to be executed
#

#
# sources organizations
#

PROG = gtk_glade

# Project packages (to be completed if necessary)

# A package level is accessible
SRCS  = $(wildcard */*.c)

# To add a second level :		
# SRC += $(wildcard */*/*.c)

OBJ = $(SRCS:.c=.o)

# Program entry point
MAIN = gtk_glade.c

# Automatic management of dependencies
DEP = $(MAIN:.c=.d)

# Executable to generate
EXEC = ../$(PROG)

CLEANFILES = $(PROG)

# Add / change option in CFLAGS and LDFLAGS
CFLAGS += -Wall $(shell pkg-config --cflags gtk+-3.0)
LDFLAGS += $(shell pkg-config --libs gtk+-3.0) -pthread -lm -lrt

all: $(PROG)

$(PROG): $(SRCS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(CLEANFILES) $(patsubst %.c,%.o, $(SRCS))