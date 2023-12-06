#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>


int main(int argc, char** argv){
    char str1[50];
    char str2[50];
    scanf("%[^\n]s",str1);
    int len =strlen(str1);
    printf("result= %s len=%d\n", str1,len);
    fgets(str2,50,stdin);
    printf("result= 4%s4 \n", str2);
    
}