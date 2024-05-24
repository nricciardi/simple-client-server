#include <stdio.h>

#include "server.h"

void on_ipv4_server_start(int communication_socketd, struct sockaddr_in client_address) {

    size_t buffer_len = 1024;
    char buffer[1024] = {0};
    int read_res = 0;

    do {
        bzero(buffer, buffer_len);

        read_res = read(communication_socketd, buffer, buffer_len - 1);     // -1 to assicure last byte as 0

        if (read_res < 0) {
            printf("ERROR: read error...\n");
        }

        if (read_res == 0) {
            printf("INFO: EOF read\n");
            break;
        }


        printf("INFO: read %d bytes:\n", read_res);
        printf("%s\n", buffer);

    } while(read_res > 0);
}


// int argc, char *argv[]
int main(void) {

    int socketd = new_tcp_ipv4_server("127.0.0.1", 12345);

    start_singlethread_server(socketd, 6);
    // start_multiprocess_server(socketd, 6);


    return 0;
}
