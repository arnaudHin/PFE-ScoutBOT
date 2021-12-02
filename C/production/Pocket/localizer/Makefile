#
# @file  Makefile
# @author Arnaud
# @version 1.1
# @date 23/11/2021

## ------------- PACKAGES -------------  
# Project packages (to be completed if necessary)
PACKAGES = cartographer
PACKAGES += mapping
PACKAGES += positioning

## ------------- SRC files -------------  
SRC = $(wildcard */*.c)
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

