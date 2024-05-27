//
// Created by ncla on 5/9/24.
//

#include "server.h"




/**
 *
 * @param domain IP family (e.g AF_INET for IPv4)
 * @param type socket type (e.g. SOCK_STREAM for TCP)
* @param protocol specifies a particular protocol to be used with the
       socket.  Normally only a single protocol exists to support a
       particular socket type within a given protocol family, in which
       case protocol can be specified as 0.
 * @param server_address
 * @param address_length
 * @return server socket descriptor
 */
int new_server(int domain, int type, int protocol, const struct sockaddr* server_address, socklen_t address_length) {
    int socketd = socket(domain, type, protocol);

    if (socketd < 0) {
        printf("ERROR: socket creation failed...\n");
        exit(1);
    }

    printf("INFO: socket created\n");

    int opt = 1;
    if (setsockopt(socketd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        printf("ERROR: SO_REUSEADDR, SO_REUSEPORT not set\n");
        exit(1);
    }

    printf("INFO: SO_REUSEADDR, SO_REUSEPORT set\n");

    int bind_res = bind(socketd, server_address, address_length);

    if (bind_res < 0) {
        printf("ERROR: binding failed...\n");
        exit(1);
    }

    printf("INFO: socket bound\n");

    return socketd;
}

/**
 * Create new TCP IPv4 server
 * @param address
 * @param port
 * @return
 */
int new_tcp_ipv4_server(const char* address, short port) {

    struct sockaddr_in addr = build_ipv4_sockaddr(address, port);

    return new_server(AF_INET, SOCK_STREAM, 0, (struct sockaddr*) &addr, sizeof(addr));
}

/**
 * Start a new single thread server in loop.
 * Must be implemented function "on_ipv4_server_start"
 *
 * @param socketd
 * @param client_queue_len
 */
void start_singlethread_server(int socketd, int client_queue_len) {
    int result = listen(socketd, client_queue_len);

    if (result < 0) {
        printf("ERROR: listen failed...\n");
        exit(1);
    }

    printf("INFO: single thread server listening...\n");

    int communication_socketd;
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    while(1) {
        printf("INFO: waiting a new connection...\n");

        communication_socketd = accept(socketd, (struct sockaddr *) &client_address, &client_address_len);

        if(communication_socketd < 0) {
            printf("ERROR: accept failed...\n");
            exit(1);
        }

        printf("INFO: accepted new client %s:%d...\n", inet_ntoa(client_address.sin_addr), client_address.sin_port);

        on_ipv4_server_start(communication_socketd, client_address);

        close(communication_socketd);
        printf("INFO: closed communication socket with %s:%d\n", inet_ntoa(client_address.sin_addr), client_address.sin_port);
    }
}

/**
 * Start a new multi-process server in loop.
 * Must be implemented function "on_ipv4_server_start"
 *
 * @param socketd
 * @param client_queue_len
 */
void start_multiprocess_server(int socketd, int client_queue_len) {
    int result = listen(socketd, client_queue_len);

    if (result < 0) {
        printf("ERROR: listen failed...\n");
        exit(1);
    }

    printf("INFO: multi-thread server (PID: %d) listening...\n", getpid());

    int communication_socketd;
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    while(1) {
        printf("INFO: waiting a new connection...\n");

        communication_socketd = accept(socketd, (struct sockaddr *) &client_address, &client_address_len);

        if(communication_socketd < 0) {
            printf("ERROR: accept failed...\n");
            exit(1);
        }

        printf("INFO: accepted new client %s:%d...\n", inet_ntoa(client_address.sin_addr), client_address.sin_port);

        int fork_pid = fork();

        if(fork_pid < 0) {
            printf("ERROR: fork failed...\n");
            exit(1);
        }


        if(fork_pid == 0) {         // child

            printf("INFO: communication elaboration in child process with PID: %d\n", getpid());

            on_ipv4_server_start(communication_socketd, client_address);

            close(communication_socketd);
            printf("INFO: closed communication socket with %s:%d\n", inet_ntoa(client_address.sin_addr), client_address.sin_port);

            exit(0);
        }

    }
}