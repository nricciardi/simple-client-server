//
// Created by ncla on 5/9/24.
//

#include "server.h"

struct sockaddr_in build_ipv4_sockaddr(char* address, short port) {
    struct sockaddr_in addr_in;

    addr_in.sin_family = AF_INET;
    addr_in.sin_addr.s_addr = inet_addr(address);
    addr_in.sin_port = htons(port);

    return addr_in;
}

int new_server(int domain, int type, int protocol, struct sockaddr* server_address, socklen_t address_length) {
    int socketd = socket(domain, type, protocol);

    if (socketd < 0) {
        printf("ERROR: socket creation failed...\n");
        exit(1);
    }

    printf("INFO: socket created");

    int bind_res = bind(socketd, server_address, address_length);

    if (bind_res < 0) {
        printf("ERROR: binding failed...\n");
        exit(1);
    }

    printf("INFO: socket bound");

    return socketd;
}

int new_tcp_ipv4_server(char* address, short port) {
    
}