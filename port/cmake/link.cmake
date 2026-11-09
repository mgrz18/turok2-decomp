# What of the game C goes into the linked executable (docs/port/LINK.md).
#
# game.cmake compiles every C file; this file sorts out the ones that must NOT be linked:
#   - module 2 (.virtual_1, the dead second VM module: RUNTIME.md §1.4): func_0043CC14 ..
#     func_00469374 and the four *_2.c files, 629 files;
#   - the game's libultra C that drives N64 hardware or the libultra kernel, replaced by
#     port/runtime (t2os.c: threads/queues/events; t2ultra.c: VI/SP/DP/AI/PI/SI/timers/caches;
#     t2boot.c: boot, heap, VM, sprintf). Their callers reach the t2_ names through
#     port/include/t2_rename.h.
# Those files go to turok2_game_unlinked (built only to keep the "every file compiles" check,
# T2_BUILD_UNLINKED) instead of turok2_game.
#
# The 9 files of T2_GAME_EXCLUDE (game.cmake: libgcc, CP0, SN64 varargs, host I/O) are not
# compiled at all; their functions are the host compiler's (libgcc, libm) or port/runtime's.

set(T2_LINK_REPLACED
    # threads, queues, events (t2os.c)
    func_0029B060.c   # osCreateThread
    func_0029BB10.c   # osStartThread
    func_0029B9B0.c   # osSetThreadPri
    func_0029B150.c   # osGetThreadPri
    func_0029DF70.c   # osYieldThread
    func_0029DE00.c   # osDestroyThread
    func_0029B030.c   # osCreateMesgQueue
    func_0029B820.c   # osSendMesg
    func_0029B5B0.c   # osJamMesg
    func_0029B6F0.c   # osRecvMesg
    func_0029B950.c   # osSetEventMesg
    func_0029AA20.c   # osViSetEvent
    # time (t2ultra.c: virtual clock)
    func_0029BA80.c   # osSetTimer
    func_0029B170.c   # osGetTime
    # VI
    func_0029A720.c   # osCreateViManager
    func_0029A580.c   # osViBlack
    func_0029A6E0.c   # osViGetCurrentFramebuffer
    func_0029AA80.c   # osViSetMode
    func_0029AAD0.c   # osViSetSpecialFeatures
    func_0029AC40.c   # osViSetYScale
    func_0029AC90.c   # osViSwapBuffer
    # SP / DP
    func_0029A248.c   # osSpTaskLoad
    func_0029A34C.c   # osSpTaskStartGo
    func_0029A4F0.c   # osSpTaskYield
    func_0029A510.c   # osSpTaskYielded
    func_0029A380.c   # __osSpDeviceBusy
    func_0029A560.c   # __osSpGetStatus
    func_0029A4D0.c   # __osSpSetStatus
    func_00298D50.c   # osDpSetNextBuffer
    func_00298DF0.c   # __osDpDeviceBusy
    # AI
    func_00298480.c   # osAiGetStatus
    func_002984A0.c   # osAiSetFrequency
    func_002985C0.c   # osAiSetNextBuffer
    func_00298650.c   # __osAiDeviceBusy
    # PI
    func_0029D5D0.c   # osCartRomInit
    func_0029D450.c   # osCreatePiManager
    func_0029DA30.c   # osEPiRawReadIo
    func_0029D190.c   # osEPiRawStartDma
    func_0029DBD0.c   # osEPiRawWriteIo
    func_002A3060.c   # osPiGetCmdQueue
    func_0029B410.c   # osPiRawReadIo
    func_00299EB0.c   # osPiRawStartDma
    func_002A2FB0.c   # osPiStartDma
    # SI: controllers, rumble
    func_00298A40.c   # osContInit
    func_002986F8.c   # osContGetQuery
    func_002987B4.c   # osContGetReadData
    func_00298900.c   # osContReset
    func_00298670.c   # osContStartQuery
    func_00298720.c   # osContStartReadData
    func_0029A080.c   # __osSiRawStartDma
    func_0029A140.c   # __osSiDeviceBusy
    func_00299198.c   # osMotorInit
    func_00298FD4.c   # osMotorStart
    func_00298E10.c   # osMotorStop
    # system, TLB, 64DD
    func_0029B1F0.c   # osInitialize
    func_0029BF80.c   # osVirtualToPhysical
    func_0028E5D0.c   # osMapTLBRdb
    func_0029D370.c   # osLeoDiskInit
    # engine boot and VM (t2boot.c)
    func_00202130.c   # InitializeSystemResources (heap = host arena)
    func_0028D820.c   # VM_Initialize (no pager)
    func_0028E60C.c   # VM_Alloc (resident copy)
    func_0028E6DC.c   # VM_Free
    # audio (docs/port/AUDIO.md): Sound__Initialize in t2boot.c (only with --audio); the SDK
    # audio manager in port/audio/t2audio_mgr.c (frames through the ABI1 interpreter, resident
    # wave data instead of the loader's DMA windows)
    func_00275520.c   # Sound__Initialize
    func_0026F760.c   # amCreateAudioMgr
    func_0026FB70.c   # __amMain
    func_0026FD4C.c   # __amDmaNew
    func_0026FD88.c   # __amHandleFrameMsg
    func_002700D0.c   # __amDMA
    # the sound player API (m2c views with N64 offsets): port/audio/t2audio_sndp.c
    func_00293470.c   # alSndpAllocate
    func_00293530.c   # alSndpDeallocate
    func_00293580.c   # alSndpGetState
    func_00293CE0.c   # alSndpPlay
    func_00293D40.c   # alSndpSetFXMix
    func_00293D90.c   # alSndpSetPan
    func_00293DE0.c   # alSndpSetPitch
    func_00293E60.c   # alSndpSetSound
    func_00293E70.c   # alSndpSetVol
    func_00293EC0.c   # alSndpStop
)

# True if NAME (a src/us/code file name) is module 2 (.virtual_1).
function(t2_is_module2 name out)
    set(r FALSE)
    if(name MATCHES "_2\\.c$")
        set(r TRUE)
    elseif(name MATCHES "^func_00([0-9A-F]+)\\.c$")
        math(EXPR addr "0x${CMAKE_MATCH_1}")
        math(EXPR lo "0x43CC14")
        math(EXPR hi "0x46A000")
        if(addr GREATER_EQUAL lo AND addr LESS hi)
            set(r TRUE)
        endif()
    endif()
    set(${out} ${r} PARENT_SCOPE)
endfunction()
