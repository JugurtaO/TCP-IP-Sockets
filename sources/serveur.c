#include "../headers/serveur.h"
#include "../headers/sncf.h"
// handler to wait a child when he died
void end_child()
{
    int myPid = wait(NULL);
    if (myPid != -1)
    {
        bool found = false;
        int i = 0;
        while (!found && i < MAX_SIZE)
        {
            if (Pid_fils[i] != NULL)
            {
                if (*(Pid_fils[i]) == myPid)
                {
                    Pid_fils[i] = NULL;
                    Number_Pid_fils--;
                }
            }
            i++;
        }
    }
    else
    {
        perror("wait :");
    }
}

void end_serveur()
{
    int i = 0;
    while (i < MAX_SIZE && Number_Pid_fils != 0)
    {
        if (Pid_fils[i] != NULL)
        {
            kill(*Pid_fils[i], SIGINT);
            Number_Pid_fils--;
        }
        i++;
    }
    // kill(getpgrp(), SIGINT);
    exit(0);
}
int initServeur(struct sockaddr_in *sockAddr, int port)
{
    // redéfinition du comportement des processus à la réception du signal SIGCHILD
    struct sigaction ac;
    ac.sa_handler = end_child;
    ac.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &ac, NULL);
    signal(SIGINT, end_serveur);
    // initialisation du tableau globale contenant les pid fils
    for (int i = 0; i < MAX_SIZE; i++)
    {
        Pid_fils[i] = NULL;
    }

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
    int pid;
    while (1)
    {
        if (Number_Pid_fils < MAX_SIZE)
        {
            int len = sizeof(struct sockaddr_in);
            printf("Waiting for connection ...\n");
            int socketService = accept(socketServeur, (struct sockaddr *)socketClient, (unsigned int *)&len);
            pid = fork();
            switch (pid)
            {
            case -1:
                perror("accept : ");
                exit(0);
            case 0:

                printf("Child process is born [ NEW USER IS SUCCESSFULLY CONNECTED ! - TOTAL USERS  = %d ] \n", Number_Pid_fils + 1);
                close(socketServeur);
                while (1)
                {

                    dialogueClient(socketService, sncf);
                }

                exit(0);

            default:
                Pid_fils[Number_Pid_fils] = (int *)malloc(sizeof(int));
                *(Pid_fils[Number_Pid_fils]) = pid;
                Number_Pid_fils++;
                close(socketService);
                break;
            }
        }
        else
        {
            printf("MAX AMOUNT OF USERS IS REACHED : %d", MAX_SIZE);
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

    read(socketService, &buffer, sizeof(int));
    switch (buffer)
    {
    case 0:
        sendAllTrains(socketService, sncf);
        break;
    case 1:
        sendTrainbyHourAndCity(socketService, sncf);
        break;
    case 2:
        sendTrainsOverSlotTime(socketService, sncf);
        break;
    case 3:
        sendTrainBy_Departure_AND_Arrival(socketService, sncf);
        // clearSocket(socketService);
        break;
    case -1:
        printf("I'm gonna die, help me father !\n");
        exit(0);
    default:
        printf("Error Connexion has been cut prematurly !\n");
        exit(0);
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
        write(socketService, allTrains[i], strLength * sizeof(char));
    }
    for (int i = 0; i < length; i++)
    {
        free(allTrains[i]);
    }
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
    getDepartureAndArrival(socketService, departure, arrival);
    listeTrains = getTrainBy_Departure_AND_Arrival(departure, arrival, sncf);
    sendListeTrains(socketService, listeTrains);
    free(departure);
    free(arrival);
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
    if (len == 0)
    {
        fprintf(stderr, "Error: No departure city has been read !\n");
        exit(0);
    }
    // lecture de departure
    read(socketService, departure, sizeof(char) * len);
    departure[len] = '\0';
    len = 0;
    // lecture de la taille de arrival
    read(socketService, &len, sizeof(int));
    if (len == 0)
    {
        fprintf(stderr, "Error: No arrival city has been read\n");
        exit(0);
    }
    // lecture de arrival
    read(socketService, arrival, sizeof(char) * len);
    arrival[len] = '\0';
    len = 0;
    // lecture de la taille de limit1
    read(socketService, &len, sizeof(int));
    if (len == 0)
    {
        fprintf(stderr, "Error : No time limit1 has been read\n");
        exit(0);
    }
    // lecture de limit1
    read(socketService, born1, sizeof(char) * len);
    born1[len] = '\0';
    len = 0;
    // lecture de la taille de limit2
    read(socketService, &len, sizeof(int));
    if (len == 0)
    {
        fprintf(stderr, "Error : No time limit2 has been read\n");
        exit(0);
    }
    // lecture de limit2
    read(socketService, born2, sizeof(char) * len);
    born2[len] = '\0';

    Trains = getTrainsOverSlotTime(departure, arrival, born1, born2, sncf);
    sendListeTrains(socketService, Trains);

    free(departure);
    free(arrival);
}
void sendTrainbyHourAndCity(int socketService, char *sncf)
{
    char *departure = (char *)malloc(sizeof(char) * 50);
    char *arrival = (char *)malloc(sizeof(char) * 50);
    char *hour = (char *)malloc(sizeof(char) * 50);
    char *Train;

    getDepartureAndArrival(socketService, departure, arrival);
    int len = 0;
    // lecture de la taille de hour
    read(socketService, &len, sizeof(int));
    // lecture de hour
    read(socketService, hour, sizeof(char) * len);
    hour[len] = '\0';
    Train = getTrainByGivenDepartureCity(sncf, departure, arrival, hour);
    int lenTrain = strlen(Train);

    // sending the size of the string
    write(socketService, &lenTrain, sizeof(int));

    // sending the string (train -> full line of sncf.txt file)

    write(socketService, Train, lenTrain * sizeof(char));
    free(departure);
    free(arrival);
    free(hour);
}
void getDepartureAndArrival(int socketService, char *departure, char *arrival)
{
    int len = 0;
    // lecture de la taille de departure
    read(socketService, &len, sizeof(int));
    if (len == 0)
    {
        fprintf(stderr, "Error : No departure city has been read\n");
        exit(0);
    }
    // lecture de departure
    read(socketService, departure, sizeof(char) * len);
    departure[len] = '\0';
    len = 0;
    // lecture de la taille de arrival
    read(socketService, &len, sizeof(int));
    if (len == 0)
    {
        fprintf(stderr, "Error : No arrival city has been read\n");
        exit(0);
    }
    // lecture de arrival
    read(socketService, arrival, sizeof(char) * len);
    arrival[len] = '\0';
}