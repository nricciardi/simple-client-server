//
// Created by Nicola Ricciardi.
//


#include "client.h"

int connect_to_server(int domain, int type, int protocol, const struct sockaddr* server_address, socklen_t server_address_length) {
    int socketd = socket(domain, type, protocol);

    if (socketd < 0) {
        printf("ERROR: socket creation failed...\n");
        exit(1);
    }

    int connection_res = connect(socketd, server_address, server_address_length);

    if (connection_res < 0) {
        printf("ERROR: connection to server failed...\n");
        exit(1);
    }

    return socketd;
}

int connect_to_tcp_ipv4_server(const struct sockaddr_in* server_address) {
    return connect_to_server(AF_INET, SOCK_STREAM, 0, server_address, sizeof(struct sockaddr_in));
}