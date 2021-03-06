/*
    Author:      Andrius Kelly
    Date:        Feb 10, 2018
    Description: CS 372 Project 1 - Client implimentation for basic chat program. 
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>


#define BUF_LEN 1024 


//prototypes
int connectToHost(char* hostname, char* port) ;
void chatWithHost(int sockfd, char* username) ;


int main(int argc, char* argv[]) {

    char* port;
    char* hostname;
    
    
    //check arguments
    if ( argc == 3 ) {
        hostname = argv[1];
        port = argv[2];    
    }
    else {       
        fprintf(stderr, "Usage: chatclient [hostname] [port number]\n");
        exit(1);
    }

   
   //create connection
    int sockfd = connectToHost(hostname, port);

   
   //get username
    char username[12];
    do {
        printf("Enter your username: ");
        
        scanf("%11s", username);
        while( (getchar()) != '\n'); //clear input buffer
        
        if (strlen (username) > 10 ) {
            printf("Sorry, username must be 10 characters or less.\n");
        }
    } while (strlen (username) > 10 );
    
   
   //chat
    if (sockfd >= 0 ) {
        chatWithHost(sockfd, username);
    }

    close(sockfd);

    return 0;
}



/*--------------------------------  FUNCTIONS  -----------------------------------------*/



//Description: handles user input and chat sending and receiving
void chatWithHost(int sockfd, char* username) {

    char* userinput = NULL;
    char clientMessage[BUF_LEN];
    size_t maxbuffer = BUF_LEN;
    char* serverMessage = malloc(BUF_LEN);
    int sizeRecv;

    while(1) {
       
        //get user input
        printf("%s> ", username);
       
        getline( &userinput, &maxbuffer, stdin);
        //scrub trailing \n from input
        userinput[ strlen(userinput) - 1 ] = '\0';

        if ( strcmp( userinput, "\\quit" ) == 0 ) {
            printf("leaving chat\n");
            break;
        }
        else {
            //preppend input with username
            strcpy(clientMessage, username);
            strcat(clientMessage, "> ");
            strcat(clientMessage, userinput);
        }

        //send message
        if ( send( sockfd, clientMessage, strlen(clientMessage), 0) < 0 ) {
            close(sockfd);
            fprintf(stderr, "send error\n");
            exit(1);
        }
        
        //receive message
        sizeRecv = recv( sockfd, serverMessage, BUF_LEN + 12, 0);
        
        if( sizeRecv == 0 ) {
            printf("connection closed\n");
            break;
        }

        //null terminate received message
        serverMessage[sizeRecv] = '\0';
        printf("%s\n", serverMessage);
    }
    
    free(serverMessage);
    free(userinput);
}    



// Description: creates a connects with hostname and port.
// Exits on connection errors 
// Returns: socket file descriptor (int)
int connectToHost(char* hostname, char* port) {

    //open serverinfo
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;

    memset( &hints, 0, sizeof hints );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo( hostname, port, &hints, &servinfo );    //get hostname info
        
    if( status != 0 ) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }
    

    //create socket for connection
    int sockfd = socket( servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol );

    if ( sockfd < 0 )  {
        fprintf(stderr, "socket error\n");
        exit(1); 
    }

    if ( connect( sockfd, servinfo->ai_addr, servinfo->ai_addrlen ) < 0 ){
        close(sockfd);
        fprintf(stderr, "connect error\n");
        exit(1); 
    }

    free(servinfo);
    return sockfd;
}
