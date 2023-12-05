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

struct sockaddr_in sockaddr_inConstructor();
int initServeur( struct sockaddr_in *sockAddr,int port);

int mainloop(int socketServeur, struct sockaddr_in *socketClient, char *sncf);

void dialogueClient(int SocketService,char* sncf);
void sendListeTrains(int socketService,char **allTrains);
void sendAllTrains(int socketService,char* sncf);
void sendTrainBy_Departure_AND_Arrival(int socketService,char* sncf);
void clearSocket(int socket);
