/*
* ** client.c -- a stream socket client demo
* */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "client.h"
#include "server.h"

#define PORT "0" // the port client will be connecting to 
#define MAXDATASIZE 100 // max number of bytes we can get at once 

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) 
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
//void client(char *input, char *input1)
void client(char *cl_ipaddr, char *cl_port, char *datatosend)
{
    extern int sockfd; 
    int numbytes;  
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char dest[INET6_ADDRSTRLEN];
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(cl_ipaddr, cl_port, &hints, &servinfo)) != 0) 
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    }

    for(p = servinfo; p != NULL; p = p->ai_next) 
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) 
        {
            perror("client: socket");
            continue;
        }
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) 
        {
            close(sockfd);
            perror("client: connect");
            continue;
        }
        break;
    }

    if (p == NULL) 
    {
        fprintf(stderr, "client: failed to connect\n");
    }
    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),dest, sizeof dest);
    if (!fork())  // this is the child process
    {
        //   close(sockfd); // child doesn't need the listener
        if (send(sockfd, datatosend, 1024, 0) == -1)
        perror("send");
        //     close(new_fd);
        exit(0);
    }

//    printf("client: connecting to %s\n", dest);
//   printf("client data%s\n", datatosend);
    //    freeaddrinfo(servinfo); // all done with this structure
    /*   if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
perror("recv");
exit(1);
}*/
    //    buf[numbytes] = '\0';
    //    printf("client: received '%s'\n",buf);
    //    close(sockfd);
}



