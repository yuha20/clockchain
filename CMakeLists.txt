cmake_minimum_required(VERSION 2.8)
project(project3)

#set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
link_libraries(pthread)
include_directories(.)
set(JSONCPP_SOURCES
        json/json_tool.h
        json/json_reader.cpp
        json/json_valueiterator.inl
        json/json_value.cpp
        json/json_writer.cpp
        )
add_executable(jsontest
        test/jsontest.cpp
        ${JSONCPP_SOURCES})
add_executable(jsontest1
        test/jsontest1.cpp
        ${JSONCPP_SOURCES})
add_executable(single
        Block.cpp
        Block.h
        Blockchain.cpp
        Blockchain.h
        test/main.cpp
        sha256.cpp
        ${JSONCPP_SOURCES})
add_executable(clockchain
        Block.cpp
        Block.h
        Blockchain.cpp
        Blockchain.h
        sha256.cpp
        sha256.h ChainServer.cpp
        ${JSONCPP_SOURCES})
add_executable(mingclient
        Block.cpp
        Block.h
        Blockchain.cpp
        Blockchain.h
        sha256.cpp
        sha256.h Mingclent.cpp
        ${JSONCPP_SOURCES} )
add_executable(queryclient
        Block.cpp
        Block.h
        Blockchain.cpp
        Blockchain.h
        sha256.cpp
        sha256.h Queryclient.cpp
        ${JSONCPP_SOURCES} )