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

    bcopy((char *) server->h_aliases, (char *) &addr_in.sin_addr, server->h_length);
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
char* read_until_terminator_found(const int descriptor, const char* terminator, int terminator_len, const int include_terminator, int* byte_read) {

    int buffer_len = 1;
    char buffer[1];

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

        int new_result_len = result_len + buffer_len;
        result = realloc(result, new_result_len);

        if(result == NULL) {
            printf("ERROR: realloc return NULL...\n");
            exit(1);
        }

        memcpy(result + result_len, buffer, buffer_len);

        result_len = new_result_len;

        int terminator_found = (memcmp(result + result_len - terminator_len, terminator, terminator_len)) == 0;

        if(terminator_found) {

            if(!include_terminator) {
                printf("INFO: terminator '%s' found, but not included in result\n", terminator);

                result_len -= terminator_len;
                memcpy(result, result, result_len);

                break;
            } else {
                printf("INFO: terminator '%s' found and included in result\n", terminator);
                break;
            }
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

    for(int i = 0; source[i] != 0; i += 1) {
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

char* zero_term(char* array, int len) {
    char* result = realloc(array, len + 1);

    if(result == NULL) {
        printf("ERROR: realloc return NULL...\n");
        exit(1);
    }

    result[len] = 0;

    return result;
}

/**
 * Validate number of parameters. It must be exactly n.
 *
 * @param n number of params required
 * @param argc actual parameters number
 */
void require_n_params_or_fail(const int n, const int argc) {

    if(argc - 1 < n) {
        printf("ERROR: too few params\n");
        exit(1);
    }

    if(argc - 1 > n) {
        printf("ERROR: too many params\n");
        exit(1);
    }
}

/**
 * Creates a new string from a concatenation of existing ones in an array,
 * 
 * @param str_array the string array
 * @param n_strings the number of strings
 * @attention if n_strings is -1, the str_array must have a NULL terminating pointer
*/
char* str_concat_array(char** str_array, int n_strings) {

    char* buffer;
    int i, size, tot_size;

    tot_size = 0;
    buffer = malloc(sizeof(char) * (tot_size + 1));

    for(i = 0; 
        ( (n_strings == -1) ? (str_array[i] != NULL) : (i < n_strings) );
        i++) {

        size = strlen(str_array[i]);
        tot_size += size;
        buffer = realloc(buffer, tot_size + 1);

        strncpy(buffer + tot_size - size, str_array[i], size);

    }

    buffer[tot_size] = '\0';
    return buffer;

}

/**
 * Sends to descriptor to_send, until terminator
 * 
 * @param descriptor the descriptor
 * @param to_send the memory to send
 * @param terminator the terminating string
 * @param n_terminator the length of the terminator
 * @param included to know if the terminator is to be sent
*/
void write_until(int descriptor, char* to_send, char* terminator, int n_terminator, int included) {

    char* c;

    c = to_send;

    while(memcmp(c++, terminator, n_terminator) != 0);

    write_n_bytes(descriptor, to_send, c - to_send + (included ? n_terminator : 0));

}

/**
 * Sends the file called 'filename' onto descriptor.
 * 
 * @param descriptor to which send the file
 * @param filename the file to send
 * @param terminator the bytes to put at the end of the send (if NUll => EOF)
 * @param n_terminator the length of the teminator (if terminator is not NULL)
*/
void send_file(int descriptor, char* filename, char* terminator, int n_terminator) {

    FILE* f;
    char buffer[1024], eof_str;
    int n;

    // Si potrebbe togliere :P
    bzero( (void*) buffer, 1024);

    eof_str = -1;

    f = fopen(filename, "r");
    if(f == NULL) {
        printf("ERROR: Opening %s\n", filename);
        exit(1);
    }

    while( (n = fread(buffer, sizeof(char), 1024, f)) == 1024 )
        write_n_bytes(descriptor, buffer, 1024);

    if( (n > 0) && (n < 1024) )
        write_n_bytes(descriptor, buffer, n);

    if(n < 0) {
        printf("ERROR: While reading from %s\n", filename);
        exit(2);
    }

    write_n_bytes(descriptor, 
                    (terminator == NULL) ? &eof_str : terminator, 
                    (terminator == NULL) ?     1    : n_terminator );

    fclose(f);

}

/**
 * Receives data and saves it onto 'filename'
 * 
 * @param descriptor from which receive the data
 * @param filename the file on which to save the data read
 * @param terminator the terminating sequence of the data (if NULL => EOF)
 * @param n_terminator the length of the terminator (if not NULL)
 * @param include_term to include the terminator in the data to save
*/
void receive_and_save_file(int descriptor, char* filename, char* terminator, int n_terminator, int include_term) {
    
    char* buffer, eof_string;
    int tot_size;

    eof_string = -1;
    
    buffer = read_until_terminator_found(descriptor, (terminator == NULL) ? &eof_string : terminator, (terminator == NULL) ? 1 : n_terminator, include_term, &tot_size);

    save_on_file(filename, buffer, tot_size);

}

/**
 * Saves on file 'filename' the 'to_save' bytes
 * 
 * @param filename the name of the file
 * @param to_save the bytes to save
 * @param n_to_save the num of bytes to save
*/

void save_on_file(char* filename, char* to_save, int n_to_save) {

    FILE* f;

    f = fopen(filename, "w");
    if(f == NULL) {
        printf("ERROR: While reading from %s\n", filename);
        exit(1);
    }

    if(n_to_save != fwrite(to_save, sizeof(char), n_to_save, f))
        printf("WARNING: An error occured while writing onto %s\n", filename);

    fclose(f);

}