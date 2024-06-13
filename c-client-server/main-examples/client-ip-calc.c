//
// Created by Nicola Ricciardi.
//


#include "common.h"
#include "client.h"


void start_client(int communication_socketd, char ip[]) {

    char hostname[255];

    gethostname(hostname, 255);

    char *request = str_concatenation(2, ip, "\n");
    int request_len = strlen(request);

    write_n_bytes(communication_socketd, request, request_len);

    int response_len;
    char* response = read_until_terminator_found(communication_socketd, "\n", 1, 0, &response_len);
    response = zero_term(response, response_len);
    response_len += 1;

    printf("response: %s", response);
}


int main(int argc, char *argv[]) {

    require_n_params_or_fail(2, argc);

    short port = 1025;

    struct sockaddr_in server_address;

    server_address = build_ipv4_sockaddr(argv[1], port);       // automatic htons

    int communication_socketd = connect_to_tcp_ipv4_server(&server_address);

    start_client(communication_socketd, argv[2]);

    return 0;
}
