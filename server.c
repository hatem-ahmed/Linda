/*-------------------------------------------------------------------*/
/*---- 		server.c -- a stream socket server demo		-----*/
/*-------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <inttypes.h>

#include "server.h"
#include "client.h"
#include "file_ops.h"

#define PORT "0"
#define MAXDATASIZE 1000
#define BACKLOG 10  	 // this is used in Listen() call, how many pending connections queue will hold


void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

void server(char *userhostdef)
{
    extern int sockfd, new_fd;  			// listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    socklen_t sin_size;
    int yes=1;
    extern char local_ipaddr[INET6_ADDRSTRLEN];
    int rv;					//return value of function getaddrinfo()

    memset(&hints, 0, sizeof hints);	

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    rv = getaddrinfo(NULL, PORT, &hints, &servinfo);

    char host[128];
    int len;
    int xx;
    xx = gethostname(host, sizeof host);

    if (rv != 0) 
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    }


    for(p = servinfo; p != NULL; p = p->ai_next) 
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) 
        {
            perror("server: socket");
            continue;
        }
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int)) == -1) 
        {
            perror("setsockopt");
            exit(1);
        }
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) 
        {
            close(sockfd);
            perror("server: bind");
            continue;
        }
        break;
    }


    if (p == NULL)  
    {
        fprintf(stderr, "server: failed to bind\n");
    }


    freeaddrinfo(servinfo); // all done with this structure, freeaddrinfo() function frees the memory that was allocated for the linked list res of getaddrinfo()

    int rrr;
    rrr = getaddrinfo(host, PORT, &hints, &servinfo);
    p = servinfo;
    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), local_ipaddr, sizeof local_ipaddr); 


    struct sockaddr_in sin;
    extern int local_portnumber;
    socklen_t lenn = sizeof(sin);
    xx = getsockname(sockfd, (struct sockaddr *)&sin, &lenn); 
    local_portnumber = ntohs(sin.sin_port);

}

void server_in_background()
{
    extern int sockfd;
    extern int new_fd;
    extern char s[INET6_ADDRSTRLEN];
    struct sigaction sa;
    struct sockaddr_storage their_addr; 	// connector's address information
    char buffer[1024];
    int nummbytes;
    extern char *client_part;
    extern char *userhostdef;
    char filename[50];
    char filetuple[50];	
    extern char input[1000];
    extern char input_copy[1000];
    extern char* part;
    extern char* flag;
    if (listen(sockfd, BACKLOG) == -1) 
    {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) 
    {
        perror("sigaction");
        exit(1);
    }

    //    printf("server: waiting for connections...\n");
    //memset(buffer,0,sizeof(buffer));
    while(1)    // main accept() loop 
    {        
        int sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);

        if (new_fd == -1) 
        {
            perror("accept");
            continue;
        }
        inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),s, sizeof s);
//              printf("server: got connection from %s\n", s);


        //        if (!fork())  // this is the child process
        /*
{            
close(sockfd); // child doesn't need the listener
if (send(new_fd, "Hello, Hatem!", 13, 0) == -1)
perror("send");
close(new_fd);
exit(0);
}
*/
        if ((nummbytes = recv(new_fd, buffer, MAXDATASIZE-1, 0)) == -1) 
        {
            perror("recv");
            exit(1);
        }
        //	printf("server recieved data%s\n", buffer);
        //	client_part = strtok(buffer, ", ()");
        write_client_input(buffer, s, filename, filetuple);
//	printf("%s\n",buffer);
        //*flag = 10;
        /*
while (1)
{
printf("Linda>");
fgets(input, sizeof (input), stdin);
strcpy(input_copy, input);
part = strtok(input, ", ()");
write_parsed_input_in_nets_tuples(part);
}
*/

    }
}




