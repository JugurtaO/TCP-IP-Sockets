//Author: OURZIK J.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

//User requests handlers
char *getTrainBy_Departure_Arrival_AND_DepartureTime(char *departure, char *arrival, char *time,char*sncf);
char **getTrainBy_Departure_Arrival_AND_TimeSlot(char *departure, char *arrival, char *limite1, char *limit2,char*sncf);
char **getTrainBy_Departure_AND_Arrival(char *departure, char *arrival,char*sncf);
char **getAllTrains(char*sncf);

//utils function
int cmpInlowerCase(char* str1,char*str2);
char * strtolower( char * dest, const char * src );