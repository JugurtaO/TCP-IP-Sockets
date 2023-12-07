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
    char user_char;
    while (exit == 1)
    {
        // reset the option buffer in a invalid value to prevent error when the user input a char instead of int

        afficherMenu();
        printf("option >>: ");
        // read chossen option

        scanf("%c", &user_char);
        // look if the option is valid

        switch (user_char)
        {
        case '0':
            // Fetch all trains from the server and print them
            getAllTrainsFromTheServer(socketServeur);
            break;
        case '1':
            // Fetch a train by given time and departure/arrival city or the closer one from the server and print it
            getTrainbyHourAndCityFromServer(socketServeur);
            break;
        case '2':
            // Fetch all train by given time slot and departure/arrival city from the server and print them, show to the user the lowest price travel and the fastest travel
            getAllTrainsWithGivenSlotTime(socketServeur);
            break;
        case '3':
            // Fetch
            getServeurTrainBy_Departure_AND_Arrival(socketServeur);
            break;
        case 'q':
            cutConnexion(socketServeur);
            exit = 0;
        default:
            break;
        }
        clearInputBuffer();
    }
}
void afficherMenu()
{
    printf("\n");
    printf("*************************************************************************************************\n");
    printf("***********************************   MAIN MENU   ***********************************************\n");
    printf("*************************************************************************************************\n");
    printf("\n");
    printf("Choose an option : \n");
    printf("    0 : See all trains\n");
    printf("    1 : Get Train By given departure city,arrival city and departure time \n");
    printf("    2 : Get Train By given departure city,arrival city and departure slot time\n");
    printf("    3 : Get Train By given departure and arrival city with best prices or optimal journey time \n");
    printf("    q : Quit\n");
    printf("\n");
    printf("*************************************************************************************************\n");
    printf("*************************************************************************************************\n");
    printf("*************************************************************************************************\n");
}
void getServeurTrainBy_Departure_AND_Arrival(int socketServeur)
{
    char **trains;
    // sending choosed option
    int user_int = 3;
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
    
    
    checkErrorRead(read(socketServeur, arrayLength, sizeof(int)));
        
    if (arrayLength != 0)
    {
        char **trainlist = (char **)malloc(sizeof(char *) * (*arrayLength));

        for (int i = 0; i < (*arrayLength); i++)
        {
            // reading size of the train number i
            
            checkErrorRead(read(socketServeur, &strLength, sizeof(int)));
             

            trainlist[i] = (char *)malloc(sizeof(char) * strLength + 1);
            // reading the value of the train number i
           
           checkErrorRead(read(socketServeur, trainlist[i], strLength * sizeof(char)));
           
            trainlist[i][strLength] = '\0';
           
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
    int departurelength = 50;
    char departure[departurelength];
    int len = 0;
    while (len == 0)
    {
        // Reset the buffer of departure
        for (int i = 0; i < departurelength; i++)
        {
            departure[i] = '\0';
        }
        printf("Enter Departure Town Or Train Station :\n");
        // Ask user the value of departure
        clearInputBuffer();
        scanf("%[^\n]s", departure);

        // mesuring the length of departure
        len = strlen(departure);
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
    int arrivalLength = 50;
    char arrival[arrivalLength];
    int len = 0;
    while (len == 0)
    {
        for (int i = 0; i < arrivalLength; i++)
        {
            arrival[i] = '\0';
        }
        printf("Enter Arrival Town Or Train Station :\n");
        // Ask user the value of departure

        clearInputBuffer();
        scanf("%[^\n]s", arrival);
        // mesuring the length of departure
        len = strlen(arrival);
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
    char *cheapTrain;
    float minprice = FLT_MAX;
    char *fastTrain;
    int tempsmin = __INT_MAX__;

    if (arraylength <= 0)
    {
        printf("Oups No train was found ..!\n");
    }
    else
    {
        int i = 0;
        sscanf(trains[0], "%d;%[^;];%[^;];%[^;];%[^;];%f;%s", &trainNumber, departure, arrival, limit1, limit2, &price, promotion);
        printf(">>> HERE ARE THE TRAINS GOING FROM %s To %s <<< (found %d )\n", departure, arrival, arraylength);
        while (i < arraylength)
        {
            promotion[0] = '\0';
            sscanf(trains[i], "%d;%[^;];%[^;];%[^;];%[^;];%f;%s", &trainNumber, departure, arrival, limit1, limit2, &price, promotion);
            if (compareTemps(limit1, limit2, &tempsmin))
            {
                fastTrain = trains[i];
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
            printf("\n\t\t Train N° %d  %s -> %s    %s    %s    -  %0.2f $  \n", trainNumber, departure, arrival, limit1, limit2, price);

            // printf("Train N°: %d leaving at %s and arriving at %s\n    price : %.2f\n", trainNumber, limit1, limit2, price);
            i++;
        }
        sscanf(cheapTrain, "%d;%[^;];%[^;];%[^;];%[^;];%f;%s", &trainNumber, departure, arrival, limit1, limit2, &price, promotion);
        printf("----------------------\nThe lower price for this travel is %.2f for \n", minprice);
        printf("Train N°: %d leaving at %s and arriving at %s\n----------------------\n", trainNumber, limit1, limit2);
        sscanf(fastTrain, "%d;%[^;];%[^;];%[^;];%[^;];%f;%s", &trainNumber, departure, arrival, limit1, limit2, &price, promotion);
        printf("----------------------\nThe fastest train for this travel is \n");
        printf("Train N°: %d leaving at %s and arriving at %s for %d minutes of travel\n    price : %.2f $ \n----------------------\n", trainNumber, limit1, limit2, tempsmin, price);
    }

    char user_char;
    int j = 0;
    while (j < arraylength)
    {
        free(trains[j]);
        j++;
    }

    printf("\n");
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
    int trainNumber = 0;
    char departure[50];
    char arrival[50];
    char limit1[50];
    char limit2[50];
    float price = 0.0;
    char promotion[50];
    // sending choosed option
    write(socketService, &user_int, sizeof(int));
    // reading allTrains array length
    

    checkErrorRead(read(socketService, &arrayLength, sizeof(int)));
            
    
    if (arrayLength != 0)
    {
        printf(">>> HERE IS THE LIST OF FOUND TRAINS  <<< \n");

        for (int i = 0; i < arrayLength; i++)
        {
            // reading size of the train number i
            

    checkErrorRead(read(socketService, &strLength, sizeof(int)));
       

            // reading the value of the train number i
            
            checkErrorRead(read(socketService, train, strLength * sizeof(char)));
            
            train[strLength] = '\0';
            sscanf(train, "%d;%[^;];%[^;];%[^;];%[^;];%f;%s", &trainNumber, departure, arrival, limit1, limit2, &price, promotion);

            // apply the reduction or the supplement to the price
            if (strcmp(promotion, "REDUC") == 0)
            {
                price *= 0.8;
            }
            else if (strcmp(promotion, "SUPPL") == 0)
            {
                price *= 1.1;
            }
            printf("\n\t\t Train N° %d  %s -> %s    %s    %s    -  %0.2f $  \n", trainNumber, departure, arrival, limit1, limit2, price);
            // reset the promotion value since it can be absent
            promotion[0] = '\0';
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
    printf("\n");
    printf("press q to quit : \n");
    scanf("%c", &user_char);
    while (user_char != 'q')
    {
        scanf("%c", &user_char);
    }
    printf("\n");
}

void getAllTrainsWithGivenSlotTime(int socketService)
{
    int arrayLength;

    // sending choosed option
    int user_int = 2;
    write(socketService, &user_int, sizeof(int));

    askAndSendDeparture(socketService);
    askAndSendArrival(socketService);
    // ask to the user and send the two time limits to the server
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
    printf("############## TIME ############\n");
    printf("Enter Time limit %d :  [ Requested format HH:mm ] : ", N);
    while (flag)
    {

        // Ask user the value of born
        scanf("%s", born);

        // mesuring the length of born
        len = strlen(born);
        if (validTimeFormat(born) == 1)
        {

            sscanf(born, "%d:%d", &heures, &minutes);

            if (heures < 24 && heures >= 0 && minutes < 60 && minutes >= 0)
            {
                if (born[0] == '0')
                {
                    for (int i = 1; i <= len; i++)
                    {
                        born[i - 1] = born[i];
                    }
                    len--;
                }
                flag = 0;
            }
            else
            {
                printf("\nInvalid value for hours/minutes: ");
            }
        }
        else
        {
            printf("\nInvalid format please, need HH:mm : ");
        }
    }

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
    write(socketServeur, &user_int, sizeof(int));
}

int compareTemps(char *limite1, char *limite2, int *tempsmin)
{
    int heuresA = 0;
    int heuresD = 0;
    int minutesD = 0;
    int minutesA = 0;
    int newtemps = 0;
    // on récupére sous forme de int les heures et minutes de départ et d'arrivé
    sscanf(limite1, "%d:%d", &heuresD, &minutesD);
    sscanf(limite2, "%d:%d", &heuresA, &minutesA);
    // on calcule le nouveau temps en minutes
    newtemps = (heuresA - heuresD) * 60 + (minutesA - minutesD);
    // si le nouveau temps et inférieur au temps donné on retourne 1 et tempmin prend la nouvel valeur
    if (newtemps < (*tempsmin))
    {
        (*tempsmin) = newtemps;
        return 1;
    } // sinon on retourne 0
    else
    {
        return 0;
    }
}
void getTrainbyHourAndCityFromServer(int socketServeur)
{

    int user_option = 1;

    write(socketServeur, &user_option, sizeof(int));
    askAndSendDeparture(socketServeur);
    askAndSendArrival(socketServeur);
    askAndSendTime(socketServeur, 1);

    int strLength;
    // reading size of the train
    
     checkErrorRead(read(socketServeur, &strLength, sizeof(int)));
    if (strLength != 0)
    {
        char *train = (char *)malloc(sizeof(char) * strLength);

        // reading the value of the train
       
        checkErrorRead(read(socketServeur, train, strLength * sizeof(char)));
        

        train[strLength] = '\0';
        int trainNumber = 0;
        char departure[50];
        char arrival[50];
        char limit1[50];
        char limit2[50];
        float price = 0.0;
        char promotion[50] = "";
        departure[0] = '\0';
        arrival[0] = '\0';
        sscanf(train, "%d;%[^;];%[^;];%[^;];%[^;];%f;%s", &trainNumber, departure, arrival, limit1, limit2, &price, promotion);
        if (strcmp(promotion, "REDUC") == 0)
        {
            price *= 0.8;
        }
        else if (strcmp(promotion, "SUPPL") == 0)
        {
            price *= 1.1;
        }
        printf(">>> THE NEXT TRAIN GOING FROM  %s  to  %s :\n", departure, arrival);
        printf("\n\tTrain N° %d  %s -> %s    %s    %s    -  %0.2f $\n", trainNumber, departure, arrival, limit1, limit2, price);
        free(train);
    }
    else
    {
        printf("Oups No train was found ..!\n");
    }
    char user_char;
    printf("\n");
    printf("press q to quit : \n");
    scanf("%c", &user_char);
    while (user_char != 'q')
    {
        scanf("%c", &user_char);
    }
    printf("\n");
}

int validTimeFormat(char *str)
{
    int numberDigHours = 0;
    int numberDigitminutes = 0;
    int separator = 0;
    int result = 1;
    int indice = 0;
    int len = strlen(str);
    char current;
    if (len < 4 || len > 5)
    {
        result = 0;
    }
    while (result == 1 && indice < len)
    {
        current = str[indice];
        if (isdigit(current) != 0)
        {
            if (separator == 0)
            {
                numberDigHours++;
            }
            else
            {
                numberDigitminutes++;
            }
        }
        else if (current == ':')
        {
            separator++;
        }
        else
        {
            result = 0;
        }

        indice++;
    }
    if (result != 0)
    {
        if (numberDigHours > 2 || numberDigHours <= 0 || separator != 1 || numberDigitminutes != 2)
        {
            result = 0;
        }
    }
    return result;
}
void checkErrorRead(int resultRead){
    if(resultRead==0){
                printf("Error connexion has been lost\n");
                exit(0);
            }
}

// void checkErrorWrite(int resultWrite){
//     if(resultWrite==-1){
//         printf("Error connexion has been lost\n");
//                 exit(0);
//     }
// }