#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "client.h"

#define PORT_SERVER (8080)

static int mySocket;
struct sockaddr_in adressServer;

static uint8_t DataReadTab[10];
static uint8_t DataWriteTab[10];
static Data *dataReadTab1;
static Data *dataWriteTab1;

static void init()
{
    // for (size_t i = 0; i < 10; i++){
    //     DataWriteTab[i] = i+1;
    // }
}

extern int client_new()
{

    //init();

    mySocket = socket(AF_INET, SOCK_STREAM, 0);
    adressServer.sin_family = AF_INET;
    adressServer.sin_port = htons(PORT_SERVER);
    // adressServer.sin_addr.s_addr = inet_addr("192.168.72.1");
    adressServer.sin_addr.s_addr = inet_addr("192.168.72.1");
    return 1;
}

extern int client_start()
{

    int resConnect = 0;

    fprintf(stderr, "Recherche de serveur MP1 sur le port %d ...\n", PORT_SERVER);

    resConnect = connect(mySocket, (struct sockaddr *)&adressServer, sizeof(adressServer));

    if (!resConnect)
    {
        fprintf(stderr, "Client MP1 correctement connecte au serveur MP1\n\n");
    }
    else
    {
        fprintf(stderr, "erreur : %d\n\n", resConnect);
    }
    return 1;
}

extern void client_stop()
{

    close(mySocket);
}

static ssize_t client_read(uint8_t *dataRead, ssize_t size)
{
    int resRead = 0;
    // resRead = recv(mySocket, /*dataRead*/, 10, MSG_WAITALL);
    return resRead;
}

static ssize_t client_write(Data *dataWrite /*uint8_t *dataWrite, ssize_t size*/)
{
    int resWrite = 0;
    resWrite = write(mySocket, dataWrite, sizeof(dataWrite));
    return resWrite;
}

extern void client_run_write(Direction direction)
{
    dataWriteTab1->dir = direction;
    client_write(dataWriteTab1 /*DataWriteTab, 10*/);
    fprintf(stderr, "Client MP1 envoie au serveur MP1 : ");
    // for (size_t i = 0; i < 10; i++)
    // {
    //     fprintf(stderr, "%d, ", DataWriteTab[i]);
    // }
}

extern void client_run_read()
{
    client_read(DataReadTab, 10);
    fprintf(stderr, "Client MP1 recoit du serveur MP1 : ");
    for (size_t i = 0; i < 10; i++)
    {
        fprintf(stderr, "%d, ", DataReadTab[i]);
    }
}

// int main(int argc, char const *argv[])
// {
//     client_new();

//     client_start();

//     client_run();

//     client_stop();

//     return 0;
// }