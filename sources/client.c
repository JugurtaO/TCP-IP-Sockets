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
    int user_int=-30;
    while (exit == 1)
    {
        afficherMenu();
        printf("option >>: ");
        clearInputBuffer();
        scanf("%d", &user_int);
        while (user_int < -1 || user_int > 3)
        {
            printf("choose valid option >>: ");
            clearInputBuffer();
            scanf("%d", &user_int);
        }

        switch (user_int)
        {
        case 0:
            getAllTrainsFromTheServer(socketServeur);
            // printf("***********EXIT = %d\n", exit);
            break;
        case 1:
            getTrainbyHourAndCityFromServer(socketServeur);
            break;
        case 2:
            getAllTrainsWithGivenSlotTime(socketServeur);
            // printf("***********EXIT = %d\n", exit);
            break;
        case 3:
            getServeurTrainBy_Departure_AND_Arrival(socketServeur);
            // printf("***********EXIT = %d\n", exit);
            break;
        case -1:
            cutConnexion(socketServeur);
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
    printf("############## DEPARTURE ##########\n");
    int departurelength=50;
    char departure[departurelength];
    int len = 0;
    while (len == 0)
    {
        for(int i=0;i<departurelength;i++){
            departure[i]='\0';
        }
        printf("Enter departure town or train station :\n");
        // Ask user the value of departure
        clearInputBuffer();
        scanf("%[^\n]s", departure);
        // fgets(departure, 50, stdin);

        printf("I scan : %s \n", departure);
        // mesuring the length of departure
        len = strlen(departure);
        printf("len : %d \n", len);
        if (departure[len - 1] == ' ')
        {
            departure[len - 1] = '\0';
            len--;
        }
    }
    // sending departure length
    write(socketServeur, &len, sizeof(int));
    // sending departure
    write(socketServeur, departure, sizeof(char) * len);
}
void askAndSendArrival(int socketServeur)
{
    printf("############## ARRIVAL ############\n");
    int arrivalLength=50;
    char arrival[arrivalLength];
    int len = 0;
    while (len == 0)
    {
        for(int i=0;i<arrivalLength;i++){
            arrival[i]='\0';
        }
        printf("Enter arrival town or train station :\n");
        // Ask user the value of departure

        clearInputBuffer();
        scanf("%[^\n]s", arrival);
        // fgets(arrival, 50, stdin);
        printf("I scan : %s \n", arrival);
        // mesuring the length of departure
        len = strlen(arrival);
        printf("len : %d \n", len);
        if (arrival[len - 1] == ' ')
        {
            arrival[len - 1] = '\0';
            len--;
        }
    }

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
    char *cheapTrain;float minprice = FLT_MAX;
    char* fastTrain; int tempsmin=__INT_MAX__;
    
    
    if (arraylength <= 0)
    {
        printf("No train was found  !");
    }
    else
    {
        int i = 0;
        sscanf(trains[0], "%d;%[^;];%[^;];%[^;];%[^;];%f;%s", &trainNumber, departure, arrival, limit1, limit2, &price, promotion);
        printf("Here are the trains going from %s to %s:\n", departure, arrival);
        while (i < arraylength)
        {
            sscanf(trains[i], "%d;%[^;];%[^;];%[^;];%[^;];%f;%s", &trainNumber, departure, arrival, limit1, limit2, &price, promotion);
            if(compareTemps(limit1,limit2,&tempsmin)){
                fastTrain= trains[i];
            }

            if (strcmp(promotion, "REDUC") == 0)
            {
                price *= 0.8;
            }
            else if (strcmp(promotion, "SUPPL") == 0)
            {
                price *= 1.1;
            }
            if (price < minprice)
            {
                minprice = price;
                cheapTrain = trains[i];
            }
            printf("Train N°: %d leaving at %s and arriving at %s\n    price : %.2f\n", trainNumber, limit1, limit2, price);
            i++;
        }
        sscanf(cheapTrain, "%d;%[^;];%[^;];%[^;];%[^;];%f;%s", &trainNumber, departure, arrival, limit1, limit2, &price, promotion);
        printf("----------------------\nThe lower price for this travel is %.2f for \n", minprice);
        printf("Train N°: %d leaving at %s and arriving at %s\n----------------------\n", trainNumber, limit1, limit2);
        sscanf(fastTrain, "%d;%[^;];%[^;];%[^;];%[^;];%f;%s", &trainNumber, departure, arrival, limit1, limit2, &price, promotion);
        printf("----------------------\nThe fastest train for this travel is \n");
        printf("Train N°: %d leaving at %s and arriving at %s for %d minutes of travel\n    price : %.2f\n----------------------\n", trainNumber, limit1, limit2,tempsmin,price);
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

void getAllTrainsFromTheServer(int socketService)
{
    int user_int = 0;
    int arrayLength = 0;
    int strLength = 0;
    char train[1024];
    // sending choosed option
    printf("I send %d\n", user_int);
    write(socketService, &user_int, sizeof(int));
    // reading allTrains array length
    read(socketService, &arrayLength, sizeof(int));
    if (arrayLength != 0)
    {
        printf("HERE IS THE LIST OF FOUND TRAINS  \n");
        for (int i = 0; i < arrayLength; i++)
        {
            // reading size of the train number i
            read(socketService, &strLength, sizeof(int));

            // reading the value of the train number i
            read(socketService, train, strLength * sizeof(char));
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
        printf("No train was found !\n");
    }
    char user_char;
    printf("press q to quit : \n");
    scanf("%c", &user_char);
    while (user_char != 'q')
    {
        scanf("%c", &user_char);
    }
    // printf("***********EXIT = %d\n", exit);
}

void getAllTrainsWithGivenSlotTime(int socketService)
{
    int arrayLength;

    // sending choosed option
    int user_int = 2;
    write(socketService, &user_int, sizeof(int));

    askAndSendDeparture(socketService);
    askAndSendArrival(socketService);
    // read and send the two time limits to the server
    askAndSendTime(socketService, 1);
    askAndSendTime(socketService, 2);
    char **trains = getListeTrain(socketService, &arrayLength);
    affichageTrainBy_Departure_AND_Arrival(trains, arrayLength);
    free(trains);
}

void askAndSendTime(int socketServeur, int N)
{
    int flag = 1;
    char born[50];
    int heures = 0;
    int minutes = 0;
    int len = 0;
    printf("############## TIME DEPARTURE ############\n");
    while (flag)
    {
        printf("Enter limit %d :  (format HH:mm):\n", N);
        // Ask user the value of born
        scanf("%s", born);
        printf("I scan : %s \n", born);

        // mesuring the length of born
        len = strlen(born);
        if (len == 5)
        {
            if (born[0] == '0')
            {
                for (int i = 1; i <= len; i++)
                {
                    born[i - 1] = born[i];
                }
                len--;
            }
            sscanf(born, "%d:%d", &heures, &minutes);

            if (heures < 24 && heures >= 0 && minutes < 60 && minutes >= 0)
            {
                flag = 0;
            }
        }
    }
    printf("len : %d \n", len);
    // sending departure length
    write(socketServeur, &len, sizeof(int));
    // sending departure
    write(socketServeur, born, sizeof(char) * len);
}
void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        // lire et ignorer les caractères du tampon d'entrée
    }
}
void cutConnexion(int socketServeur)
{
    int user_int = -1;
    printf("I send %d\n", user_int);
    write(socketServeur, &user_int, sizeof(int));
}

int compareTemps(char* limite1,char* limite2, int *tempsmin){
    int heuresA=0;int heuresD=0;int minutesD=0;int minutesA=0; int newtemps=0;
    // on récupére sous forme de int les heures et minutes de départ et d'arrivé 
    sscanf(limite1,"%d:%d",&heuresD,&minutesD);
    sscanf(limite2,"%d:%d",&heuresA,&minutesA);
    // on calcule le nouveau temps en minutes
    newtemps=(heuresA-heuresD)*60+(minutesA-minutesD);
    // si le nouveau temps et inférieur au temps donné on retourne 1 et tempmin prend la nouvel valeur
    if (newtemps<(*tempsmin)){
        (*tempsmin)=newtemps;
        return 1;
    }// sinon on retourne 0
    else{
        return 0;
    }
}
void getTrainbyHourAndCityFromServer(int socketServeur){

    int user_option = 1;
    
    write(socketServeur,&user_option,sizeof(int));
    askAndSendDeparture(socketServeur);
    askAndSendArrival(socketServeur);
    askAndSendTime(socketServeur,1);
    
    int strLength;
    // reading size of the train
    read(socketServeur, &strLength, sizeof(int));
    printf("len %d \n", strLength);
    char *train =(char*) malloc(sizeof(char) * strLength);

    // reading the value of the train
    read(socketServeur, train, strLength * sizeof(char));

    printf("T:%s\n",train);
    int trainNumber = 0;
    char departure[50];
    char arrival[50];
    char limit1[50];
    char limit2[50];
    float price = 0.0;
    char promotion[50];

    sscanf(train, "%d;%[^;];%[^;];%[^;];%[^;];%f;%s", &trainNumber, departure, arrival, limit1, limit2, &price, promotion);
    printf("Here are the trains going from %s to %s :\n\t\t\tstart : %s\n\t\t\tstop : %s\n\t\t\tprice : %f\n", departure, arrival, limit1, limit2, price);
    free(train);
}