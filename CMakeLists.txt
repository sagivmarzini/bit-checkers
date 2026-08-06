cmake_minimum_required(VERSION 4.3)
project(bit_checkers)

set(CMAKE_CXX_STANDARD 20)

add_executable(bit_checkers
        src/main.cpp
        src/Board.cpp
        src/Board.h
        src/Game.cpp
        src/Game.h
        src/InvalidMove.h
        src/MoveGenerator.cpp
        src/MoveGenerator.h
)
