#include <stdio.h>

#include "common.h"
#include "server.h"

const char* ERR_MSG = "- ERR\r\n";


void on_ipv4_server_start(int communication_socketd, struct sockaddr_in client_address) {

    int byte_read;
    char* result = read_until_terminator_found(communication_socketd, "\r\n", 2, 0, &byte_read); // no zero-term
    result = zero_term(result, byte_read);

    printf("INFO: read seed and niters\n");

    char* seed = get_nth_str(result, ",", 0);
    char* niters = get_nth_str(result, ",", 1);

    if(seed == NULL || niters == NULL) {
        write_n_bytes(communication_socketd, ERR_MSG, strlen(ERR_MSG));
        printf("ERROR: seed or niters NULL\n");
        exit(1);
    }

    if(strlen(seed) != 1 || !isdigit(seed[0])) {
        write_n_bytes(communication_socketd, ERR_MSG, strlen(ERR_MSG));
        exit(1);
    }

    int i_seed;
    int res = strict_str_to_int(seed, &i_seed);

    if(res < 0) {

        write_n_bytes(communication_socketd, ERR_MSG, strlen(ERR_MSG));
        printf("ERROR: i_seed error\n");
        exit(1);
    }

    int i_niters;
    res = strict_str_to_int(niters, &i_niters);

    if(res < 0) {
        write_n_bytes(communication_socketd, ERR_MSG, strlen(ERR_MSG));
        printf("ERROR: i_niters error\n");
        exit(1);
    }

    char ok_msg[1024];

    sprintf(ok_msg, "+ OK %d iterations on seed %d\r\n", i_niters, i_seed);

    write_n_bytes(communication_socketd, ok_msg, strlen(ok_msg));

    char current_result[4096] = {0};
    strncpy(current_result, seed, 1);
    char* next_result = calloc(4096, 0);
    for (int i = 0; i < i_niters; ++i) {

        printf("INFO: iteration %d\n", i + 1);

        int current_digit_index = 0;
        int current_digit_group_size = 0;
        char last_digit = 0;
        char current_digit;

        while(1) {

            current_digit = current_result[current_digit_index];

            if(current_digit != last_digit && current_digit_group_size > 0) {

                char current_digit_group_size_str[255];
                sprintf(current_digit_group_size_str, "%d", current_digit_group_size);
                char current_digit_str[255];
                sprintf(current_digit_str, "%c", last_digit);
                next_result = str_concatenation(3, next_result, current_digit_group_size_str, current_digit_str);

                current_digit_group_size = 0;
            }

            if(current_digit == 0)
                break;

            current_digit_group_size += 1;
            last_digit = current_digit;


            current_digit_index += 1;
        }

        char *to_send = str_concatenation(2, next_result, "\r\n");
        write_n_bytes(communication_socketd, to_send, strlen(to_send));

        strncpy(current_result, next_result, strlen(next_result));
        bzero(next_result, strlen(next_result));

    }

    free(next_result);
}


int main(int argc, char *argv[]) {

    // require_n_params_or_fail(1, argc);

    short port = 8080;

    int socketd = new_tcp_ipv4_server("127.0.0.1", port);       // automatic htons

    //start_singlethread_server(socketd, 6);
    start_multiprocess_server(socketd, 6);


    return 0;
}
