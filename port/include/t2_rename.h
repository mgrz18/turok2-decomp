/* t2_rename.h: host names for the libultra and libc functions the port implements itself
 * (docs/port/LINK.md). Included by port_shim.h (so every game file sees it) and by the host
 * runtime files under port/runtime/ that define the t2_ names. Never seen by the N64 build.
 *
 * Why rename: libultraship's static library defines its own osSendMesg, osRecvMesg,
 * osCreateMesgQueue, osContInit, osViSwapBuffer, osSetTimer, ... (non-blocking stubs, see
 * docs/port/RUNTIME.md §2.4) and the host libc defines memset, strlen, sprintf, ... with
 * 64-bit size_t. The game's calls go to t2_<name> instead, so none of those can clash or
 * silently interpose: t2os (port/runtime/t2os.c) and the device shims
 * (port/runtime/t2ultra.c) define the t2_ names, and the game's own C copies of the libc
 * routines (memset, strlen, ...: SN64 libc, N64 size_t) are compiled under the t2_ names too.
 * Port code that wants libultraship's or libc's real function simply does not include this
 * header (port/app/*.cpp).
 *
 * Only functions are renamed. libultra data (osMemSize, osTvType, __osContPifRam, ...) keep
 * their names: they are game data (q2's generator / the placeholders), and LUS's data objects
 * are only pulled in by LUS functions the game no longer references. */
#ifndef T2_RENAME_H
#define T2_RENAME_H

/* threads, queues, events, interrupts: t2os */
#define osCreateThread t2_osCreateThread
#define osStartThread t2_osStartThread
#define osStopThread t2_osStopThread
#define osDestroyThread t2_osDestroyThread
#define osYieldThread t2_osYieldThread
#define osSetThreadPri t2_osSetThreadPri
#define osGetThreadPri t2_osGetThreadPri
#define osGetThreadId t2_osGetThreadId
#define osCreateMesgQueue t2_osCreateMesgQueue
#define osSendMesg t2_osSendMesg
#define osJamMesg t2_osJamMesg
#define osRecvMesg t2_osRecvMesg
#define osSetEventMesg t2_osSetEventMesg
#define osViSetEvent t2_osViSetEvent
#define __osDisableInt t2___osDisableInt
#define __osRestoreInt t2___osRestoreInt

/* devices: port/runtime/t2ultra.c */
#define osInitialize t2_osInitialize
#define osGetTime t2_osGetTime
#define osGetCount t2_osGetCount
#define osSetTime t2_osSetTime
#define osSetTimer t2_osSetTimer
#define osStopTimer t2_osStopTimer
#define osCreateViManager t2_osCreateViManager
#define osViBlack t2_osViBlack
#define osViGetCurrentFramebuffer t2_osViGetCurrentFramebuffer
#define osViGetNextFramebuffer t2_osViGetNextFramebuffer
#define osViSetMode t2_osViSetMode
#define osViSetSpecialFeatures t2_osViSetSpecialFeatures
#define osViSetXScale t2_osViSetXScale
#define osViSetYScale t2_osViSetYScale
#define osViSwapBuffer t2_osViSwapBuffer
#define osSpTaskLoad t2_osSpTaskLoad
#define osSpTaskStartGo t2_osSpTaskStartGo
#define osSpTaskYield t2_osSpTaskYield
#define osSpTaskYielded t2_osSpTaskYielded
#define osDpSetNextBuffer t2_osDpSetNextBuffer
#define osAiGetStatus t2_osAiGetStatus
#define osAiGetLength t2_osAiGetLength
#define osAiSetFrequency t2_osAiSetFrequency
#define osAiSetNextBuffer t2_osAiSetNextBuffer
#define osCartRomInit t2_osCartRomInit
#define osCreatePiManager t2_osCreatePiManager
#define osPiStartDma t2_osPiStartDma
#define osPiRawStartDma t2_osPiRawStartDma
#define osPiRawReadIo t2_osPiRawReadIo
#define osPiGetCmdQueue t2_osPiGetCmdQueue
#define osEPiStartDma t2_osEPiStartDma
#define osEPiRawStartDma t2_osEPiRawStartDma
#define osEPiRawReadIo t2_osEPiRawReadIo
#define osEPiRawWriteIo t2_osEPiRawWriteIo
#define osContInit t2_osContInit
#define osContReset t2_osContReset
#define osContStartQuery t2_osContStartQuery
#define osContGetQuery t2_osContGetQuery
#define osContStartReadData t2_osContStartReadData
#define osContGetReadData t2_osContGetReadData
#define osMotorInit t2_osMotorInit
#define osMotorStart t2_osMotorStart
#define osMotorStop t2_osMotorStop
#define osVirtualToPhysical t2_osVirtualToPhysical
#define osMapTLB t2_osMapTLB
#define osMapTLBRdb t2_osMapTLBRdb
#define osUnmapTLBAll t2_osUnmapTLBAll
#define osSetIntMask t2_osSetIntMask
#define osInvalDCache t2_osInvalDCache
#define osInvalICache t2_osInvalICache
#define osWritebackDCache t2_osWritebackDCache
#define osWritebackDCacheAll t2_osWritebackDCacheAll
#define osLeoDiskInit t2_osLeoDiskInit
#define __osSiRawStartDma t2___osSiRawStartDma
#define __osSiDeviceBusy t2___osSiDeviceBusy
#define __osSpDeviceBusy t2___osSpDeviceBusy
#define __osSpGetStatus t2___osSpGetStatus
#define __osSpSetStatus t2___osSpSetStatus
#define __osDpDeviceBusy t2___osDpDeviceBusy
#define __osAiDeviceBusy t2___osAiDeviceBusy
#define __osGetFpcCsr t2___osGetFpcCsr
#define __osSetFpcCsr t2___osSetFpcCsr
#define __osGetSR t2___osGetSR
#define __osSetSR t2___osSetSR
#define __osProbeTLB t2___osProbeTLB
#define __osUnmapTLBAll t2___osUnmapTLBAll
#define __osSetCompare t2___osSetCompare

/* libc: the game's own SN64 copies (N64 size_t), or a host wrapper (sprintf). Host runtime files
 * that call the host libc define T2_RENAME_NO_LIBC. */
#ifndef T2_RENAME_NO_LIBC
#define memset t2_memset
#define memmove t2_memmove
#define memcmp t2_memcmp
#define strlen t2_strlen
#define strchr t2_strchr
#define lldiv t2_lldiv
#define __assert t2___assert
#define sprintf t2_sprintf
#endif

#endif
