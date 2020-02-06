cmake_minimum_required(VERSION 3.8)
project(network_middleware)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        _final/mw_internal.c
        _final/mw_internal.h
        _final/mw_public.c
        _final/mw_public.h
        circleTest/_app.c
        fullyConnected/include/config.h
        fullyConnected/include/fully_connected.h
        fullyConnected/include/fully_connected_connect.h
        fullyConnected/include/fully_connected_helpers.h
        fullyConnected/include/fully_connected_listen.h
        fullyConnected/src/config.c
        fullyConnected/src/fully_connected.c
        fullyConnected/src/fully_connected_connect.c
        fullyConnected/src/fully_connected_helpers.c
        fullyConnected/src/fully_connected_listen.c
        fullyConnected/src/fully_connected_test.c
        oneAppTest/_app.c
        oneAppTest/_background.c
        oneAppTest/_middleware.c
        oneAppTest/_middleware.h
        _client.c
        _input.c
        _msg.c
        _server.c
        config.c
        config.h
        exec.c
        fork.c
        fully_connected.c
        fully_connected_helpers.c
        print.c
        star_client.c
        star_serv.c
        tcp_client.c
        tcp_server.c _final/test/_starTest.c)

add_executable(network_middleware ${SOURCE_FILES})