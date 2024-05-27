#include <stdio.h>

#include "common.h"
#include "cli.h"
#include "server.h"


void on_ipv4_server_start(int communication_socketd, struct sockaddr_in client_address) {

    int byte_read;
    char* result = read_until_terminator_found(communication_socketd, "\n", 0, &byte_read); // no zero-term
    result = zero_term(result, byte_read);

    char* content = get_nth_str(result, ":", 1);

    if(content == NULL) {
        printf("ERROR: string does not contain ':'\n");
        exit(1);
    }

    content = str_to_upper(content);

    char* packet = str_concatenation(2, "Value:", content);

    write_n_bytes(communication_socketd, packet, strlen(packet));
}


int main(int argc, char *argv[]) {

    require_n_params_or_fail(1, argc);

    short port = parse_to_long_or_fail(argv[1]);

    int socketd = new_tcp_ipv4_server("127.0.0.1", port);       // automatic htons

    start_singlethread_server(socketd, 6);
    // start_multiprocess_server(socketd, 6);


    return 0;
}
