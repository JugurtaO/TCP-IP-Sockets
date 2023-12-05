#include "../headers/client.h"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "nombre d'arguments invalides\n");
        fprintf(stderr, "Usage ./mainClient hostname port \n");
        exit(0);
    }

    // récupération du port depuis les arguments à l'appel du programme
    int port = atoi(argv[2]);
    while (port < 3000)
    {
        printf("Please choose a port higher than or equal to 3000 \n");
        scanf("%d", &port);
    }

    char *hostname = argv[1];





    
    struct sockaddr_in adserv; // adresse du serveur (infos du server récupérées par gesthostbyname)

    int socketClient = initClient( &adserv,hostname,port);

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