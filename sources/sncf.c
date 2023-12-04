#include "../headers/sncf.h"

char **getAllTrains(FILE *sncf)
{
    char **allTrains =(char**) malloc(sizeof(char*) * 50);
    // int trainNumber = 0;
    // char *departure = NULL;
    // char *arrival = NULL;
    // char *limit1 = NULL;
    // char *limit2 = NULL;
    // float price = 0.0;
    // char *promotion = NULL;

    
    int i=0;
    while (!feof(sncf))
    {
        allTrains[i]=(char*)malloc(512 * sizeof(char));
        // fscanf(sncf,"%d;%s;%s;%s;%s;%f;%s",&trainNumber,departure,arrival,limit1,limit2,&price,promotion);
        fscanf(sncf, "%[^\n]\n",allTrains[i]);
       
        
        i++;
    }

    return allTrains;
}