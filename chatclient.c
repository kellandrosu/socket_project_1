

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT "33330"
#define SERVER "flip1.engr.oregonstate.edu"
#define BUF_LEN 501


int main(int argc, char* argv[]) {

    char* port;
    char* hostname;

    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;

    memset( &hints, 0, sizeof hints );

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ( argc == 3 ) {
        hostname = argv[1];
        port = argv[2];    
        status = getaddrinfo( hostname, port, &hints, &servinfo );    //get hostname info
    }
    else {       
        status = getaddrinfo( SERVER, PORT, &hints, &servinfo );    //get hostname info
    }

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

    char username[12];

    //sign in
    do {
        printf("Enter your username: ");
        
        scanf("%11s", username);
        while( (getchar()) != '\n'); //clear input buffer
        
        if (strlen (username) > 10 ) {
            printf("Sorry, username must be 10 characters or less.\n\n");
        }
    } while (strlen (username) > 10 );


    char* userinput = NULL;
    char clientMessage[BUF_LEN];
    size_t maxbuffer = BUF_LEN;
    char* serverMessage = malloc(BUF_LEN + 12);
    int sizeRecv;

    //chat
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
        sizeRecv = recv( sockfd, serverMessage, BUF_LEN + 12, 0);
        serverMessage[sizeRecv] = '\0';


        printf("%s\n", serverMessage);

    }
    
    free (serverMessage);
    free(userinput);

    close(sockfd);

    return 0;
}
