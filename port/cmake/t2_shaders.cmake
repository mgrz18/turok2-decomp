# t2_shaders.cmake (cmake -P, round 18 g4): stages LUS's Fast3D shaders for t2port.o2r.
#   -DSRC=<LUS>/src/fast/shaders -DDST=<build>/t2port_shaders/shaders
# The Metal shader is patched: LUS declares texture, sampler and int parameters in the `thread`
# address space ("thread const texture2d<float> tex"), which the current Metal compiler rejects
# ("parameter may not be qualified with an address space": macOS 27, Apple M5). The library then
# fails to compile and the Metal renderer aborts at its first draw (docs/port/MACOS.md 20.1).
# Handles and scalars are passed by value: the fix only drops the qualifier.
file(REMOVE_RECURSE ${DST})
file(COPY ${SRC}/ DESTINATION ${DST})
set(_m ${DST}/metal/default.shader.metal)
if(EXISTS ${_m})
    file(READ ${_m} _s)
    string(REPLACE "thread const texture2d<float> " "texture2d<float> " _s "${_s}")
    string(REPLACE "thread const sampler " "sampler " _s "${_s}")
    string(REPLACE "thread const int " "const int " _s "${_s}")
    file(WRITE ${_m} "${_s}")
endif()
