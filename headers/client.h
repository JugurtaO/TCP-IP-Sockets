 #include <sys/types.h>
 #include <unistd.h>
 #include <sys/socket.h>
 #include <netdb.h>
 #include <stdio.h>
 #include <string.h>
 #include <stdlib.h>
 #include <float.h>
 #include <ctype.h>

//Build client socket and establish connexion to the server 
int initClient(struct sockaddr_in* sockaddr,char* hostname,int port);  

//Display Main MENU for user
void afficherMenu();

//Start the dialogue between the client and the server 
void dialogueServeur(int socketServeur,struct sockaddr_in* adserv);



//Read a list of trains from the server
char **getListeTrain(int socketServeur,int* arrayLength);
//Ask the user for departure city and send it to the server
void askAndSendDeparture(int socketServeur);
//Ask the user for arrival city and send it to the server
void askAndSendArrival(int socketServeur);
//Ask the user for departure or arrival time  and send it to the server
void askAndSendTime(int socketServeur,int N);

//Display trains given by a list and does a selection by lower price and faster travel
void affichageTrainBy_Departure_AND_Arrival(char** trains ,int arraylength);
//Fetch trains according to given departure and arrival from the server and displays them using "affichageTrainBy_Departure_AND_Arrival"
void getServeurTrainBy_Departure_AND_Arrival(int socketServeur);
//Fetch all trains from the server 
void getAllTrainsFromTheServer(int socketService);
//Fetch trains from the server according to given departure city , arrival city, slot time  and display them.
void getAllTrainsWithGivenSlotTime(int socketService);
//Fetch train from the server corresponding to given data.If no train was found, the next one or the first train of the day is rendered and displayed.
void getTrainbyHourAndCityFromServer(int socketServeur);
//Clean the standard input 
void clearInputBuffer();
//stop properly the connexion with the server
void cutConnexion( int socketServeur);
//Compare departure time and arrival time to given time in minutes.Returns 0 if the calculated time from limit1 and limit2 is greater or equals to tempsmin
// return 1 if calculated time from limit1 and limit2 is lower than tempsmin, and set the value of temps min to the new time
int compareTemps(char* limite1,char* limite2, int *tempsmin);
// retourne 0 si cela ne respecte pas le format hh:mm ou h:mm et 1 sinon
int validTimeFormat(char* str);
//Check wether the read function result is an erro and stops properly the programm.
void checkErrorRead(int resultRead);
// void checkErrorWrite(int resultWrite);