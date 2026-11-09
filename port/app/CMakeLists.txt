# turok2: the host executable (skeleton). See docs/port/LIBULTRASHIP.md.
add_library(t2asset STATIC
    ${T2_PORT_DIR}/assets/t2asset.c
    ${T2_PORT_DIR}/assets/rnc.c
    ${T2_PORT_DIR}/third_party/miniz/miniz.c)
target_include_directories(t2asset PUBLIC ${T2_PORT_DIR}/assets)
# miniz must not export zlib names: LUS links libzip, which links the system zlib.
target_compile_definitions(t2asset PRIVATE MINIZ_NO_ZLIB_COMPATIBLE_NAMES _LARGEFILE64_SOURCE)
set_source_files_properties(${T2_PORT_DIR}/third_party/miniz/miniz.c PROPERTIES COMPILE_OPTIONS "-w")

add_executable(turok2 main.cpp t2asset_lus.cpp testdl.c)
set_target_properties(turok2 PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON C_STANDARD 11)
# The game's GBI family: F3DEX2 (F3DEX.NoN fifo 2.05 + L3DEX 2.05, include/globals/ucode.h).
# Must match libultraship's GBI_UCODE so macros encode what the interpreter decodes.
target_compile_definitions(turok2 PRIVATE ${GBI_UCODE})
target_link_libraries(turok2 PRIVATE libultraship t2asset)

# The game linked into turok2 (docs/port/LINK.md): turok2_game (engine + module 1, minus the
# files port/runtime replaces) + t2runtime (t2os threads, device shims, boot glue) + the
# generated WEAK placeholders for game data/functions not defined in C yet, linked last so
# any real definition wins. `turok2 --game` runs it.
option(T2_LINK_GAME "Link the game library into turok2" ON)
if(T2_LINK_GAME AND TARGET turok2_game)
    add_library(t2runtime STATIC
        ${T2_PORT_DIR}/runtime/t2os.c
        ${T2_PORT_DIR}/runtime/t2ultra.c
        ${T2_PORT_DIR}/runtime/t2boot.c
        ${T2_PORT_DIR}/runtime/t2loader.c)
    target_include_directories(t2runtime PUBLIC ${T2_PORT_DIR}/runtime PRIVATE ${T2_PORT_DIR}/include)
    target_compile_definitions(t2runtime PRIVATE T2_RENAME_NO_LIBC)
    set_target_properties(t2runtime PROPERTIES C_STANDARD 11)
    # t2os.c defines the libultra names: compile it under the game's t2_ names
    set_source_files_properties(${T2_PORT_DIR}/runtime/t2os.c PROPERTIES
        COMPILE_OPTIONS "-include;${T2_PORT_DIR}/include/t2_rename.h")
    find_package(Threads REQUIRED)
    # RNC_UnpackMethod1/2 (t2boot.c) use the clean-room decompressor in t2asset (port/assets/rnc.c)
    target_link_libraries(t2runtime PUBLIC Threads::Threads m t2asset)

    add_library(t2placeholders STATIC ${T2_PORT_DIR}/runtime/t2_placeholders.c)
    target_compile_options(t2placeholders PRIVATE -w)

    # q2's generated data (port/cmake/data.cmake; port/gen/ from tools/port/gendata.py), in the
    # rescanned group: the data points at game functions and the game names the data.
    set(_t2_data "")
    set(_t2_data_group "")
    if(TARGET turok2_data)
        set(_t2_data turok2_data)
        set(_t2_data_group ",turok2_data")
    else()
        message(WARNING "turok2: no generated data in port/gen (tools/port/gendata.py): "
                        "every game data symbol falls to a zero placeholder")
    endif()

    target_sources(turok2 PRIVATE game.cpp)
    target_compile_definitions(turok2 PRIVATE T2_HAVE_GAME=1)
    if(APPLE)
        # ld64 resolves archive members in any order
        target_link_libraries(turok2 PRIVATE t2runtime turok2_game ${_t2_data} t2placeholders)
    else()
        # t2runtime <-> turok2_game reference each other: one rescanned group
        target_link_libraries(turok2 PRIVATE "$<LINK_GROUP:RESCAN,t2runtime,turok2_game${_t2_data_group}>" t2placeholders)
    endif()

    # Bring-up aid (Linux x86-64): a non-PIE executable keeps the game's statics below 2 GB,
    # and T2Port_Boot puts the heap and game thread stacks there too (MAP_32BIT), so pointers
    # that the not-yet-typed game code passes through s32 survive (LINK.md §5). Not a design:
    # the typing work (64BIT.md) removes the need; macOS cannot do it at all.
    option(T2_LOW_MEMORY "Non-PIE executable + game memory below 2 GB (Linux bring-up aid)" ON)
    if(T2_LOW_MEMORY AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
        target_link_options(turok2 PRIVATE -no-pie)
        target_compile_definitions(turok2 PRIVATE T2_LOW_MEMORY_DEFAULT=1)
    endif()
endif()

# t2port.o2r: the port's own archive (not game data): Fast3D's shaders, which LUS loads as
# resources ("shaders/opengl/default.shader.glsl", ...; without them Fast3D aborts with
# "missing f3d.o2r?"). Other HM ports pack the same directory with Torch (`torch pack port
# x.o2r`); a plain zip is enough here. Later: fonts, port UI assets, gamecontrollerdb.
set(T2_PORT_O2R ${CMAKE_BINARY_DIR}/t2port.o2r)
file(GLOB_RECURSE _t2_shaders CONFIGURE_DEPENDS ${T2_LUS_DIR}/src/fast/shaders/*)
add_custom_command(OUTPUT ${T2_PORT_O2R}
    COMMAND ${CMAKE_COMMAND} -E tar cf ${T2_PORT_O2R} --format=zip --mtime=1980-01-01 shaders
    WORKING_DIRECTORY ${T2_LUS_DIR}/src/fast
    DEPENDS ${_t2_shaders}
    COMMENT "Packing t2port.o2r (Fast3D shaders)")
add_custom_target(t2port_o2r ALL DEPENDS ${T2_PORT_O2R})
add_dependencies(turok2 t2port_o2r)
target_compile_definitions(turok2 PRIVATE T2_PORT_O2R_DEFAULT="${T2_PORT_O2R}")
