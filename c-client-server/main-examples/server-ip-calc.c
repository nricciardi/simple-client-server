#include <stdio.h>

#include "common.h"
#include "server.h"

const char ERROR[] = "ERROR";


void on_ipv4_server_start(int communication_socketd, struct sockaddr_in client_address) {

    int byte_read;
    char* result = read_until_terminator_found(communication_socketd, "\n", 1, 0, &byte_read); // no zero-term
    result = zero_term(result, byte_read);

    char ip[4] = {0};
    char* ip_str[4];
    ip_str[0] = get_nth_str(result, ".", 0);
    ip_str[1] = get_nth_str(result, ".", 1);
    ip_str[2] = get_nth_str(result, ".", 2);
    ip_str[3] = get_nth_str(result, ".", 3);

    for (int i = 0; i < 4; ++i) {
        if(ip_str[i] == NULL) {
            printf("ERROR: ip %d is NULL\n", i);
            write_n_bytes(communication_socketd, ERROR, strlen(ERROR));
            exit(1);
        }

        char* endpointer;
        long int result = strtol(ip_str[i], &endpointer, 10);

        if(*endpointer != 0 || result < 0 || result > 255) {
            printf("ERROR: ip %d is invalid\n", i);
            write_n_bytes(communication_socketd, ERROR, strlen(ERROR));
            exit(1);
        }

        ip[i] = result;
    }

    char first_nibble = ((ip[0] & 0xF0) >> 4) & 0x0F;

    printf("INFO: first nibble: %d\n", first_nibble);

    if((first_nibble & 0x08) == 0) {     // class A 0xxx
        char* response = str_concatenation(5, "A ", ip_str[0], ".0.0.0 ", ip_str[0], ".255.255.255\n");
        write_n_bytes(communication_socketd, response, strlen(response));
        exit(0);
    }

    if((first_nibble & 0x0C) == 8) {     // class B 10xx
        char* response = str_concatenation(9, "B ", ip_str[0], ".", ip_str[1], ".0.0 ", ip_str[0], ".", ip_str[1], ".255.255\n");
        write_n_bytes(communication_socketd, response, strlen(response));
        exit(0);
    }

    if((first_nibble & 0x0E) == 12) {     // class C 110x
        char* response = str_concatenation(13, "C ", ip_str[0], ".", ip_str[1], ".", ip_str[2], ".0 ", ip_str[0], ".", ip_str[1], ".", ip_str[2], ".255\n");
        write_n_bytes(communication_socketd, response, strlen(response));
        exit(0);
    }

    if((first_nibble & 0x0F) == 14) {     // class D 1110
        char* response = str_concatenation(1, "D\n");
        write_n_bytes(communication_socketd, response, strlen(response));
        exit(0);
    }

    if((first_nibble & 0x0F) == 15) {     // class E 1111
        char* response = str_concatenation(1, "E\n");
        write_n_bytes(communication_socketd, response, strlen(response));
        exit(0);
    }

}


int main(int argc, char *argv[]) {

    short port = 1025;

    int socketd = new_tcp_ipv4_server("127.0.0.1", port);       // automatic htons

    //start_singlethread_server(socketd, 6);
    start_multiprocess_server(socketd, 6);


    return 0;
}
