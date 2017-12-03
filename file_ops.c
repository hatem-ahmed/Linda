#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define SHMSZ 50

void checkdir()
{
    char rmdir[50] = "rm -r /tmp/hahmed";
    DIR* dir = opendir("/tmp/hahmed");
    if (dir)
    {
        system(rmdir);
    }
    else
    {
        closedir(dir);
    }
}

void createdir(char* userhostdef_ptr)
{
    char mkdir_hahmed[50];
    char mkdir_linda[50];
    char mkdir_host[50];
    
    strcpy (mkdir_hahmed, "mkdir /tmp/hahmed");
    system (mkdir_hahmed);
    strcpy (mkdir_hahmed, "chmod 777 /tmp/hahmed");
    system (mkdir_hahmed);

    strcpy (mkdir_linda, "mkdir /tmp/hahmed/linda");
    system (mkdir_linda);
    strcpy (mkdir_linda, "chmod 777 /tmp/hahmed/linda");
    system (mkdir_linda);

    strcpy (mkdir_host, "mkdir /tmp/hahmed/linda/");
    strcat (mkdir_host, userhostdef_ptr);
    system (mkdir_host);

    strcpy (mkdir_host, "chmod 777 /tmp/hahmed/linda/");
    strcat (mkdir_host, userhostdef_ptr);
    system (mkdir_host);
}

void write_nets_file(char* userhostdef_ptr, char* local_ipaddr_ptr, char* local_port_ptr)
{
    FILE *fp;
    extern char filename[50];
    char comm666nets[50];
/*
    int shmid;
    key_t key;
    char *shm;
    key = 5678;
*/
    strcpy (filename, "/tmp/hahmed/linda/" );
    strcat (filename, userhostdef_ptr);
    strcat (filename, "/nets");
/*
    shmid = shmget(key, SHMSZ, IPC_CREAT | 0666);
    if (shmid < 0)
    {
	perror("shmget");
	exit(1);
    }

    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1)
    {
	perror("shmat");
	exit(1);
    }

    shm = filename;
printf("%s\n", shm);
*/
    fp = fopen(filename, "w+");
    fprintf(fp, "%s,%s,%d,\n", userhostdef_ptr, local_ipaddr_ptr, local_port_ptr);
    fclose(fp);

    strcpy (comm666nets, "chmod 666 ");
    strcat (comm666nets, filename);
    system (comm666nets);
}
void write_host_net_input(char* user_input)
{
    FILE *fp;
    extern char filename[];
    fp = fopen(filename, "a");
    fprintf(fp, "%s", user_input);
    fclose(fp);
}
void write_tuple_input(char* user_input)
{
    FILE *fp1;
    extern char filetuple[50];
    extern char *userhostdef;
    char comm666tup[50];

    strcpy (filetuple, "/tmp/hahmed/linda/" );
    strcat (filetuple, userhostdef);
    strcat (filetuple, "/tuples");

    fp1 = fopen(filetuple, "a");
    fprintf(fp1, "%s", user_input);
    fclose(fp1);

    strcpy (comm666tup, "chmod 666 ");
    strcat (comm666tup, filetuple);
    system (comm666tup);

}
void write_seperator ()
{
    FILE *fp;
    extern char filename[];
    fp = fopen(filename, "a");
    fprintf(fp, ",");
    fclose(fp);
}
void write_tuple_seperator ()
{
    FILE *fp1;
    extern char filetuple[];
    fp1 = fopen(filetuple, "a");
    fprintf(fp1, ",");
    fclose(fp1);
}
void write_eol ()
{
    FILE *fp;
    extern char filename[];
    fp = fopen(filename, "a");
    fprintf(fp, "\n");
    fclose(fp);
}

