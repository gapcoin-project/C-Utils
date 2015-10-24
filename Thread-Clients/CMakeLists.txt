cmake_minimum_required(VERSION 2.8)

PROJECT(C-Utils)

add_library(Thread-Clients STATIC src/thread-clients.c)
add_library(Thread-Client STATIC src/thread-client.c)
