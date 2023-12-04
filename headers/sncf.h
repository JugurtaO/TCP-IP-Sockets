//Author: OURZIK J.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//User requests handlers
char *getTrainBy_Departure_Arrival_AND_DepartureTime(char *departure, char *arrival, char *time,FILE*sncf);
char **getTrainBy_Departure_Arrival_AND_TimeSlot(char *departure, char *arrival, char *limite1, char *limit2,FILE*sncf);
char *getTrainBy_Departure_AND_Arrival(char *departure, char *arrival,FILE*sncf);
char **getAllTrains(FILE*sncf);