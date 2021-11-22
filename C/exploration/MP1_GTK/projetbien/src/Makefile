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

# Project packages (to be completed if necessary)
PACKAGES = pocket

# A package level is accessible
SRC  = $(wildcard */*.c)
SRC  += $(wildcard */*/*.c)

# To add a second level :		
# SRC += $(wildcard */*/*.c)

OBJ = $(SRC:.c=.o)

# Program entry point
MAIN = starter.c

# Automatic management of dependencies
DEP = $(MAIN:.c=.d)

# Executable to generate
EXEC = ../$(PROG)

# Inclusion from the package level.
CCFLAGS += -I.

#
# Rules of Makefile.
#

# Compilation.
all:
	@for p in $(PACKAGES); do (cd $$p; $(MAKE) $@); done
	@$(MAKE) CCFLAGS="$(CCFLAGS)" LDFLAGS="$(LDFLAGS)" $(EXEC)

$(EXEC): $(OBJ) $(MAIN)
	$(CC) $(CCFLAGS) $(OBJ) $(MAIN) -MF $(DEP) -o $(EXEC) $(LDFLAGS)

# Clean.
.PHONY: clean

clean:
	@for p in $(PACKAGES); do (cd $$p; $(MAKE) $@); done
	@rm -f $(DEP)

-include $(DEP)
