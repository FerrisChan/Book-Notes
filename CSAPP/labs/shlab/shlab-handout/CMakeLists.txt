cmake_minimum_required(VERSION 3.7)
project(shlab_handout)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        Makefile
        myint.c
        myspin.c
        mysplit.c
        mystop.c
        tsh.c)

add_executable(shlab_handout ${SOURCE_FILES})