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
#   data.cmake          turok2_data, the generated game data (docs/port/DATA.md)
cmake_minimum_required(VERSION 3.24)  # libultraship needs 3.24
project(turok2_port LANGUAGES C CXX)

set(T2_PORT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(T2_REPO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
# tools/port/gendata.py reads each game file's compile flags from compile_commands.json;
# set before any target exists (the property is taken at target creation).
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ThinLTO (round 16 e4, MACOS.md 18): links on macOS since hostify keeps the label targets
# (`__attribute__((used))`); the whole-game det log is identical, but the run is not faster on
# an M5 (42.5-43.5 s vs 40.0-43.3 s), so it is off by default.
option(T2_LTO "ThinLTO (-flto=thin) for every target" OFF)
if(T2_LTO)
    string(APPEND CMAKE_C_FLAGS " -flto=thin")
    string(APPEND CMAKE_CXX_FLAGS " -flto=thin")
    string(APPEND CMAKE_EXE_LINKER_FLAGS " -flto=thin")
    message(STATUS "turok2: ThinLTO on")
endif()

option(T2_BUILD_GAME_LIB "Build turok2_game (the game C, host static library)" ON)
if(T2_BUILD_GAME_LIB)
    include(cmake/game.cmake)
    include(cmake/data.cmake)
endif()

option(T2_BUILD_EXE "Build the turok2 executable (links libultraship)" ON)
if(T2_BUILD_EXE)
    include(cmake/libultraship.cmake)
    add_subdirectory(app)
endif()
