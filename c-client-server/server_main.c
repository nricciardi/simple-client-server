#include <stdio.h>

#include "server.h"

int main(void) {

    int socketd = new_tcp_ipv4_server("127.0.0.1", 12345);

    listen(socketd, 5);

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    int communication_socket = accept(socketd, (struct sockaddr_in *) &client_addr, &client_addr_len);

    char buffer[256] = {0};

    read(communication_socket, buffer, 255);

    printf("%s", buffer);

    return 0;
}
