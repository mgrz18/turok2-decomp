# turok2_game: every C-origin file of the game (src/us/code/*.c) compiled for
# the 64-bit little-endian host as a static library (no link, no main yet).
# docs/port/BUILD.md explains the flags, the shim and what is left.
#
# port/include comes first on the include path and port/include/port_shim.h is
# force-included in every file: together they are the host shim layer.

set(T2_ROOT ${CMAKE_CURRENT_LIST_DIR}/../..)
get_filename_component(T2_ROOT ${T2_ROOT} ABSOLUTE)
set(T2_PORT ${T2_ROOT}/port)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

option(T2_WARN_64BIT "Report the 64-bit hazard warnings (pointer<->int)" ON)
option(T2_WARN_ALL "Report every clang warning, not just the 64-bit ones" OFF)

file(GLOB T2_GAME_ALL CONFIGURE_DEPENDS ${T2_ROOT}/src/us/code/*.c)

# Files whose code is the N64 itself, not the game: the native build supplies
# host versions instead (docs/port/BUILD.md, "Host replacements").
set(T2_GAME_EXCLUDE
    func_002A3AE0.c   # libgcc __divdi3     (MIPS longlong.h asm; host compiler-rt has it)
    func_002A40D0.c   # libgcc __moddi3
    func_002A4650.c   # libgcc __udivdi3
    func_002A4BE0.c   # libgcc __umoddi3
    func_0029DFC0.c   # osGetCount          (mfc0 Count)
    func_0029E210.c   # __osSetCompare      (mtc0 Compare)
    func_0029E3E0.c   # sprintf             (SN64 varargs __builtin_next_arg; host libc)
    func_002A259C.c   # HostIo_Call         (SN debug host I/O, break 1)
    func_00298470.c   # sqrtf               (sqrt.s; host libm)
)

set(T2_GAME_SOURCES)
set(T2_HOSTIFIED)
foreach(f ${T2_GAME_ALL})
    get_filename_component(name ${f} NAME)
    list(FIND T2_GAME_EXCLUDE ${name} ex)
    if(NOT ex EQUAL -1)
        continue()
    endif()
    # Register pins (asm("$f4") on a local), empty asm barriers, asm labels
    # and section attributes: compile a filtered host copy
    # (port/tools/hostify.py).
    file(STRINGS ${f} pins REGEX "(asm|__asm__)[ \t]*(volatile|__volatile__)?[ \t]*\\([ \t]*\"|section[ \t]*\\(")
    if(pins)
        set(out ${CMAKE_CURRENT_BINARY_DIR}/hostified/${name})
        add_custom_command(OUTPUT ${out}
            COMMAND ${Python3_EXECUTABLE} ${T2_PORT}/tools/hostify.py ${f} ${out}
            DEPENDS ${f} ${T2_PORT}/tools/hostify.py
            COMMENT "hostify ${name}" VERBATIM)
        list(APPEND T2_GAME_SOURCES ${out})
        list(APPEND T2_HOSTIFIED ${name})
    else()
        list(APPEND T2_GAME_SOURCES ${f})
    endif()
endforeach()
list(LENGTH T2_GAME_SOURCES n_src)
list(LENGTH T2_HOSTIFIED n_host)
message(STATUS "turok2_game: ${n_src} C files (${n_host} hostified)")

# Split off what the executable must not link (port/cmake/link.cmake, docs/port/LINK.md):
# module 2 and the libultra/boot files port/runtime replaces. They still compile, as
# turok2_game_unlinked, so "every C file builds for the host" stays checked.
include(${CMAKE_CURRENT_LIST_DIR}/link.cmake)
set(T2_GAME_LINKED)
set(T2_GAME_UNLINKED)
foreach(f ${T2_GAME_SOURCES})
    get_filename_component(name ${f} NAME)
    t2_is_module2(${name} mod2)
    list(FIND T2_LINK_REPLACED ${name} repl)
    if(mod2 OR NOT repl EQUAL -1)
        list(APPEND T2_GAME_UNLINKED ${f})
    else()
        list(APPEND T2_GAME_LINKED ${f})
    endif()
endforeach()
# Game-side host replacements (port/game: game headers and flags, e.g. the typed scheduler).
file(GLOB T2_PORT_GAME CONFIGURE_DEPENDS ${T2_PORT}/game/*.c)
list(APPEND T2_GAME_LINKED ${T2_PORT_GAME})
list(LENGTH T2_GAME_LINKED n_linked)
list(LENGTH T2_GAME_UNLINKED n_unlinked)
message(STATUS "turok2_game: ${n_linked} files linked, ${n_unlinked} compile-only (module 2 + replaced libultra)")

# Flags shared by both libraries (an interface target).
add_library(t2_game_flags INTERFACE)
target_include_directories(t2_game_flags BEFORE INTERFACE
    ${T2_PORT}/include
    ${T2_ROOT}/include)
target_compile_definitions(t2_game_flags INTERFACE
    TARGET_PORT=1 _LANGUAGE_C F3DEX_GBI_2 GAME_VERSION="us")

add_library(turok2_game STATIC ${T2_GAME_LINKED})
target_link_libraries(turok2_game PRIVATE t2_game_flags)
option(T2_BUILD_UNLINKED "Also compile the game files the executable does not link" ON)
if(T2_BUILD_UNLINKED)
    add_library(turok2_game_unlinked STATIC ${T2_GAME_UNLINKED})
    target_link_libraries(turok2_game_unlinked PRIVATE t2_game_flags)
endif()
target_compile_options(t2_game_flags INTERFACE
    -std=gnu89
    "SHELL:-include ${T2_PORT}/include/port_shim.h"
    # MIPS GCC: plain char is signed (arm64 hosts default to unsigned).
    -fsigned-char
    # The decomp punning (M2C_FIELD, s32<->f32 views) is everywhere.
    -fno-strict-aliasing -fwrapv
    # The R4300 has no fused multiply-add: arm64 clang contracts a * b + c into fmadd by
    # default, which changes the float results, so the attract demos (recorded inputs
    # replayed through the physics) drift away from the x86-64 runs. Same results everywhere.
    -ffp-contract=off
    # Some decompiled code still reads a local before writing it (round 13 b4): the value
    # then depends on the host, the optimisation level and the code layout. Linux Debug and
    # macOS split at level 135 (frame 71,469 of the whole-game run: a particle 0.7 units off,
    # then the player's aim), and a -O2 build no longer finished the game. Zeroed locals give
    # the same run everywhere (Mac Debug = Mac -O2 = Linux, 266,000 frames).
    # Tentative definitions repeated across files (SN64 linked them as common).
    -fcommon
    # Clang 16+ makes these errors in C; the code predates them. They stay
    # warnings so the 64-bit audit can count them.
    -Wno-error=int-conversion
    -Wno-error=incompatible-pointer-types
    -Wno-error=implicit-function-declaration
    -Wno-error=implicit-int
    -Wno-error=return-type
)
# Round 14 c5: T2_AUTO_VAR_INIT = zero (default), pattern or none (the compiler's default: to
# look for reads of unset locals, see docs/port/STABILITY.md section 10).
set(T2_AUTO_VAR_INIT "zero" CACHE STRING "-ftrivial-auto-var-init for the game code: zero, pattern or none")
if(NOT T2_AUTO_VAR_INIT STREQUAL "none")
    target_compile_options(t2_game_flags INTERFACE -ftrivial-auto-var-init=${T2_AUTO_VAR_INIT})
endif()
if(T2_WARN_ALL)
    # everything clang reports by default
elseif(T2_WARN_64BIT)
    target_compile_options(t2_game_flags INTERFACE
        -Wno-everything
        -Wint-conversion -Wint-to-pointer-cast -Wpointer-to-int-cast
        -Wshorten-64-to-32 -Wvoid-pointer-to-int-cast
        # an implicit declaration returns int: a pointer result is truncated
        -Wimplicit-function-declaration -Wimplicit-int
        # a host libc function redeclared with the N64 types (size_t = u32)
        -Wincompatible-library-redeclaration)
else()
    target_compile_options(t2_game_flags INTERFACE -w)
endif()
