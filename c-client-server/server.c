//
// Created by ncla on 5/9/24.
//

#include "server.h"


int new_server(int domain, int type, int protocol) {
    int socketd = socket(domain, type, protocol);

    if (socketd < 0) {
        printf("ERROR: socket creation failed...\n");
        exit(1);

        return socketd;
    }

    printf("INFO: socket created");

    // struct sockaddr server_address;
    // bzero(&server_address, sizeof(server_address));
    //
    // server_address


    // int bind_res = bind(socketd, )

    return socketd;
}