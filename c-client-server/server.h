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
#include <arpa/inet.h>


#ifndef SERVER_H
#define SERVER_H

#endif



struct sockaddr_in build_ipv4_sockaddr(const char* address_or_name, short port);

struct sockaddr_in build_ipv4_any_sockaddr(short port);

int new_server(int domain, int type, int protocol, const struct sockaddr* server_address, socklen_t address_length);

int new_tcp_ipv4_server(const char* address, short port);

void start_singlethread_server(int socketd, int client_queue_len);

void start_multiprocess_server(int socketd, int client_queue_len);

void on_ipv4_server_start(int communication_socketd, struct sockaddr_in client_address);