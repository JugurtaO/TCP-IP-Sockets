 #include <sys/types.h>
 #include <unistd.h>
 #include <sys/socket.h>
 #include <netdb.h>
 #include <stdio.h>
 #include <string.h>
 #include <stdlib.h>

int initClient(struct sockaddr_in* sockaddr,char* hostname,int port);               
void afficherMenu();
void dialogueServeur(int socketServeur,struct sockaddr_in* adserv);




char **getListeTrain(int socketServeur,int* arrayLength);
void askAndSendDeparture(int socketServeur);
void askAndSendArrival(int socketServeur);
void affichageTrainBy_Departure_AND_Arrival(char** trains ,int arraylength);
void getServeurTrainBy_Departure_AND_Arrival(int socketServeur);