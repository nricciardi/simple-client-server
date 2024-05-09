//
// Created by ncla on 5/9/24.
//

#include "server.h"

struct sockaddr_in build_ipv4_sockaddr(const char* address_or_name, short port) {

    struct hostent *server = gethostbyname(address_or_name);

    if (server == NULL) {
        printf("ERROR: no such host '%s'\n", address_or_name);
    }

    struct sockaddr_in addr_in;

    addr_in.sin_family = AF_INET;

    // to bind all interfaces: addr_in.sin_addr.s_addr = INADDR_ANY;
    bcopy((char *) server->h_addr, (char *) &addr_in.sin_addr, server->h_length);
    addr_in.sin_port = htons(port);

    return addr_in;
}

int new_server(int domain, int type, int protocol, struct sockaddr* server_address, socklen_t address_length) {
    int socketd = socket(domain, type, protocol);

    if (socketd < 0) {
        printf("ERROR: socket creation failed...\n");
        exit(1);
    }

    printf("INFO: socket created\n");

    int bind_res = bind(socketd, server_address, address_length);

    if (bind_res < 0) {
        printf("ERROR: binding failed...\n");
        exit(1);
    }

    printf("INFO: socket bound\n");

    return socketd;
}

int new_tcp_ipv4_server(char* address, short port) {

    struct sockaddr_in addr = build_ipv4_sockaddr(address, port);

    return new_server(AF_INET, SOCK_STREAM, 0, (struct sockaddr*) &addr, sizeof(addr));
}