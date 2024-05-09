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


#ifndef SERVER_H
#define SERVER_H

#endif



struct sockaddr_in build_sockaddr_in(char* address, short port);

int new_server(int domain, int type, int protocol);

int new_tcp_ipv4_server(char* address, short port);