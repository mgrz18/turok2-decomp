# turok2: the host executable (skeleton). See docs/port/LIBULTRASHIP.md.
add_library(t2asset STATIC
    ${T2_PORT_DIR}/assets/t2asset.c
    ${T2_PORT_DIR}/third_party/rnc/rnc.c
    ${T2_PORT_DIR}/third_party/miniz/miniz.c)
target_include_directories(t2asset PUBLIC ${T2_PORT_DIR}/assets)
# miniz must not export zlib names: LUS links libzip, which links the system zlib.
target_compile_definitions(t2asset PRIVATE MINIZ_NO_ZLIB_COMPATIBLE_NAMES _LARGEFILE64_SOURCE)
set_source_files_properties(${T2_PORT_DIR}/third_party/rnc/rnc.c ${T2_PORT_DIR}/third_party/miniz/miniz.c
    PROPERTIES COMPILE_OPTIONS "-w")

add_executable(turok2 main.cpp t2asset_lus.cpp testdl.c)
set_target_properties(turok2 PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON C_STANDARD 11)
# The game's GBI family: F3DEX2 (F3DEX.NoN fifo 2.05 + L3DEX 2.05, include/globals/ucode.h).
# Must match libultraship's GBI_UCODE so macros encode what the interpreter decodes.
target_compile_definitions(turok2 PRIVATE ${GBI_UCODE})
target_link_libraries(turok2 PRIVATE libultraship t2asset)

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
