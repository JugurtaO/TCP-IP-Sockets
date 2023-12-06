 #include <sys/types.h>
 #include <unistd.h>
 #include <sys/socket.h>
 #include <netdb.h>
 #include <stdio.h>
 #include <string.h>
 #include <stdlib.h>
 #include <float.h>

int initClient(struct sockaddr_in* sockaddr,char* hostname,int port);               
void afficherMenu();
void dialogueServeur(int socketServeur,struct sockaddr_in* adserv);




char **getListeTrain(int socketServeur,int* arrayLength);
void askAndSendDeparture(int socketServeur);
void askAndSendArrival(int socketServeur);
void affichageTrainBy_Departure_AND_Arrival(char** trains ,int arraylength);
void getServeurTrainBy_Departure_AND_Arrival(int socketServeur);
void getAllTrainsFromTheServer(int socketService);
void getAllTrainsWithGivenSlotTime(int socketService);
void askAndSendTime(int socketServeur,int N);
void getTrainbyHourAndCityFromServer(int socketServeur);
void clearInputBuffer();
void cutConnexion( int socketServeur);
int compareTemps(char* limite1,char* limite2, int *tempsmin);