#
# @file  Makefile
# @author Arnaud
# @version 1.1
# @date 23/11/2021


## ------------- SRC files -------------  
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

# Inclusion depuis le niveau du package.
CCFLAGS += -I..

#
# Règles du Makefile.
#

# Compilation.
all: $(OBJ)
.c.o:
	@$(CC) -c $(CCFLAGS) $< -o $@


# Nettoyage.
.PHONY: clean
clean:
	@rm -f $(OBJ) $(DEP) $(GCDA) $(GCNO)

-include $(DEP)