void hash_table_generation(int n)
{
    int offset = 0;
    int y = 0;
    int x = 0;
    int j = 0;
    int z = (x+(n/2)) % n;
    FILE *fphash;
    fphash = fopen("hashtablefile", "w+");

    for (y = 1; y<=n; y++)
    {
        for (j = ((offset*y)-offset); j<(65536/n)*y; j++)
        {
            fprintf(fphash, "%d", j);
            fprintf(fphash, ",");
            fprintf(fphash, "host_%d", x);
            fprintf(fphash, ",host_%d", z);
            fprintf(fphash, "\n");
        }
        offset = (65536/n);
        x++;
        z = (x+(n/2)) % n;
    }
    fclose(fphash);
}
void numberofhosts(int *n)
{
    int check = 0;
    char line[150];
    int x = 0;
    FILE *fp;
    extern char filename[];

    fp = fopen(filename, "r");
    while ( fgets( line , sizeof(line) , fp) != NULL)
    {
        check = strncmp(line,"host",4);
        x++; 
    }
    *n = x;
    fclose(fp);
}
int tuple_line_index(char *crit)
{
    FILE *fp;
    int z = 0;
    char line1[150];
    extern char filetuple[];
    char *useri;
    char *stored;
    char storedtuple[200];
    char outtuple[200];
    useri = strtok(crit, ", ()");
    useri = strtok(NULL, ", ()");
    strcpy(outtuple,useri);
    while (useri != NULL)
    {
        useri = strtok(NULL, ", ()");
        strcat(outtuple,useri);	
        if (*useri == '\n') break;
    }

    fp = fopen(filetuple, "r");

    while ( fgets( line1 , sizeof(line1) , fp) != NULL)
    {
        stored = strtok(line1, ",");
        strcpy(storedtuple,stored);
        while (stored != NULL)
        {
            stored = strtok(NULL, ",");
            strcat(storedtuple,stored);	
            if (*stored == '\n') break;
        }
        if (strcmp(outtuple,storedtuple) != 0)
        z++;
        else break;
    }
    fclose(fp);
    return z;
}
long hash_out_data(int line_index)
{
    char line[150];
    char command[150];
    FILE *fp;
    char *p;
    extern char filetuple[];
    char md5hash[100];
    long i;
    int lines = 0, z = 0;
    strcpy (command, "echo ");
    fp = fopen(filetuple, "r");
    int count = 0;

    while ( fgets( line , sizeof(line) , fp) != NULL)
    {
        if (count == line_index)
        {
            p = strtok(line,",");
            while (p != NULL)
            {
                strcat(command, p);
                p = strtok(NULL, ",");
                if (*p == '\n') break;
            } 	
            strcat(command, " | md5sum");
//printf("debug1%s\n", command);
            FILE *fpp;
            fpp = popen(command, "r");
            if (fpp == NULL) {
                printf("Failed to run command\n" );
                exit(1);
            }

            fgets(md5hash, sizeof(md5hash), fpp);
            pclose(fpp);
            char hash[5];
            strncpy(hash,md5hash,4);
            hash[4] = '\0';
            char *end;
//            printf("%s\n",hash);
            i = strtol(hash, &end, 16);
            if (p == "$" || p == "\n") break;
	    count++;
        }
        else count++;
    }
    return i;
    fclose(fp);
}
get_host(int index, char *Mhost, char *Shost)
{
    FILE *fphash;
    fphash = fopen("hashtablefile", "r");
    char line1[100];
    char *pointer;
    char ind[10];

    sprintf(ind , "%d" , index);
//    printf("%s\n",ind);
    while ( fgets( line1 , sizeof(line1) , fphash) != NULL)
    {
        pointer = strtok(line1, ",");
        if (strcmp(ind,pointer) == 0)
        {
            pointer = strtok(NULL, ",");
            strcpy(Mhost,pointer);
            pointer = strtok(NULL, ",\n");
            strcpy(Shost,pointer);
        }
        else continue;
    }
    fclose(fphash);
}
void get_dest_ip_port(char *Mhost, char *Shost, char *M_dest_ip, char *M_dest_port, char *S_dest_ip, char *S_dest_port)
{
    FILE *fp;
    extern char filename[];
    fp = fopen(filename, "r");
    char line1[100];
    char *pointer;
    char ind[10];

    while ( fgets( line1 , sizeof(line1) , fp) != NULL)
    {
        pointer = strtok(line1, ",");
        if (strcmp(pointer,Mhost) == 0)
        {
            pointer = strtok(NULL, ",");
            strcpy(M_dest_ip,pointer);
            pointer = strtok(NULL, ",");
            strcpy(M_dest_port,pointer);
        }
        else if (strcmp(pointer,Shost) == 0)
        {
            pointer = strtok(NULL, ",");
            strcpy(S_dest_ip,pointer);
            pointer = strtok(NULL, ",");
            strcpy(S_dest_port,pointer);
        }
        else continue;
    }
    fclose(fp);
}
void write_tuple_infile(char *part)
{
    int i = 0;
    while (part != NULL)
    {
        if (i == 0) {
            part = strtok(NULL, ", ()");
            i++; }
        else {
            write_tuple_input(part);
            part = strtok(NULL, ", ()");
            if (part == NULL) break;
            write_tuple_seperator();
            i++; }
    }
}
/*
void delete_tuple(int line_index)
{
    char line[150];
    FILE *fp;
    extern char filetuple[];
    char **tuples = malloc(10 *sizeof(char *));
    int i;
    int count = 0;
    for (i = 0; i < 150; i++)
    {
	tuples[i] = (char *)malloc(150);
    }
    fp = fopen(filetuple, "r");
    
    while ( fgets( line , sizeof(line) , fp) != NULL)
    {
	if (count != line_index)
	strcpy(tuples[count],line);
	count++;
    }
    fclose(fp);

    fp = fopen(filetuple, "w+");
    for (i = 0; i < count; i++)
    {
	fprintf(fp , "%s\n" , tuples[i]);
    }
    fclose(fp);
}
*/
void write_parsed_input_in_nets_tuples(char* part)
{
    int add = strcmp(part, "add");
    int in = strcmp(part, "in");
    int out = strcmp(part, "out");
    int rd = strcmp(part, "rd");
    char cl_ipaddr[50]; 
    char cl_port[50];
    extern char input_copy[1000];
    extern char input_send_dest[1000];
    extern char *userhostdef;

        long index;
        int line_index;
        char M_dest_ip[50];
        char M_dest_port[50];
        char S_dest_ip[50];
        char S_dest_port[50];
        char Mhost[10];
        char Shost[10];

    if (add == 0)
    {
        part = strtok(NULL, ", ()");
        while (part != NULL)
        {
            if (strlen(part) == 5) {
                write_host_net_input(part);
                write_seperator();
                write_eol();
                part = strtok(NULL, ", ()\n"); }
            else {
                write_host_net_input(part);
                write_seperator();
                part = strtok(NULL, ", ()\n"); }
        }
        int n;
        numberofhosts(&n);
        hash_table_generation(n);
        //	client(cl_ipaddr, cl_port, input_copy);
    }
    else if (rd == 0)
    {
        write_tuple_infile(part);
        line_index = tuple_line_index(input_copy);
        index = hash_out_data(line_index);
//printf("%ld\n", index);
        get_host(index, Mhost, Shost);
        get_dest_ip_port(Mhost, Shost, M_dest_ip, M_dest_port, S_dest_ip, S_dest_port);
        if (strcmp(Mhost,userhostdef) == 0) {
            printf ("Current Host is Master, get Tuple %s on %s ip: %s (%s)\n", input_send_dest, Mhost, M_dest_ip, M_dest_port); }

        else if (strcmp(Shost,userhostdef) == 0) {
            client(M_dest_ip, M_dest_port, input_send_dest);
            printf ("Current Host is Backup, get Tuple %s on Master %s ip: %s (%s)\n", input_send_dest, Mhost, M_dest_ip, M_dest_port); }

        else {
            client(M_dest_ip, M_dest_port, input_send_dest);
            printf ("get Tuple %s on Master %s ip: %s (%s)\n", input_send_dest, Mhost, M_dest_ip, M_dest_port);

            client(S_dest_ip, S_dest_port, input_send_dest); 
            printf ("get Tuple %s on Backup %s ip: %s (%s)\n", input_send_dest, Shost, S_dest_ip, S_dest_port);
	     }
//	    delete_tuple(line_index);
 
    }
    else if (out == 0)
    {
        write_tuple_infile(part);
        line_index = tuple_line_index(input_copy);
        index = hash_out_data(line_index);
//printf("%ld\n", index);
        get_host(index, Mhost, Shost);
        get_dest_ip_port(Mhost, Shost, M_dest_ip, M_dest_port, S_dest_ip, S_dest_port);
        if (strcmp(Mhost,userhostdef) == 0) {
            client(S_dest_ip, S_dest_port, input_send_dest);
            printf ("Current Host is Master, Put Tuple %s on Backup %s ip: %s (%s)\n", input_send_dest, Shost, S_dest_ip, S_dest_port); }

        else if (strcmp(Shost,userhostdef) == 0) {
            client(M_dest_ip, M_dest_port, input_send_dest);
            printf ("Current Host is Backup, Put Tuple %s on Master %s ip: %s (%s)\n", input_send_dest, Mhost, M_dest_ip, M_dest_port); }

        else {
            client(M_dest_ip, M_dest_port, input_send_dest);
            client(S_dest_ip, S_dest_port, input_send_dest); 
            printf ("Put Tuple %s on Master %s ip: %s (%s),and on Backup %s ip: %s (%s)\n", input_send_dest, Mhost, M_dest_ip, M_dest_port, Shost, S_dest_ip, S_dest_port);
//	    delete_tuple(line_index);
        }

    }
    else if (in == 0)
    {
        int i = 0;
        while (part != NULL)
        {
            if (i == 0) {
                part = strtok(NULL, ", ()");
                i++; }
            else {
                write_tuple_input(part);
                part = strtok(NULL, ", ()");
                if (part == NULL) break;
                write_tuple_seperator();
                i++; }
        }
    }
    else
    {
        printf("command in wrong format, please check\n");
    }
}
void write_client_net_input(char* user_input)
{
    FILE *fp;
    extern char* userhostdef;
    char filename[50];
    strcpy(filename, "/tmp/hahmed/linda/");
    strcat(filename, userhostdef);
    strcat(filename, "/nets");

    fp = fopen(filename, "a");
    fprintf(fp, "%s", user_input);
    fclose(fp);
}

