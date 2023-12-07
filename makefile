#author: Jugurta OURZIK
#****************************
    
#compiler name
CC=gcc

#compiling flags
CFLAGS= -g -Wall -c 

#link edition flags
LDFLAGS= -lm 


#list of executable files
exec: mainClient mainServeur


#specifiyin the list of dependencies
client.o:client.c ../headers/client.h 
serveur.o:serveur.c ../headers/serveur.h ../headers/sncf.h
sncf.o:sncf.c ../headers/sncf.h 
mainClient.o:mainClient.c ../headers/client.h
mainServeur.o:mainServeur.c ../headers/serveur.h ../headers/sncf.h

#build object files from c source target
%.o:%.c
	$(CC) $(CFLAGS) $<



#specifiyin the list of object files dependencies	
mainClient:client.o mainClient.o 
mainServeur:serveur.o sncf.o mainServeur.o 

#build executable from object files
%:%.o 
	 $(CC) -o $@ $(LDFLAGS) $^

	
#clean command
clean:
	rm -rf *.o mainClient mainServeur