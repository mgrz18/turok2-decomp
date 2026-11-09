# Turok 2 native port (Phase 7). Host build, separate from the matching N64
# build in the top-level Makefile (which never reads this directory).
#
#   cmake -S port -B <out> -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
#         [-DT2_LUS_SOURCE_DIR=/path/to/libultraship]
#   ninja -C <out> -k 0 turok2_game     # the game C as a host static library
#   ninja -C <out> turok2               # the executable on libultraship
#
# Pieces live in port/cmake/*.cmake so they merge independently:
#   game.cmake          turok2_game (docs/port/BUILD.md)
#   libultraship.cmake  libultraship pin + app/ (docs/port/LIBULTRASHIP.md)
cmake_minimum_required(VERSION 3.24)  # libultraship needs 3.24
project(turok2_port LANGUAGES C CXX)

set(T2_PORT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(T2_REPO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

option(T2_BUILD_GAME_LIB "Build turok2_game (the game C, host static library)" ON)
if(T2_BUILD_GAME_LIB)
    include(cmake/game.cmake)
endif()

option(T2_BUILD_EXE "Build the turok2 executable (links libultraship)" ON)
if(T2_BUILD_EXE)
    include(cmake/libultraship.cmake)
    add_subdirectory(app)
endif()
