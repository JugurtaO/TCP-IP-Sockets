#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/*   ---------- User requests handlers --------*/

//Option 0 : Scans the document (sncf.txt file) and returns all trains 
char **getAllTrains(char*sncf);
//Option 1 : Scans the docuemnt (sncf.txt file) and returns the corresponding train to given data or the next one if no train was found. (including next day)
char *getTrainByGivenDepartureCity(char *sncf, char *departure, char *arrival, char *hour);
//Option 2 : Scans the document (sncf.txt file) and returns all trains that matches given slot time, departure and arrival cities
char ** getTrainsOverSlotTime(char *departure, char *arrival, char *limit1, char *limit2,char *sncf);
//Option 3 : Scans the document (sncf.txt file) and returns a list of trains matching given departure and arrival cities
char **getTrainBy_Departure_AND_Arrival(char *departure, char *arrival,char*sncf);

/*   ---------- Util functions -----------*/
//Compare two strings in lowercase format (transforms given parameters to lowercase format and does the comparaison).Returns strcmp(str1,str2)
int cmpInlowerCase(char* str1,char*str2);
//Transforms source string into lowercase format and stores it in given destination variable
char * strtolower( char * dest, const char * src );

// Transform the two time in a number of minutes and return the differences
int cmp2Time(char * time1, char*time2);