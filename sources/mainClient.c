#include "../headers/client.h"

int main(int argc, char **argv)
{
    struct hostent host;
    struct sockaddr_in adserv; //adresse du serveur (infos du server récupérées par gesthostbyname)

    int socketClient = initClient(&host, &adserv);

    int len = sizeof(struct sockaddr);
    int errConnect = connect(socketClient, (struct sockaddr *)&adserv, len);
    if (errConnect == -1)
    {
        perror("connection failed  :");
        exit(0);
    }
    printf("Successfully connected to the server!\n");
    dialogueServeur(socketClient, &adserv);
}