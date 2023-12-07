#include "../headers/sncf.h"

char **getAllTrains(char *sncf)
{
    char **allTrains = (char **)malloc(sizeof(char *) * 50);
    FILE *SNCF = fopen(sncf, "r");
    if (SNCF == NULL)
    {
        perror("sncf : ");
        fprintf(stderr, "Please provide the full path to %s\n", sncf);
        exit(0);
    }
    else
    {

        int i = 0;
        while (!feof(SNCF))
        {
            allTrains[i] = (char *)malloc(512 * sizeof(char));
            fscanf(SNCF, "%[^\n]\n", allTrains[i]);
            i++;
        }
        allTrains[i] = NULL;
    }

    fclose(SNCF);

    return allTrains;
}
char **getTrainBy_Departure_AND_Arrival(char *departure, char *arrival, char *sncf)
{
    // on déclare un tableau des trains que l'on recherche dans le tas
    char **searchTrains = (char **)malloc(sizeof(char *) * 50);
    // chaîne de caractères qui va accueillir la ligne lu du document
    char train[256];
    // variable accueillant les différents champs d'un train
    int trainNumber = 0;
    char departureT[50];
    char arrivalT[50];
    char limit1[50];
    char limit2[50];
    float price = 0.0;
    char promotion[50];

    // ouverture du fichier en mode lecture

    FILE *SNCF = fopen(sncf, "r");
    // Gestion de l'erreur d'ouverture
    if (SNCF == NULL)
    {
        perror("sncf : ");

        fprintf(stderr, "Please provide the full path to %s\n", sncf);
        exit(0);
    }
    else
    {

        int i = 0;
        // tant que le fichier n'est pas fini on lit une ligne et on l'affecte à train
        while (!feof(SNCF))
        {
            fscanf(SNCF, "%[^\n]\n", train);
            // on récupére les caractéristiques du train
            sscanf(train, "%d;%[^;];%[^;];%[^;];%[^;];%f;%s", &trainNumber, departureT, arrivalT, limit1, limit2, &price, promotion);

            // printf("N°%d :D %s :A %s :Hd %s :Ha %s :P %f :O%s \n", trainNumber, departureT, arrivalT, limit1, limit2, price, promotion);
            if (cmpInlowerCase(departure, departureT) == 0 && cmpInlowerCase(arrival, arrivalT) == 0)
            {
                searchTrains[i] = (char *)malloc(512 * sizeof(char));
                strcpy(searchTrains[i], train);
                i++;
            }
        }

        searchTrains[i] = NULL;
    }

    fclose(SNCF);

    return searchTrains;
}

char *strtolower(char *dest, const char *src)
{
    char *result = dest;
    while ((*dest++ = tolower(*src++)))
        ;

    return result;
}
int cmpInlowerCase(char *str1, char *str2)
{

    int result;
    char *cpy1 = (char *)malloc(sizeof(char) * 100);
    char *cpy2 = (char *)malloc(sizeof(char) * 100);
    strtolower(cpy1, str1);
    strtolower(cpy2, str2);
    result = strcmp(cpy1, cpy2);
    free(cpy1);
    free(cpy2);
    return result;
}

// By given departure and arrival cities, departure slot time, we fetch the corresponding Trains from our sncf.txt file (db)
char **getTrainsOverSlotTime(char *departure, char *arrival, char *limit1, char *limit2, char *sncf)
{
    char **requestedTrains = (char **)malloc(sizeof(char *) * 50);
    char **allTrainsByGiven_Departure_AND_Arrival = getTrainBy_Departure_AND_Arrival(departure, arrival, sncf);
    int length = 0;
    while (allTrainsByGiven_Departure_AND_Arrival[length] != NULL)
    {
        length++;
    }
   

    // variable accueillant les différents champs d'un train
    int trainNumber = 0;
    char departureT[50];
    char arrivalT[50];
    char born1[50];
    char born2[50];
    float price = 0.0;
    char promotion[50];

    int indice = 0;
    for (int i = 0; i < length; i++)
    {
        sscanf(allTrainsByGiven_Departure_AND_Arrival[i], "%d;%[^;];%[^;];%[^;];%[^;];%f;%s", &trainNumber, departureT, arrivalT, born1, born2, &price, promotion);
        // We check wether the departure time born1 is between limite1 and limit2
        
       if  ((cmp2Time(born1, limit1) >= 0 && cmp2Time(born1, limit2) <= 0) )
        {
            requestedTrains[indice] = malloc(sizeof(char) * 512);
            strcpy(requestedTrains[indice], allTrainsByGiven_Departure_AND_Arrival[i]);
            indice++;
        }
    }
    requestedTrains[indice] = NULL;
    departure=NULL;
    arrival=NULL;
    
   

    return requestedTrains;
}

char *getTrainByGivenDepartureCity(char *sncf, char *departure, char *arrival, char *hour)
{

    char **trainDepartureCity = (char **)malloc(sizeof(char *) * 50);

    /*creation of the table*/
    char *first_train = (char *)malloc(sizeof(char) * 50);
    first_train[0]='\0';
    char *currentLine_exploitable = (char *)malloc(sizeof(char) * 50);

    trainDepartureCity = getTrainBy_Departure_AND_Arrival(departure, arrival, sncf);
    /*research of table length*/
    int len = 0;
    while (trainDepartureCity[len] != NULL)
    {
        len++;
    }

    /*Iterating through the array to select informations*/
    for (int elm = 0; elm < len; elm++)
    {
        if (elm == 0)
        {
            strcpy(first_train, trainDepartureCity[elm]);
        }
        strcpy(currentLine_exploitable, trainDepartureCity[elm]);
        strtok(trainDepartureCity[elm], ";");
        strtok(NULL, ";");
        strtok(NULL, ";");
        char *hourTraject = strtok(NULL, ";");

        if (cmp2Time(hourTraject, hour) >= 0)
        {
            return currentLine_exploitable;
        }
    }
    
    return first_train;
}

int cmp2Time(char * time1, char*time2){
    int heures1=0;
    int heures2=0;
    int minutes1=0;
    int minutes2=0;
    int tempsInmin1=0;
    int tempsInmin2=0;
    sscanf(time1,"%d:%d",&heures1,&minutes1);
    sscanf(time2,"%d:%d",&heures2,&minutes2);    
    tempsInmin1=heures1*60+minutes1;
    tempsInmin2=heures2*60+minutes2;

    return tempsInmin1-tempsInmin2;
}