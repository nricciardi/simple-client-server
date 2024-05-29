cmake_minimum_required(VERSION 3.28)
project(c_client_server C)

set(CMAKE_C_STANDARD 23)

add_executable(c_client_server
        server-split-manipulate-resend.c
        server.c

        #client-split-manipulate-resend.c
        client.c

        common.c
        common.h
)
