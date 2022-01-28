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

