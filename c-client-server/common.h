#ifndef COMMON_H
#define COMMON_H

#endif //COMMON_H


#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdarg.h>


struct sockaddr_in build_ipv4_sockaddr(const char* address_or_name, short port);

struct sockaddr_in build_ipv4_any_sockaddr(short port);

long int parse_to_long_or_fail(const char* maybe_number);

char* str_concatenation(int n_strings, ...);

char* read_until_terminator_found(const int descriptor, const char* terminator, int terminator_len,  const int include_terminator, int* byte_read);

void write_n_bytes(const int descriptor, const char* bytes, const int n_bytes);

char* get_nth_str(const char* source, const char* delimiter, int nth);

char* str_to_upper(const char* source);

char* str_to_lower(const char* source);

char* zero_term(char* array, int len);

void require_n_params_or_fail(int n, int argc);

char* str_concat_array(char** str_array, int n_strings);

void write_until(int sockfd, char* to_send, char* terminator, int n_terminator, int included);

void send_file(int descriptor, char* filename, char* terminator, int n_terminator);

void receive_and_save_file(int descriptor, char* filename, char* terminator, int n_terminator, int include_term);

void save_on_file(char* filename, char* to_save, int n_to_save);