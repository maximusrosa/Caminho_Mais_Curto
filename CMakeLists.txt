cmake_minimum_required(VERSION 3.29)
project(Caminho_Mais_Curto)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(Caminho_Mais_Curto
        include/graph.hpp
        src/ref/gen.cpp
        src/ref/leia.cpp
        src/main.cpp)
