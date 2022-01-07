#
# Makefile des tests du projet.
#

#
# Organisation des sources.
#

# Packages.

PACKAGES = com_race
PACKAGES+= com_remote
PACKAGES+= localizer
PACKAGES+= pilot

#Inclure les fonctions WRAP ici
# LDWRAP = -Wl,--wrap=proxy_pocket_send_position

# # TODO : comment if run race_protocol
LDWRAP+= -Wl,--wrap=convert_uint16_to_2_bytes


# LDWRAP+= -Wl,--wrap=read


#méthode bouchonnée conditionnée
#
#

# Un niveau de package est accessible.
SRC = $(wildcard */*.c)		
SRC += $(wildcard */*/*.c)		
SRC += $(wildcard */*/*/*.c)		

OBJ = $(SRC:.c=.o)
# Point d'entrée du programme.
MAIN = starter_test.c

# Gestion automatique des dépendances.
DEP = $(MAIN:.c=.d)

# Executable à générer
EXEC = ../$(TEST)

# Inclusion depuis le niveau du package.
CCFLAGS += -I. -I$(CMOCKA)/include 
CCFLAGS += -DNDEBUG # avec debuggage : -DDEBUG | sans debuggage : -DNDEBUG

# Liens pour les tests.
LDFLAGS += -L$(CMOCKA)/lib 


CCFLAGS += -ftest-coverage -fprofile-arcs
# Utilisation de la librairie static CMocka
LDFLAGS += -lcmocka-static
LDFLAGS += -lgcov
#LDFLAGS += -lcmocka

# Règles du Makefile.
#

# Compilation.
all: 
	@for p in $(PACKAGES); do (cd $$p && $(MAKE) $@); done
	@$(MAKE) $(EXEC)

$(EXEC): $(OBJ) $(MAIN)
	$(CC) --coverage $(CCFLAGS) $(LDWRAP) $(OBJ) $(MAIN) -MF $(DEP) -o $(EXEC) $(LDFLAGS)

# Nettoyage.
.PHONY: clean

clean:
	@for p in $(PACKAGES); do (cd $$p && $(MAKE) $@); done
	@rm -f $(DEP)

-include $(DEP)

