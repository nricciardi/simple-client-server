cmake_minimum_required(VERSION 3.28)
project(c_client_server C)

set(CMAKE_C_STANDARD 23)

add_executable(c_client_server
        # server_main.c
        # server.c

        client_main.c

        cli.c
        cli.h
        client.c
        common.c
        common.h
)
