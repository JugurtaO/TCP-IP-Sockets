
#include "../headers/serveur.h"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "nombre d'arguments invalides\n");
        fprintf(stderr, "Usage ./mainServer port path/file.txt\n");
        
    }

    // récupération du port depuis les arguments à l'appel du programme
    int port = atoi(argv[1]);
    while (port < 3000)
    {
        printf("Please choose a port higher than or equal to 3000 \n");
        scanf("%d", &port);
    }

    char *sncf = argv[2];

    struct sockaddr_in sockAddr;
    int socketServeur = initServeur(&sockAddr, port);
    printf("Server successfully configured!\n");
    int errlisten = listen(socketServeur, 10);
    printf("App runs on port %d\n", port);
    if (errlisten == -1)
    {
        perror("Listen : ");
        exit(0);
    }

    // Prêt à recevoir les requêtes éternellement
    mainloop(socketServeur, &sockAddr, sncf);

    

    return 0;
}