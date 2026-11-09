# libultraship (LUS) for the Turok 2 native port.
#
# Pinned to the epoch-2 release tag 2.0.0.0 (c6ecfdb4, 2026-08-15). Two ways to get it:
#   -DT2_LUS_SOURCE_DIR=/path/to/libultraship   use a local checkout (offline builds, LUS hacking)
#   (default)                                    FetchContent clones the pinned commit
# A git submodule at port/libultraship (the Ghostship/SpaghettiKart layout) is the
# long-term plan once the port has a real tree; see docs/port/LIBULTRASHIP.md.
#
# LUS pulls further deps with FetchContent at configure time (imgui docking, thread-pool,
# prism, monocypher, stb) and needs these system packages (Debian 13):
#   libsdl3-dev libzip-dev zipcmp zipmerge ziptool nlohmann-json3-dev libtinyxml2-dev
#   libspdlog-dev libglew-dev libpng-dev libgl-dev + the X11 -dev set in LUS's apt-deps.txt

set(T2_LUS_GIT_TAG c6ecfdb491cc3a0d08aa04eff2f17f0bf5946297 CACHE STRING "libultraship commit (tag 2.0.0.0)")
set(T2_LUS_SOURCE_DIR "" CACHE PATH "Local libultraship checkout (empty = FetchContent)")

# Turok 2 links "F3DEX.NoN fifo 2.05" + "L3DEX fifo 2.05" (1998): the F3DEX2 family, which
# is what the matching build compiles for (Makefile: -DF3DEX_GBI_2) and LUS's default.
set(GBI_UCODE F3DEX_GBI_2 CACHE STRING "GBI ucode for LUS" FORCE)
# No TCC scripting, no MPQ (.otr) support: the port reads one .o2r.
set(ENABLE_SCRIPTING OFF CACHE BOOL "" FORCE)
set(INCLUDE_MPQ_SUPPORT OFF CACHE BOOL "" FORCE)

if(T2_LUS_SOURCE_DIR)
    set(_t2_lus_dir ${T2_LUS_SOURCE_DIR})
else()
    include(FetchContent)
    FetchContent_Declare(libultraship
        GIT_REPOSITORY https://github.com/Kenix3/libultraship.git
        GIT_TAG ${T2_LUS_GIT_TAG}
        SOURCE_SUBDIR _t2_no_auto_add) # added below with our own binary dir
    FetchContent_MakeAvailable(libultraship)
    set(_t2_lus_dir ${libultraship_SOURCE_DIR})
endif()

add_subdirectory(${_t2_lus_dir} ${CMAKE_BINARY_DIR}/libultraship EXCLUDE_FROM_ALL)
set(T2_LUS_DIR ${_t2_lus_dir} CACHE INTERNAL "")
