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
            printf("J'ai scanné  %s \n", allTrains[i]);
            i++;
        }
        allTrains[i] = NULL;
    }

    fclose(SNCF);

    return allTrains;
}
char **getTrainBy_Departure_AND_Arrival(char *departure, char *arrival, char *sncf)
{
    printf("I'm in getTrainBy_Departure_AND_Arrival !\n");
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
    // ouverture du fichier

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
            // fscanf(sncf,"%d;%s;%s;%s;%s;%f;%s",&trainNumber,departure,arrival,limit1,limit2,&price,promotion);
            fscanf(SNCF, "%[^\n]\n", train);
            // on récupére les caractéristiques du train
            sscanf(train, "%d;%[^;];%[^;];%[^;];%[^;];%f;%s", &trainNumber, departureT, arrivalT, limit1, limit2, &price, promotion);

            // printf("N°%d :D %s :A %s :Hd %s :Ha %s :P %f :O%s \n", trainNumber, departureT, arrivalT, limit1, limit2, price, promotion);
            if (cmpInlowerCase(departure, departureT) == 0 && cmpInlowerCase(arrival, arrivalT) == 0)
            {
                searchTrains[i] = (char *)malloc(512 * sizeof(char));
                strcpy(searchTrains[i], train);
                printf("J'ai scan %s \n", searchTrains[i]);
                i++;
            }
        }
        printf("******************************\n");
        searchTrains[i] = NULL;
    }

    fclose(SNCF);

    return searchTrains;
}

char *strtolower(char *dest, const char *src)
{
    char *result = dest;
    while (*dest++ = tolower(*src++));
    return result;
}
int cmpInlowerCase(char *str1, char *str2)
{
    printf("str1: %s str2: %s \n", str1, str2);
    int result;
    char *cpy1 = (char *)malloc(sizeof(char) * 100);
    char *cpy2 = (char *)malloc(sizeof(char) * 100);
    strtolower(cpy1, str1);
    strtolower(cpy2, str2);
    // printf("I compare %s et %s copy of %s et %s\n", cpy1, cpy2, str1, str2);
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
        printf("*****************\nT:%s\n",allTrainsByGiven_Departure_AND_Arrival[length]);
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
     
        if ((strcmp(born1, limit1) >= 0 && strcmp(born1, limit2) <=0 ) /*|| (strcmp(born1, limit1)==0 || strcmp(born1, limit2)==0 )*/)
        {   
            requestedTrains[indice] =malloc(sizeof(char) * 512);
            strcpy(requestedTrains[indice], allTrainsByGiven_Departure_AND_Arrival[i]);
            printf("I copy :%s\n",requestedTrains[indice]);
            indice++;
        }
    }
    requestedTrains[indice] = NULL;
    printf("i final : %d",indice);
    int j=0;
    while (requestedTrains[j] != NULL)
    {
        printf("00000000000000000000\nT:%s\n",requestedTrains[j]);
        j++;
    }
    return requestedTrains;
}

// test de la fonction cmpINlowerCase
// int main(int argc, char **argv){
//     printf("result =%d\n",cmpInlowerCase(argv[1],argv[2]));

// }
