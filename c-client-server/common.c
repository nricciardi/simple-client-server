//
// Created by Nicola Ricciardi.
//

#include "common.h"


/**
 *
 * Build an IPv4 address using address or name and port passed
 *
 * @param address_or_name
 * @param port
 * @return address struct
 */
struct sockaddr_in build_ipv4_sockaddr(const char* address_or_name, short port) {

    struct hostent *server = gethostbyname(address_or_name);

    if (server == NULL) {
        printf("ERROR: no such host '%s'\n", address_or_name);
    }

    struct sockaddr_in addr_in;

    addr_in.sin_family = AF_INET;

    bcopy((char *) server->h_addr, (char *) &addr_in.sin_addr, server->h_length);
    addr_in.sin_port = htons(port);

    return addr_in;
}


/**
 * Build address for any interfaces setting port
 *
 *
 * @param port
 * @return address struct
 */
struct sockaddr_in build_ipv4_any_sockaddr(short port) {

    struct sockaddr_in addr_in;

    addr_in.sin_family = AF_INET;

    addr_in.sin_addr.s_addr = INADDR_ANY;
    addr_in.sin_port = htons(port);

    return addr_in;
}


/**
 * Parse parameter to long.
 *
 * @param maybe_number
 * @return parsed parameter value
 */
long int parse_to_long_or_fail(const char* maybe_number) {
    char *end;

    long int value = strtol(maybe_number, &end, 10);

    if (end == maybe_number || *end != '\0') {
        printf("ERROR: '%s' is not a number\n", maybe_number);
        exit(1);
    }

    return value;
}

/**
 * Concatenate n strings and return result string
 *
 * @param n_strings number of strings to concatenate
 * @param ... strings...
 * @return result string
 */
char* str_concatenation(int n_strings, ...) {

    char *buffer, *next;
    va_list strings;

    buffer = malloc(sizeof(char));
    buffer[0] = '\0';

    va_start(strings, n_strings);

    for(int i = 0; i < n_strings; i++) {

        next = va_arg(strings, char*);

        buffer = realloc(buffer, sizeof(char) * (strlen(buffer) + 1 + strlen(next)));
        strncpy(buffer + strlen(buffer), next, strlen(next) + 1);

    }   va_end(strings);

    return buffer;
}

/**
 * Read from descriptor until terminator found.
 * byte_read will be set as result len.
 * Result is NOT zero-terminated.
 *
 * @param descriptor
 * @param terminator
 * @param include_terminator
 * @param byte_read
 * @return
 */
char* read_until_terminator_found(const int descriptor, const char* terminator, const int include_terminator, int* byte_read) {

    int buffer_len = strlen(terminator);
    char* buffer = calloc(buffer_len, sizeof(char));

    int result_len = 0;
    char* result = calloc(result_len, sizeof(char));

    if(result == NULL) {
        printf("ERROR: calloc return NULL...\n");
        exit(1);
    }

    int read_res = 0;
    do {
        read_res = read(descriptor, buffer, buffer_len);

        if (read_res < 0) {
            printf("ERROR: read error...\n");
        }

        int terminator_found = strcmp(buffer, terminator) == 0;

        if(terminator_found && !include_terminator) {
            printf("INFO: terminator '%s' found, but not included in result\n", terminator);
            break;
        }

        int new_result_len = result_len + buffer_len;
        result = realloc(result, new_result_len);

        if(result == NULL) {
            printf("ERROR: realloc return NULL...\n");
            exit(1);
        }

        memcpy(result + result_len, buffer, buffer_len);

        result_len = new_result_len;

        if(terminator_found) {
            printf("INFO: terminator '%s' found and included in result\n", terminator);
            break;
        }

        if (read_res == 0) {
            printf("INFO: EOF read\n");
            break;
        }

    } while(read_res > 0);


    *byte_read = result_len;
    return result;
}


void write_n_bytes(const int descriptor, const char* bytes, const int n_bytes) {

    int write_res = write(descriptor, bytes, n_bytes);

    if(write_res < 0) {
        printf("ERROR: write error...\n");
    }
}


/**
 * Return nth part of source string splitted by delimiter or NULL. nth is 0-based
 *
 * @param source
 * @param delimiter
 * @param nth 0-based position
 * @return
 */
char* get_nth_str(const char* _source, const char* delimiter, int nth) {

    char* source = calloc(strlen(_source) + 1, sizeof(char));
    strcpy(source, _source);

    char *last_token = NULL;
    char *current_token;
    int token_number = 0;

    // get the first token
    current_token = strtok(source, delimiter);

    /* walk through other tokens */
    while(current_token != NULL) {

        if(nth == token_number)
            return current_token;

        last_token = current_token;
        current_token = strtok(NULL, delimiter);

        if(current_token == NULL)
            return last_token;

        token_number += 1;
    }

    return NULL;
}

char* str_to_upper(const char* source) {

    char *result = calloc(strlen(source) + 1, sizeof(char));

    if(result == NULL) {
        printf("ERROR: calloc return NULL...\n");
        exit(1);
    }

    for(int i = 0; i < source[i] != 0; i += 1) {
        result[i] = toupper(source[i]);
    }

    result[strlen(result)] = 0;

    return result;
}

char* str_to_lower(const char* source) {

    char *result = calloc(strlen(source) + 1, sizeof(char));

    if(result == NULL) {
        printf("ERROR: calloc return NULL...\n");
        exit(1);
    }

    for(int i = 0; i < strlen(result); i += 1) {
        result[i] = tolower(source[i]);
    }

    result[strlen(result)] = 0;

    return result;
}

char* zero_term(const char* array, int len) {
    char* result = realloc(array, len + 1);

    if(result == NULL) {
        printf("ERROR: realloc return NULL...\n");
        exit(1);
    }

    result[len] = 0;

    return result;
}














