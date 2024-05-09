//
// Created by Nicola Ricciardi.
//


#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()


#ifndef CLIENT_SERVER_H
#define CLIENT_SERVER_H

#endif //CLIENT_SERVER_H


int new_server(int domain, int type, int protocol);


struct sockaddr_in build_sockaddr_in(char* address, short port);