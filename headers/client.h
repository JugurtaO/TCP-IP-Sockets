 #include <sys/types.h>
 #include <unistd.h>
 #include <sys/socket.h>
 #include <netdb.h>
 #include <stdio.h>
 #include <string.h>
 #include <stdlib.h>

int initClient(struct hostent * host,struct sockaddr_in* sockaddr);               
void afficherMenu();
void dialogueServeur(int socketServeur,struct sockaddr_in* adserv);