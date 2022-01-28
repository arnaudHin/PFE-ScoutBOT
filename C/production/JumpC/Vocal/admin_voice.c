/**
 * @file admin_voice.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "commun.h"
#include "../ComJumpC/proxy_pilot.h"


static void record_audio(){
    system("arecord -D record_codec -f S16_LE -d 2 command_record.wav");
}

static void process_inference(){
    system("python3 inference_mp1.py");
}

static Direction_e read_file(){

    uint8_t command = 8;

    FILE* fichier = NULL;
    fichier = fopen("voice_reco_predict.txt", "r");
    if(fichier != NULL){
        int a = fscanf(fichier, "%hd,", &command);

    }
    fclose(fichier);

    printf("LABEL READEN : %d", command);

    system("rm voice_reco_predict.txt");    
    system("rm command_record.wav");

    return (Direction_e) command;
}


extern void adminVoice_start() {
    record_audio();

    process_inference();

    Direction_e dir = read_file();

    proxy_pilot_set_direction(dir);

}