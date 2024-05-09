#include <stdio.h>

#include "server.h"

int main(void) {

    int socketd = new_tcp_ipv4_server(0, 12345);

    return 0;
}
