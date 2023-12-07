#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/wait.h>
#include <signal.h>
#include <netinet/in.h>
#include <stdbool.h>
#define MAX_SIZE 50
static int* Pid_fils[MAX_SIZE];
static int Number_Pid_fils=0;

// Build server socket and attach it to given port and IP adress
int initServeur(struct sockaddr_in *sockAddr, int port);

// The main loop, waits for user connexion and fork the program (creates a chikd process to handle client requests)
int mainloop(int socketServeur, struct sockaddr_in *socketClient, char *sncf);

// Handle client option and call the corresponding handler
void dialogueClient(int SocketService, char *sncf);
// Send the given trains list o the client
void sendListeTrains(int socketService, char **allTrains);
// Send all existing trains to the client
void sendAllTrains(int socketService, char *sncf);
// Send trains corresponding to given departure city and arrival city
void sendTrainBy_Departure_AND_Arrival(int socketService, char *sncf);
// Send trains matching the given slot time, departure and arrival cities.
void sendTrainsOverSlotTime(int socketService, char *sncf);
// Send the train that matches the given departure time or the next one if no train was found with given data.
void sendTrainbyHourAndCity(int socketService, char *sncf);
// Read form the client given departure and arrival cities
void getDepartureAndArrival(int socketService, char *departure, char *arrival);