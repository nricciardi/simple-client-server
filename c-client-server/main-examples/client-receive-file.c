//
// Created by Nicola Ricciardi.
//


#include "common.h"
#include "client.h"


void start_client(int communication_socketd) {

    int byte_read;
    char* result = read_until_terminator_found(communication_socketd, "\n---\n", 5, 0, &byte_read); // no zero-term
    result = zero_term(result, byte_read);

    char* filepath = get_nth_str(result, ":", 1);

    receive_and_save_file(communication_socketd, filepath, NULL, 0, 0);
}


int main(int argc, char *argv[]) {

    require_n_params_or_fail(1, argc);

    short port = parse_to_long_or_fail(argv[1]);

    struct sockaddr_in server_address;

    server_address = build_ipv4_sockaddr("127.0.0.1", port);       // automatic htons

    int communication_socketd = connect_to_tcp_ipv4_server(&server_address);

    start_client(communication_socketd);

    return 0;
}
