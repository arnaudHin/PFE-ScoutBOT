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

# # TODO : comment if run test of display
# LDWRAP+= -Wl,--wrap=display_send_txt_state
# LDWRAP+= -Wl,--wrap=display_send_txt_msg
# LDWRAP+= -Wl,--wrap=display_send_txt_connection

# # TODO : comment if run test of proxy_robot
# LDWRAP+= -Wl,--wrap=robot_try_mvmt
# LDWRAP+= -Wl,--wrap=robot_start_scanning
# LDWRAP+= -Wl,--wrap=robot_stop_scanning
# LDWRAP+= -Wl,--wrap=robot_urgent_break
# LDWRAP+= -Wl,--wrap=robot_reset_odometry

# # TODO : comment if run test of robot_state
# #LDWRAP+= -Wl,--wrap=robot_state_mutex_init
# #LDWRAP+= -Wl,--wrap=robot_state_mutex_destroy

# # TODO : comment if run test of pilot
# LDWRAP+= -Wl,--wrap=pilot_signal_connection_success
 					
# # TODO : comment if run test of proxy_pocket
# LDWRAP+= -Wl,--wrap=proxy_indicate_connection
# LDWRAP+= -Wl,--wrap=proxy_indicate_manual
# LDWRAP+= -Wl,--wrap=proxy_indicate_auto

# LDWRAP+= -Wl,--wrap=proxy_pocket_send_error
# LDWRAP+= -Wl,--wrap=proxy_pocket_send_map
# LDWRAP+= -Wl,--wrap=proxy_pocket_send_position

# # TODO : comment if run test of postman_pocket
# LDWRAP+= -Wl,--wrap=postman_pocket_receive_msg

# # TODO : comment if run test of display
# LDWRAP+= -Wl,--wrap=display_send_txt_state
# LDWRAP+= -Wl,--wrap=display_send_txt_msg
# LDWRAP+= -Wl,--wrap=display_send_txt_connection

# #LDWRAP+= -Wl,--wrap=robot_try_path
# #LDWRAP+= -Wl,--wrap=gtk_label_set_label

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

