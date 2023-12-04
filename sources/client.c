#include "../headers/client.h"

int initClient(struct hostent *host, struct sockaddr_in *adserv)
{
    int socketServeur = socket(AF_INET, SOCK_STREAM, 0);

    if (socketServeur == -1)
    {
        perror("socketClient : ");
        exit(0);
    }

    host = gethostbyname("localhost");
    memcpy(&(adserv->sin_addr.s_addr), host->h_addr_list[0], sizeof(unsigned long));

    adserv->sin_port = htons(8080);
    adserv->sin_family = AF_INET;
    return socketServeur;
}

void dialogueServeur(int socketServeur, struct sockaddr_in *adserv)
{

    int exit = 0;
    int user_int;
    while (exit == 0)
    {
        afficherMenu();
        printf("option >>: ");
        scanf("%d", &user_int);

        switch (user_int)
        {
        case 0:
            int arrayLength = 0;
            int strLength = 0;
            char train[1024];
            // sending choosed option
            write(socketServeur, &user_int, sizeof(int));
            // reading allTrains array length
            read(socketServeur, &arrayLength, sizeof(int));
            if (arrayLength != 0)
            {
                printf("HERE IS THE LIST OF FOUND TRAINS  \n");
                for (int i = 0; i < arrayLength*2 ; i++)
                {
                    // reading size of the train number i
                    read(socketServeur, &strLength, sizeof(int));

                    // reading the value of the train number i
                    read(socketServeur, train, strLength * sizeof(char));
                    printf("%s\n",train);

                    //clean the buffer train
                    for (int i = 0; i < 512; i++)
                    {
                        train[i]='\0';
                    }
                    
                }
            }else{
                printf("No train was found !");
            }

            break;
        case -1:
            exit = 1;
        default:
            break;
        }
    }
}
void afficherMenu()
{
    printf("\n");
    printf("*******Main menu*******\n");
    printf("choose an option : \n");
    printf("0 : See all trains\n");
    printf("1 : Get Train By given departure city,arrival city and departure time \n");
    printf("2 : Get Train By given departure city,arrival city and departure slot time\n");
    printf("3 : Get Train By given departure and arrival city with best  prices or optimal journey time \n");
    printf("-1: Quit\n");
    printf("***********************\n");
}
