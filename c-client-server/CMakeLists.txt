cmake_minimum_required(VERSION 3.28)
project(c_client_server C)

set(CMAKE_C_STANDARD 23)

add_executable(c_client_server main.c
        server.h
        server.c)
