# Turok 2 native port (Phase 7). The matching N64 build is the top-level Makefile and
# does not use this directory.
#
#   cmake -S port -B /path/to/out -G Ninja [-DT2_LUS_SOURCE_DIR=/path/to/libultraship]
#   cmake --build /path/to/out --target turok2
cmake_minimum_required(VERSION 3.24)
project(turok2_port LANGUAGES C CXX)

set(T2_PORT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(T2_REPO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

# ---- host executable on libultraship (docs/port/LIBULTRASHIP.md) ----
option(T2_BUILD_EXE "Build the turok2 executable (links libultraship)" ON)
if(T2_BUILD_EXE)
    include(cmake/libultraship.cmake)
    add_subdirectory(app)
endif()
