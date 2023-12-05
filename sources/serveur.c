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

int mainloop(int socketServeur, struct sockaddr_in *socketClient, char *sncf)
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

            printf("Child process is born!\n");
            close(socketServeur);
            while (1)
            {

                dialogueClient(socketService, sncf);
            }

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
void dialogueClient(int socketService, char *sncf)
{

    int buffer;

    printf("reading mode\n");
    read(socketService, &buffer, sizeof(int));
    printf("I read option N° %d\n", buffer);
    switch (buffer)
    {
    case 0:
        sendAllTrains(socketService, sncf);
        break;
    case 1:
        break;
    case 2:
        sendTrainsOverSlotTime(socketService, sncf);
        break;
    case 3:
        sendTrainBy_Departure_AND_Arrival(socketService, sncf);
        // clearSocket(socketService);
        break;

    default:
        // send Get Request Unhandled by the server message to the client
        break;
    }
}

void sendListeTrains(int socketService, char **allTrains)
{
    int length = 0;
    while (allTrains[length] != NULL)
    {
        length++;
    }

    // sending the size of the trains array
    printf("I do send length number of train's lines %d\n", length);
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
        printf("I send length \n");

        // sending the string (train -> full line of sncf.txt file)
        write(socketService, allTrains[i], strLength * sizeof(char));
        printf("TRAIN N° %d | %s\n", i, allTrains[i]);
        printf("i:%d<%d \n", i, length);
    }
    for (int i = 0; i < length; i++)
    {
        free(allTrains[i]);
    }

    printf("I go out of the for \n");
}

void sendAllTrains(int socketService, char *sncf)
{
    char **listeTrains;
    listeTrains = getAllTrains(sncf);
    sendListeTrains(socketService, listeTrains);
}
void sendTrainBy_Departure_AND_Arrival(int socketService, char *sncf)
{
    char **listeTrains;
    char *departure = (char *)malloc(sizeof(char) * 50);
    char *arrival = (char *)malloc(sizeof(char) * 50);
    int len = 0;
    // lecture de la taille de departure
    read(socketService, &len, sizeof(int));
    printf("I read len =%d\n", len);
    // lecture de departure
    read(socketService, departure, sizeof(char) * len);
    departure[len] = '\0';
    printf("I read departure =%s\n", departure);
    // lecture de la taille de arrival
    read(socketService, &len, sizeof(int));
    printf("I read len =%d\n", len);
    // lecture de arrival
    read(socketService, arrival, sizeof(char) * len);
    arrival[len] = '\0';
    printf("I read arrival =%s\n", arrival);
    printf("I do getTrainBy_Departure_AND_Arrival \n");
    listeTrains = getTrainBy_Departure_AND_Arrival(departure, arrival, sncf);
    printf("I do sendListtrains \n");
    sendListeTrains(socketService, listeTrains);
    printf("I did sendListtrains \n");
    free(departure);
    free(arrival);
}
void clearSocket(int socket)
{
    char buffer[1024];
    ssize_t bytesRead;

    // Utilisez une boucle pour lire et vider la socket
    do
    {
        bytesRead = recv(socket, buffer, sizeof(buffer), 0);
        if (bytesRead < 0)
        {
            perror("Erreur de lecture de la socket");
            break;
        }
    } while (bytesRead > 0);
}

void sendTrainsOverSlotTime(int socketService, char *sncf)
{

    char **Trains;
    char *departure = (char *)malloc(sizeof(char) * 50);
    char *arrival = (char *)malloc(sizeof(char) * 50);
    char *born1 = (char *)malloc(sizeof(char) * 50);
    char *born2 = (char *)malloc(sizeof(char) * 50);

    int len = 0;
    // lecture de la taille de departure
    read(socketService, &len, sizeof(int));
    printf("I read len =%d\n", len);
    // lecture de departure
    read(socketService, departure, sizeof(char) * len);
    departure[len] = '\0';
    printf("I read departure =%s\n", departure);

    // lecture de la taille de arrival
    read(socketService, &len, sizeof(int));
    printf("I read len =%d\n", len);
    // lecture de arrival
    read(socketService, arrival, sizeof(char) * len);
    arrival[len] = '\0';
    printf("I read arrival =%s\n", arrival);

    // lecture de la taille de limit1
    read(socketService, &len, sizeof(int));
    printf("I read len =%d\n", len);
    // lecture de limit1
    read(socketService, born1, sizeof(char) * len);
    born1[len] = '\0';
    printf("I read born1 =%s\n", born1);

    // lecture de la taille de limit2
    read(socketService, &len, sizeof(int));
    printf("I read len =%d\n", len);
    // lecture de limit2
    read(socketService, born2, sizeof(char) * len);
    born2[len] = '\0';
    printf("I read born2 =%s\n", born2);

    Trains = getTrainsOverSlotTime(departure, arrival, born1, born2, sncf);
    sendListeTrains(socketService, Trains);
    printf("I did sendListTrains \n");
    free(departure);
    free(arrival);
}