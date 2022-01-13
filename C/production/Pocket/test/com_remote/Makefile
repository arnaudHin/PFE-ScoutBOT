#
# Makefile racine pour les tests du projet.
#

#
# Organisation des sources.
#
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)
GCDA = $(SRC:.c=.gcda)
GCNO = $(SRC:.c=.gcno)
# Inclusion depuis le niveau du package.
CCFLAGS += -I.. -I../.. -I../../../$(SRCDIR)

#
# Règles du Makefile.
#

# Compilation.
all: $(OBJ)

.c.o:
	$(CC) -c $(CCFLAGS) $< -o $@
	
# Nettoyage.
.PHONY: clean

clean:
	@rm -f $(OBJ) $(DEP) $(GCDA) $(GCNO)

-include $(DEP)