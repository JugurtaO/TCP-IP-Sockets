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

int mainloop(int DescripteurSocket, struct sockaddr_in *sockAddr,FILE *sncf);

void dialogueClient(int SocketService,FILE * sncf);
