#
# @file  Makefile
# @author ESEOBOT (inspired by Matthias Brun's work)
# @version 0.1
# @date 29/04/2020
# @brief Defined set of tasks to be executed
#

#
# Root makefile for project sources.
#

#
# sources organizations
#

PACKAGES = com_Pocket
PACKAGES+= com_Race
# PACKAGES+= astar

# PACKAGES+= watchdog

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
	@for p in $(PACKAGES); do (cd $$p; $(MAKE) $@); done
	
.c.o:
	$(CC) -c $(CCFLAGS) $< -o $@
	
# Clean.
.PHONY: clean

clean:
	@for p in $(PACKAGES); do (cd $$p; $(MAKE) $@); done
	@rm -f $(DEP)
	@rm -f $(OBJ) $(DEP)

-include $(DEP)

