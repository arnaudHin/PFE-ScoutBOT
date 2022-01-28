#
# @file  Makefile
# @author nathan
# @version 1.1
# @date 02/12/2021

## ------------- PACKAGES -------------  
# Project packages (to be completed if necessary)
PACKAGES = Watchdog


## ------------- SRC files -------------  
SRC = $(wildcard */*.c)
SRC += $(wildcard *.c)

OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

# Inclusion depuis le niveau du package.
CCFLAGS += -I..

#
# Règles du Makefile.
#

# Compilation.
all:
	@for p in $(PACKAGES); do (cd $$p; $(MAKE) $@); done
	

.c.o:
	@$(CC) -c $(CCFLAGS) $< -o $@


# Nettoyage.
.PHONY: clean
clean:
	@for p in $(PACKAGES); do (cd $$p; $(MAKE) $@); done

-include $(DEP)

