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
#include <sys/mman.h>

#include "server.h"
#include "client.h"
#include "file_ops.h"

FILE *fp, *fp1;
char filename[50];
char filetuple[50];
int local_portnumber;
char local_ipaddr[INET6_ADDRSTRLEN];
char input[1000];
char input_copy[1000];
char input_send_dest[1000];
char *part;
char *client_part;
char *userhostdef;
int sockfd, new_fd;
char s[INET6_ADDRSTRLEN];

int *flag;

int main(int argc, char *argv[])
{

    userhostdef = argv[1];
    server(argv[1]);

    if (!fork())
    {
        server_in_background();
    }

    printf("%s at port number: %d\n", local_ipaddr, local_portnumber);

    checkdir();

    createdir(argv[1]);

    write_nets_file(argv[1] , local_ipaddr , local_portnumber);  

    while (1)
    {
        printf("Linda>");
        fgets(input, sizeof (input), stdin); 
        strcpy(input_copy, input);
        strcpy(input_send_dest, input);
        part = strtok(input, ", ()");
        write_parsed_input_in_nets_tuples(part);
    }

    return 0;
}

