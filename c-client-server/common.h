//
// Created by Nicola Ricciardi.
//

#ifndef COMMON_H
#define COMMON_H

#endif //COMMON_H


#define _POSIX_C_SOURCE 200112L
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

long int parse_to_long_or_fail(const char* maybe_number);

char* str_concatenation(int n_strings, ...);

char* read_until_terminator_found(const int descriptor, const char* terminator, const int include_terminator, int* byte_read);

void write_n_bytes(const int descriptor, const char* bytes, const int n_bytes);

char* get_nth_str(const char* source, const char* delimiter, int nth);

char* str_to_upper(const char* source);

char* str_to_lower(const char* source);

char* zero_term(const char* array, int len);



