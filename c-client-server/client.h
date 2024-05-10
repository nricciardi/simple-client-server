//
// Created by ncla on 5/9/24.
//

#ifndef CLIENT_H
#define CLIENT_H

#endif //CLIENT_H

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#include <arpa/inet.h>

int connect_to_server(int domain, int type, int protocol, const struct sockaddr* server_address, socklen_t server_address_length);

int connect_to_tcp_ipv4_server(const struct sockaddr_in* server_address);