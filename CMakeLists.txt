cmake_minimum_required(VERSION 3.12)
project(prodconmaster)

set(CMAKE_CXX_STANDARD 14)

find_package(Threads)

add_executable(prodconmaster main.cpp
        ProducerConsumerMaster.cpp ProducerConsumerMaster.h
        BearToPingu.cpp BearToPingu.h
        BearGenerator.cpp BearGenerator.h
        PinguToKoal.cpp PinguToKoal.h)

target_link_libraries(prodconmaster ${CMAKE_THREAD_LIBS_INIT})