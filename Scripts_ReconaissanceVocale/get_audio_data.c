
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define RECORD_ON_MP1   1

#define NUM_START_RECORD 0
#define NUM_STOP_RECORD 100


typedef enum{
    AVANCE=0,
    RECULE,
    DROITE,
    GAUCHE,
    STOP
}VocalCommande_e;

char * VocalCommade[5] = {"AVANCE", "RECULE", "DROITE", "GAUCHE", "STOP"};
char * pathCommand[5] = {"avance/avance", "recule/recule", "droite/droite", "gauche/gauche", "stop/stop"};

static char* concat(const char *s1, const char *s2)
{
    char * result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}



static void get_audio(VocalCommande_e commande){

    char wav[5] = ".wav";
#ifdef RECORD_ON_MP1
    char begincommand[40] = "arecord -D record_codec -f S16_LE -d 2 ";
#else //record on PC
    char begincommand[25] = "arecord -d 3 -D default ";
#endif
    
    char * file_n = calloc(1, 25);
    char * index_char = calloc(1, 2);
    char * path = pathCommand[commande];
    char a;

    printf("\n------- Commande %d : %s -------\n", commande, VocalCommade[commande]);

    printf("\n Taper ENTREE pour continuer\n");
    a = getchar();

    for (uint16_t i = NUM_START_RECORD; i < NUM_STOP_RECORD; i++){

        sprintf(index_char, "%d", i);
        file_n = concat(path, index_char);
        file_n = concat(file_n, wav );
        char * systcommd = concat(begincommand, file_n);

        printf("%s\n", systcommd);
        system(systcommd);
    }

    free(file_n);
    free(index_char);
}



int main(int argc, char const *argv[])
{
    printf("\n");
    printf("*******************************************************\n");
    printf("*******          Bienvenu cher developpeur      *******\n");
    printf("*******************************************************\n\n");

    printf("La commande vocale du robot sera basée sur un algorithme de reconnaissance vocale,\n\
    lui-même basé sur une méthode d'apprentissage supervisé utilisant des réseaux de neurones.\n\
    Afin de réaliser la phase d'entrainement de ce réseau de neurones, nous devons construire un modèle \n\
    et lui l'entrainener avec des echantillons audios des différentes commandes de pilotage du robot.\n");
    printf("Tu vas devoir enregistrer 100 échantillons de 3 secondes pour chacune des 5 commandes du robot : \n\
    Avance, Recule, Droite, Gauche, Stop.");
    printf("Pour cela, ce script va te guider afin d'enregistrer ces audios.\n");
    
    printf("\n NOTE : \n 1 - n'hésite pas à faire varier le ton/vitesse de ta voix lorsque tu \n\
    prononce le mot. Cela permet un modèle de reconnaissance plus robuste ! \n");

    get_audio(AVANCE);
    get_audio(RECULE);
    get_audio(DROITE);
    get_audio(GAUCHE);
    get_audio(STOP);


    printf("\n ___ Arnaud HINCELIN - ScoutBOT ___ \n");

    return 0;
}






