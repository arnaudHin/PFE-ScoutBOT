#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void record_audio(){

    system("arecord -D record_codec -f S16_LE -d 2 /usr/local/ai_mp1/scripts/command_record.wav");

}

static void process_inference(){
    system("python3 inference_mp1.py");
}

static void read_file(){

    u_int8_t command = 8;

    FILE* fichier = NULL;
    fichier = fopen("voice_reco_predict.txt", "r");
    if(fichier != NULL){
        int a = fscanf(fichier, "%hd,", &command);

    }
    fclose(fichier);

    printf("LABEL READEN : %d", command);

    system("rm voice_reco_predict.txt");    
    system("rm command_record.wav");
}

int main(int argc, char const *argv[])
{
    record_audio();

    process_inference();

    read_file();

    return 0;
}















