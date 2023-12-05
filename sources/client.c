#include "../headers/client.h"

int initClient(struct sockaddr_in *adserv, char *hostname, int port)
{
    struct hostent *host;
    int socketServeur = socket(AF_INET, SOCK_STREAM, 0);

    if (socketServeur == -1)
    {
        perror("socketClient : ");
        exit(0);
    }

    host = gethostbyname(hostname);
    memcpy(&(adserv->sin_addr.s_addr), host->h_addr_list[0], sizeof(unsigned long));

    adserv->sin_port = htons(port);
    adserv->sin_family = AF_INET;
    return socketServeur;
}

void dialogueServeur(int socketServeur, struct sockaddr_in *adserv)
{

    int exit = 1;
    int user_int;
    while (exit == 1)
    {
        afficherMenu();
        printf("option >>: ");
        scanf("%d", &user_int);

        switch (user_int)
        {
        case 0:
            int arrayLength = 0;
            int strLength = 0;
            char train[1024];
            // sending choosed option
            printf("I send %d\n", user_int);
            write(socketServeur, &user_int, sizeof(int));
            // reading allTrains array length
            read(socketServeur, &arrayLength, sizeof(int));
            if (arrayLength != 0)
            {
                printf("HERE IS THE LIST OF FOUND TRAINS  \n");
                for (int i = 0; i < arrayLength; i++)
                {
                    // reading size of the train number i
                    read(socketServeur, &strLength, sizeof(int));

                    // reading the value of the train number i
                    read(socketServeur, train, strLength * sizeof(char));
                    printf("%s\n", train);

                    // clean the buffer train
                    for (int j = 0; j < strLength; j++)
                    {
                        train[j] = '\0';
                    }
                }
            }
            else
            {
                printf("No train was found !");
            }
            char user_char;
            printf("press q to quit : \n");
            scanf("%c", &user_char);
            while (user_char != 'q')
            {
                scanf("%c", &user_char);
            }
            printf("***********EXIT = %d\n", exit);
            break;
        case 3:
            getServeurTrainBy_Departure_AND_Arrival(socketServeur);
            printf("***********EXIT = %d\n", exit);
            break;
        case -1:
            exit = 0;
        default:
            break;
        }
    }
}
void afficherMenu()
{
    printf("\n");
    printf("*******Main menu*******\n");
    printf("choose an option : \n");
    printf("0 : See all trains\n");
    printf("1 : Get Train By given departure city,arrival city and departure time \n");
    printf("2 : Get Train By given departure city,arrival city and departure slot time\n");
    printf("3 : Get Train By given departure and arrival city with best  prices or optimal journey time \n");
    printf("-1: Quit\n");
    printf("***********************\n");
}
void getServeurTrainBy_Departure_AND_Arrival(int socketServeur)
{
    char **trains;
    // sending choosed option
    int user_int = 3;
    printf("I send %d\n", user_int);
    write(socketServeur, &user_int, sizeof(int));
    askAndSendDeparture(socketServeur);
    askAndSendArrival(socketServeur);
    int arraylength = 0;
    trains = getListeTrain(socketServeur, &arraylength);
    affichageTrainBy_Departure_AND_Arrival(trains, arraylength);
    free(trains);
}

char **getListeTrain(int socketServeur, int *arrayLength)
{

    int strLength = 0;

    read(socketServeur, arrayLength, sizeof(int));
    printf("I will read %d lignes\n", (*arrayLength));
    if (arrayLength != 0)
    {
        char **trainlist = (char **)malloc(sizeof(char *) * (*arrayLength));

        for (int i = 0; i < (*arrayLength); i++)
        {
            // reading size of the train number i
            read(socketServeur, &strLength, sizeof(int));
            printf("len %d \n", strLength);
            trainlist[i] = malloc(sizeof(char) * strLength);
            // reading the value of the train number i
            read(socketServeur, trainlist[i], strLength * sizeof(char));
            printf("%s\n", trainlist[i]);
        }
        return trainlist;
    }
    else
    {
        return NULL;
    }
}
void askAndSendDeparture(int socketServeur)
{
    char departure[50];
    printf("Enter departure town (and trainstation if there is more than one):\n");
    // Ask user the value of departure
    scanf("%s", departure);
    printf("I scan : %s \n", departure);
    // mesuring the length of departure
    int len = strlen(departure);
    printf("len : %d \n", len);
    // sending departure length
    write(socketServeur, &len, sizeof(int));
    // sending departure
    write(socketServeur, departure, sizeof(char) * len);
}
void askAndSendArrival(int socketServeur)
{
    char arrival[50];
    printf("Enter arrival town (and trainstation if there is more than one):\n");
    // Ask user the value of departure
    scanf("%s", arrival);
    printf("I scan : %s \n", arrival);
    // mesuring the length of departure
    int len = strlen(arrival);
    printf("len : %d \n", len);
    // sending departure length
    write(socketServeur, &len, sizeof(int));
    // sending departure
    write(socketServeur, arrival, sizeof(char) * len);
}
void affichageTrainBy_Departure_AND_Arrival(char **trains, int arraylength)
{
    int trainNumber = 0;
    char departure[50];
    char arrival[50];
    char limit1[50];
    char limit2[50];
    float price = 0.0;
    char promotion[50];

    if (arraylength<=0)
    {
        printf("No train was found !");
    }
    else
    {
        int i = 0;
        sscanf(trains[0], "%d;%[^;];%[^;];%[^;];%[^;];%f;%s", &trainNumber, departure, arrival, limit1, limit2, &price, promotion);
        printf("Here are the trains that will go from %s to %s :\n", departure, arrival);
        while (i < arraylength)
        {
            sscanf(trains[i], "%d;%[^;];%[^;];%[^;];%[^;];%f;%s", &trainNumber, departure, arrival, limit1, limit2, &price, promotion);

            if (strcmp(promotion, "REDUC"))
            {
                price *= 0.8;
            }
            else if (strcmp(promotion, "SUPPL"))
            {
                price *= 1.1;
            }

            printf("Train N°: %d leaving at %s and arriving at %s\n    price : %.2f\n", trainNumber, limit1, limit2, price);
            i++;
        }
    }

    char user_char;
    int j = 0;
    while (j < arraylength)
    {
        free(trains[j]);
        j++;
    }

    printf("press q to quit : \n");
    scanf("%c", &user_char);
    while (user_char != 'q')
    {
        scanf("%c", &user_char);
    }
}
