//
// Created by Nicola Ricciardi.
//


#include "common.h"
#include "client.h"


void start_client(int communication_socketd) {

    char* filepath = "test.txt";

    char *header = str_concatenation(3, "File:", filepath, "\n---\n");

    write_n_bytes(communication_socketd, header, strlen(header));
    send_file(communication_socketd, filepath, NULL, 0);
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