void write_client_seperator ()
{
    FILE *fp;
    extern char *userhostdef;
    char filename[50];

    strcpy(filename, "/tmp/hahmed/linda/");
    strcat(filename, userhostdef);
    strcat(filename, "/nets");

    fp = fopen(filename, "a");
    fprintf(fp, ",");
    fclose(fp);
}

int  client_check_tuple(char *crit)
{
    FILE *fp;
    int z = 0;
    int linesno = 0;
    char line1[150];
    char *useri;
    char *stored;
    char storedtuple[200];
    char outtuple[200];
    extern char *userhostdef;
    char filetuple[50];

    strcpy(filetuple, "/tmp/hahmed/linda/");
    strcat(filetuple, userhostdef);
    strcat(filetuple, "/tuples");

    useri = strtok(NULL, ", ()");
    strcpy(outtuple,useri);

    while (useri != NULL)
    {
        useri = strtok(NULL, ", ()");
        strcat(outtuple,useri);
        if (*useri == '\n') break;
    }
//printf("%s\n", outtuple);
    fp = fopen(filetuple, "r");
    while ( fgets( line1 , sizeof(line1) , fp) != NULL)
    {
	linesno++;
    }
    fclose(fp);
//printf("%d\n",linesno);
    fp = fopen(filetuple, "r");
    while ( fgets( line1 , sizeof(line1) , fp) != NULL)
    {
        stored = strtok(line1, ",");
        strcpy(storedtuple,stored);
        while (stored != NULL)
        {
            stored = strtok(NULL, ",");
            strcat(storedtuple,stored);
            if (*stored == '\n') break;
        }
        if (strcmp(outtuple,storedtuple) != 0)
        z++;
        else {
	    z++;
	    break; } 
	if (z == linesno) z = 0;
//	else continue;
//printf("%d\n",z);
//printf("%s\n",storedtuple);
//if(z == linesno) z = 0;
    }
    fclose(fp);
    return z;
}
void write_client_input(char* buffer, char* ipaddr_sending, char* filename, char* filetuple)
{/*
 	int shmid;
	key_t key;
	char *shm;
	key  = 5678;
	if ((shmid = shmget(key, SHMSZ, 0666)) < 0) 
	{
		perror("shmget");
		exit(1);
	}
	if ((shm = shmat(shmid, NULL, 0)) == (char *) -1)
	{
		perror("shmat");
		exit(1);
	}
printf("hatem\n");
printf("%s\n", *shm);
*/
    char *client_part;
    char cl_ipaddr[50]; 
    char cl_port[50];
    extern char input_copy[1000];

    client_part = strtok(buffer, ", ()");
    int add = strcmp(client_part, "add");
    int in = strcmp(client_part, "in");
    int out = strcmp(client_part, "out");
    int rd = strcmp(client_part, "rd");

    if (add == 0)
    {
        int i = 0;
        while (client_part != NULL)
        {
            if (i == 0) {
                client_part = strtok(NULL, ", ()");
                i++; }
            else if (i == 2) {
                write_client_net_input(ipaddr_sending);
                client_part = strtok(NULL, ", ()");
                write_client_seperator();
                i++; }
            else {
                write_client_net_input(client_part);
                client_part = strtok(NULL, ", ()");
                if (client_part == NULL) break;
                write_client_seperator();
                i++; }
        }
    }
    else if (in == 0)
    {
        int i = 0;
        while (client_part != NULL)
        {
            if (i == 0) {
                client_part = strtok(NULL, ", ()");
                i++; }
            else {
                write_tuple_input(client_part);
                client_part = strtok(NULL, ", ()");
                if (client_part == NULL) break;
                write_tuple_seperator();
                i++; }
        }
    }
    else if (out == 0)
    {
        int i = 0;
        while (client_part != NULL)
        {
            if (i == 0) {
                client_part = strtok(NULL, ", ()");
                i++; }
            else {
                write_tuple_input(client_part);
                client_part = strtok(NULL, ", ()");
                if (client_part == NULL) break;
                write_tuple_seperator();
                i++; }
        }

    }
    else if (rd == 0)
    {
	int z = 0;
	z = client_check_tuple(client_part);
	if (z == 0)
	{
	    printf("Tuple is not avilable: ");
	    printf("%d\n",z);
	}
	else printf("%d\n",z);
    }
    else
    {
        printf("command in wrong format, please check\n");
    }
}

void write_client_tuple_seperator ()
{
    FILE *fp1;
    extern char* userhostdef;
    char filetuple[50];

    strcpy(filetuple, "/tmp/hahmed/linda/");
    strcat(filetuple, userhostdef);
    strcat(filetuple, "/tuples");


    fp1 = fopen(filetuple, "a");
    fprintf(fp1, ",");
    fclose(fp1);
}


