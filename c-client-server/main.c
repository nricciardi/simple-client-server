#include <stdio.h>

#include "server.h"

int main(void) {
    struct sockaddr_in sockaddr_in = build_sockaddr_in("127.0.0.1", 12345);

    int socketd = new_server()

    return 0;
}
