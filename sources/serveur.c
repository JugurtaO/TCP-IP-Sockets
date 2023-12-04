#include "../headers/serveur.h"
#include "../headers/sncf.h"
void end_child()
{
    wait(NULL);
}
int initServeur(struct sockaddr_in *sockAddr, int port)
{
    // redéfinition du comportement des processus à la réception du signal SIGCHILD
    struct sigaction ac;
    ac.sa_handler = end_child;
    ac.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &ac, NULL);

    // création de la socket d'écoute (connectée)
    int socketServeur = socket(AF_INET, SOCK_STREAM, 0);
    if (socketServeur == -1)
    {
        perror("socketServeur : ");
        exit(0);
    }
    // initialisation de l'adresse de la socket sockAddr
    if (sockAddr != NULL)
    {
        sockAddr->sin_family = AF_INET;
        sockAddr->sin_port = htons(port);
        sockAddr->sin_addr.s_addr = htonl(INADDR_ANY);
    }
    // attachement de la socket à une adresse IP et un PORT TCP
    int errBind = bind(socketServeur, (struct sockaddr *)sockAddr, sizeof(struct sockaddr_in));
    if (errBind == -1)
    {
        perror("bind : ");
        exit(0);
    }

    return socketServeur;
}

int mainloop(int socketServeur, struct sockaddr_in *socketClient, FILE *sncf)
{
    // Serveur éternel à l'écoute
    while (1)
    {
        int len = sizeof(struct sockaddr_in);
        printf("waiting for connection\n");
        int socketService = accept(socketServeur, (struct sockaddr *)socketClient, (unsigned int *)&len);
        switch (fork())
        {
        case -1:
            perror("accept : ");
            exit(0);
        case 0:

            printf("Child process is born!");
            close(socketServeur);

            dialogueClient(socketService, sncf);

            exit(0);

        default:
            close(socketService);
            break;
        }
    }
}

/* Handling User requests according to its option choice (0,1,2 or 3) and decided exchange protocol that the server and client will use for these multiple transmissions

    if option==0: See all trains
        NO READ FOR OTHER INFOS

    else if option==1 Get Train By given departure city,arrival city and departure time
        --reading in several times and before each preceded by reading the size of each data--

        read size of the departure city
        read departure city
        read size of arrival city
        read arrival city
        read size of departure time
        read departure time


    else if option==2 Get Train By given departure city,arrival city and departure slot time
        --reading in several times and before each preceded by reading the size of each data--

        read size of the departure city
        read departure city
        read size of arrival city
        read arrival city
        read size of departure slot time limit1
        read limit1
        read size of departure slot time limit2
        read limit2

    else if option==3 Get Train By given departure and arrival city with best  prices or optimal journey time
     --reading in several times and before each preceded by reading the size of each data--

        read size of the departure city
        read departure city
        read size of arrival city
        read arrival city

    else invalide option
    */
void dialogueClient(int socketService, FILE *sncf)
{
    printf("reading mode\n");
    int buffer;
    read(socketService, &buffer, sizeof(int));
    printf("I read option N° %d\n", buffer);
    switch (buffer)
    {
    case 0:
      
        char **allTrains = NULL;
        allTrains = getAllTrains(sncf);
        

        int length = 0;
        while (allTrains[length] != NULL)
        {
            length++;
        }

        // sending the size of the trains array
        write(socketService, &length, sizeof(int));

        /*here we send all trains according to the following rules:
            - sending first the size of the train (ex:44 )
            - sending the train (ex: 17564;Valence;Grenoble;6:15;7:31;17.60;REDUC)
        */

        for (int i = 0; i < length; i++)
        {
            int strLength = strlen(allTrains[i]);

            // sending the size of the string
            write(socketService, &strLength, sizeof(int));

            // sending the string (train -> full line of sncf.txt file)
            write(socketService, allTrains[i], strLength* sizeof(char));
            // printf("TRAIN N° %d | %s\n", i, allTrains[i]);
        }
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;

    default:
        // send Get Request Unhandled by the server message to the client
        break;
    }
}
