#include <stdio.h>

#include "common.h"
#include "server.h"


void on_ipv4_server_start(int communication_socketd, struct sockaddr_in client_address) {

    char* filepath = "test.txt";

    char *header = str_concatenation(3, "File:", filepath, "\n---\n");

    write_n_bytes(communication_socketd, header, strlen(header));
    send_file(communication_socketd, filepath, NULL, 0);
}


int main(int argc, char *argv[]) {

    require_n_params_or_fail(1, argc);

    short port = parse_to_long_or_fail(argv[1]);

    int socketd = new_tcp_ipv4_server("127.0.0.1", port);       // automatic htons

    start_singlethread_server(socketd, 6);
    // start_multiprocess_server(socketd, 6);


    return 0;
}
