# Turok 2 native port (Phase 7). Host build, separate from the matching N64
# build in the top-level Makefile (which never reads this directory).
#
#   cmake -S port -B <out> -G Ninja -DCMAKE_C_COMPILER=clang
#   ninja -C <out> -k 0 turok2_game
#
# Pieces live in port/cmake/*.cmake so they merge independently:
#   game.cmake  turok2_game, the game C as a host static library (docs/port/BUILD.md)
cmake_minimum_required(VERSION 3.20)
project(turok2_port C CXX)

option(T2_BUILD_GAME_LIB "Build turok2_game (the game C, host static library)" ON)

if(T2_BUILD_GAME_LIB)
    include(cmake/game.cmake)
endif()
