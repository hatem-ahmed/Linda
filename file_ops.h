void checkdir();
void createdir(char* userhostdef_ptr);
void write_nets_file(char* userhostdef_ptr, char* local_ipaddr_ptr, int local_port_ptr);
void write_tuple_input(char* user_input);
void write_host_net_input(char* user_input);
void write_parsed_input_in_nets_tuples(char* part);
void write_client_input(char* buffer, char* ipaddr_sending, char* filename, char* filetuple);
void write_client_net_input(char* user_input, char* filename);
void write_client_seperator (char* filename);
void write_client_tuple_seperator (char* filetuple);
void write_eol();
