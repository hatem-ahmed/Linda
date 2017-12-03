struct sockaddr;
void sigchld_handler(int s);
void *get_in_addr(struct sockaddr *sa);
void server();
void server_in_background();
