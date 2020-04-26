cmake_minimum_required(VERSION 3.10)
project(SpreadsheetServer)

set(CMAKE_CXX_STANDARD 98)

include_directories(.)

add_executable(SpreadsheetServer
        spreadsheet_state.cpp
        spreadsheet_state.h
        session_dispatcher.cpp
        session_dispatcher.h
        threading.cpp
        threading.h
        server.cpp
        server.h
        client.cpp
    client.h
    handlers.cpp
    handlers.h
        Message.cpp
    Message.h
    message_handler.cpp
    message_handler.h )
