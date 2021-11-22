#
# @file  Makefile
# @author ESEOBOT (inspired by Matthias Brun's work)
# @version 0.1
# @date 29/04/2020
# @brief Defined set of tasks to be executed
#

#
# sources organizations.
#

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

# Inclusion from the package level.
CCFLAGS += -I..

#
# Rules of Makefile.
#

# Compilation.
all: $(OBJ)

.c.o:
	$(CC) -c $(CCFLAGS) $< -o $@
	
# Clean.
.PHONY: clean

clean:
	@rm -f $(OBJ) $(DEP)

-include $(DEP)

# PROG = gtk_glade
# SRCS = $(wildcard *.c)

# CLEANFILES = $(PROG)

# # Add / change option in CFLAGS and LDFLAGS
# CFLAGS += -Wall $(shell pkg-config --cflags gtk+-3.0)
# LDFLAGS += $(shell pkg-config --libs gtk+-3.0) -pthread -lm -lrt 

# all: $(PROG)

# $(PROG): $(SRCS)
# 	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

# clean:
# 	rm -f $(CLEANFILES) $(patsubst %.c,%.o, $(SRCS))