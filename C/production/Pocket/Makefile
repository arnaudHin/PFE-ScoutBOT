#
# @file  Makefile
# @author ESEOBOT (inspired by Matthias Brun's work)
# @version 0.1
# @date 29/04/2020
# @brief Defined set of tasks to be executed
#

## ------------- PACKAGES -------------  
# Project packages (to be completed if necessary)
PACKAGES = pilot
PACKAGES += com_remote
PACKAGES += com_race
#PACKAGES += localizer

## ------------- SRC files -------------  
# A package level is accessible
SRC  = $(wildcard */*.c)
# To add a second level :		
#SRC  += $(wildcard */*/*.c)

OBJ = $(SRC:.c=.o)


## ------------- MAIN file -------------  
# Program entry point
MAIN = starter.c
MAIN_OBJ = $(MAIN:.c=.o)
# Automatic management of dependencies
DEP = $(MAIN:.c=.d)


## ------------- EXEC file -------------  
# Executable to generate
EXEC = bin/Pocket_new.elf

## ------------- FLAGS -------------  
# Inclusion from the package level.
# Inclusion depuis le niveau du package.
export CCFLAGS  = -O0 
export CCFLAGS += -DNDEBUG
export CCFLAGS += -MMD -MP # Gestion automatique des dépendances.
export CCFLAGS += -D_BSD_SOURCE -D_XOPEN_SOURCE_EXTENDED -D_XOPEN_SOURCE -D_DEFAULT_SOURCE -D_GNU_SOURCE
export CCFLAGS += -std=c99 -Wall -pedantic
export CCFLAGS += -Wno-unused-function -Wno-unused-variable
export LDFLAGS  = -lpthread -lrt

export CCFLAGS += -I.


## ------------- Rules of Makefile -------------  
# Compilation
all:
	for p in $(PACKAGES); do (cd $$p; $(MAKE) $@); done
	$(MAKE) CCFLAGS="$(CCFLAGS)" LDFLAGS="$(LDFLAGS)" $(EXEC)


$(EXEC): $(OBJ) $(MAIN)
	$(CC) $(CCFLAGS) $(OBJ) $(MAIN) -MF $(DEP) -o $(EXEC) $(LDFLAGS)

$(MAIN_OBJ) : $(MAIN)
	@$(CC) $(CCFLAGS) -c $(MAIN) -o $(MAIN_OBJ)

# Clean.
.PHONY: clean

clean:
	@for p in $(PACKAGES); do (cd $$p; $(MAKE) $@); done
	@rm -f $(DEP) $(MAIN_OBJ)
	@rm -f $(EXEC)

-include $(DEP)
