cmake_minimum_required(VERSION 4.3)
project(bit_checkers)

set(CMAKE_CXX_STANDARD 23)

find_package(SFML 3 CONFIG REQUIRED COMPONENTS Graphics Window System Audio)

add_executable(bit_checkers
        src/main.cpp
        src/Board.cpp
        src/Board.h
        src/Game.cpp
        src/Game.h
        src/InvalidMove.h
        src/MoveGenerator.cpp
        src/MoveGenerator.h
        src/IPlayer.h
        src/HumanPlayer.cpp
        src/HumanPlayer.h
        src/AiPlayer.cpp
        src/AiPlayer.h
        src/Renderer.cpp
        src/Renderer.h
)


target_link_libraries(bit_checkers PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
)