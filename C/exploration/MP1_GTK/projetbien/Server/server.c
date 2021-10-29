#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include "commun.h"

#define PORT_SERVER (8080)

static int socketServer;
static int socketClient;
struct sockaddr_in myAdress;

static uint8_t DataReadTab[10];
static uint8_t DataWriteTab[10];
static Data *dataReadTab1;
static Data *dataWriteTab1;
static void init()
{
    for (size_t i = 0; i < 10; i++)
    {
        DataWriteTab[i] = 99 + i;
    }
}

extern int server_new()
{

    init();

    socketServer = socket(AF_INET, SOCK_STREAM, 0);
    myAdress.sin_family = AF_INET;
    myAdress.sin_port = htons(PORT_SERVER);
    myAdress.sin_addr.s_addr = htonl(INADDR_ANY);

    return bind(socketServer, (struct sockaddr *)&myAdress, sizeof(myAdress));
}

extern int server_start()
{
    listen(socketServer, 1);
    fprintf(stderr, "Serveur MP1 en attente de client au port %d...\n", PORT_SERVER);

    socketClient = accept(socketServer, NULL, 0);
    fprintf(stderr, "Client MP1 connecte !\n");

    return 1;
}

static ssize_t server_read(Data *dataRead /*uint8_t * dataRead, ssize_t size*/)
{
    int resRead = 0;
    resRead = recv(socketClient, dataRead, sizeof(dataRead) /*10*/, MSG_WAITALL);
    return resRead;
}

static ssize_t server_write(uint8_t *dataWrite, ssize_t size)
{
    int resWrite = 0;
    //resWrite = write(socketClient, dataWrite, 10);
    return resWrite;
}

static void server_run_read()
{
    // server_read(DataReadTab, 10);
    server_read(dataReadTab1);
    fprintf(stderr, "Server MP1 recoit du client MP1 : ");
    // for (size_t i = 0; i < 10; i++)
    // {
    //     fprintf(stderr, "%d, ", DataReadTab[i]);
    // }
    fprintf(stderr, "%d, ", dataWriteTab1->dir);
}

static void server_run_write()
{
    // server_write(DataWriteTab, 10);
    //server_write(dataWriteTab1);
    fprintf(stderr, "Server MP1 envoie au client MP1 : ");
    for (size_t i = 0; i < 10; i++)
    {
        fprintf(stderr, "%d, ", DataWriteTab[i]);
    }
}

extern int server_stop()
{

    close(socketServer);

    close(socketClient);

    return 1;
}

int main(int argc, char const *argv[])
{
    server_new();

    server_start();

    server_run_read();

    server_stop();

    return 0;
}