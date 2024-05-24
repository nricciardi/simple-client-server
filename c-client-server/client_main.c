//
// Created by Nicola Ricciardi.
//


#include "client.h"

// int argc, char *argv[]
int main(void) {

    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(12345);

    inet_aton("127.0.0.1", &server_address.sin_addr);

    int socketd = connect_to_tcp_ipv4_server(&server_address);

    char* hello = "hello";
    send(socketd, hello, sizeof(hello), 0);

    return 0;
}
