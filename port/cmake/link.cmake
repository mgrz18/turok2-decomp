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
    # the sound world (Rage Wars views of CAudioWorld/SndVoice/SndQueue at N64 offsets):
    # port/audio/t2audio_world.c (docs/port/AUDIO.md §8)
    func_002706A0.c   # CAudioWorld__StartVoice
    func_002708C0.c   # CAudioWorld__StopHandle
    func_00270A60.c   # CAudioWorld__PlayLayer
    func_00270C80.c   # CAudioWorld__PlaySound
    func_00271128.c   # CAudioWorld__PlaySoundIndex
    func_002712E8.c   # CAudioWorld__Update
    func_00271570.c   # CAudioWorld__StopSound
    func_002715B0.c   # CAudioWorld__StopSoundsByOwner
    func_002715F0.c   # CAudioWorld__StopSoundsByFlags
    func_00271630.c   # CAudioWorld__PlaySoundPitched
    func_00271694.c   # CAudioWorld__UpdateAmbientTrack
    func_002717A8.c   # CAudioWorld__GetPanEnvelope
    func_002718A0.c   # CAudioWorld__FadeOutMusic
    func_002718D0.c   # CAudioWorld__SetAmbientTrackEnabled
    func_002718E0.c   # CAudioWorld__GetStereo
    func_002718F4.c   # CAudioWorld__GetSndPlayer
    func_002719F4.c   # CAudioWorld__StopMusic
    func_00271A10.c   # CAudioWorld__SetFxMix
    func_00271A70.c   # CAudioWorld__DuckMusic
    func_00271A8C.c   # CAudioWorld__UnduckMusic
    func_00271AA8.c   # CAudioWorld__DetachAllVoices
    func_00271AC4.c   # CAudioWorld__PlaySpeech
    func_00271B04.c   # CAudioWorld__StopSpeech
    func_00271B20.c   # CAudioWorld__SortInstrumentSounds
    func_00271BEC.c   # CAudioWorld__PickVariant
    func_00271D84.c   # CAudioWorld__GetVariantLayers
    func_00271DB4.c   # CAudioWorld__IsSoundBusy
    func_00271E20.c   # Sound_CalcPan
    func_00271F6C.c   # SndVoice_InitPitch
    func_00272278.c   # SndVoice_InitVolume
    func_00272B54.c   # SndVoice_UpdatePan
    func_0027314C.c   # SndVoice_Update
    func_00273370.c   # SndVoicePool_FindSlot
    func_00273504.c   # SndVoicePool_AllocVoice
    func_00273B90.c   # SndVoicePool_GetVoiceState
    func_00273BF8.c   # SndVoicePool_PlayVoice
    func_00273C5C.c   # SndVoicePool_StopGroupVoice
    func_00273D88.c   # SndVoicePool_IsSoundPlaying
    func_00273DD8.c   # SndVoicePool_StopVoice
    func_00273E98.c   # SndVoice_SetPosition
    func_00273EB8.c   # SndVoice_SetSoundId
    func_00273EC0.c   # SndVoice_Setup
    func_0027403C.c   # SndVoicePool_Update
    func_002740E8.c   # SndVoicePool_FindSoundWithOwner
    func_002746AC.c   # SndVoice_SetVolume
    func_00274728.c   # SndVoice_SetPitch
    func_00274798.c   # SndVoice_SetPan
    func_002747EC.c   # SndVoice_GetState
    func_00274830.c   # SndVoice_Play
    func_00274850.c   # SndVoice_Stop
    func_002748D0.c   # SndVoice_Release
    func_00274948.c   # SndVoice_InitPan
    func_00274A98.c   # SndVoicePool_ReleaseVoice
    func_00274B40.c   # SndSeq_Update
    func_00275018.c   # SndSeq_UpdateFade
    func_00275100.c   # SndSeq_SelectSong
    func_00275224.c   # SndSeq_Duck
    func_00275234.c   # SndSeq_Unduck
    func_00275248.c   # SndSeq_Stop
    func_00275270.c   # SndSeq_GetSongData
    func_00275324.c   # SndSeq_SetVolume
    func_00275378.c   # SndSeq_UpdateVolume
    func_00275450.c   # SndSeq_Load
    func_002757E4.c   # Sound__IsPlaying
    func_00275BE8.c   # LoopSounds_Construct
    func_00275C6C.c   # LoopSounds_Start
    func_00275D34.c   # LoopSounds_Stop
    func_00275DC0.c   # LoopSounds_Pause
    func_00275E1C.c   # LoopSounds_Resume
    func_00275E98.c   # LoopSounds_Update
    func_00275EFC.c   # LoopSounds_StopAll
    func_00275FA0.c   # SndQueue_Add
    func_00276180.c   # SndQueue_Update
    func_002763AC.c   # SndQueue_InitNode
    func_002764C4.c   # SndQueue_RequeueVoice
    func_002768D0.c   # SpeechPlayer_Update
    func_00276B10.c   # SpeechPlayer_Play
    func_00276D98.c   # SpeechPlayer_Construct
    func_00276DFC.c   # SpeechPlayer_Stop
    func_00276E78.c   # SpeechPlayer_PickEntry
    func_00277024.c   # SpeechPlayer_StopUnlocked
    func_00293E30.c   # alSndpSetPriority (port/audio/t2audio_sndp.c)
    # libaudio's synthesizer driver, voice calls and event helpers (views at N64 offsets):
    # port/audio/t2audio_lib.c
    func_00293F10.c   # alSynAddPlayer
    func_00294C70.c   # __nextSampleTime
    func_00294D18.c   # _timeToSamples
    func_00294C30.c   # _timeToSamplesNoRound
    func_00294BDC.c   # _collectPVoices
    func_00294B9C.c   # _freePVoice
    func_00294090.c   # _allocatePVoice
    func_00294160.c   # alSynFreeVoice
    func_00294200.c   # alSynSetFXMix
    func_002942B0.c   # alSynSetPan
    func_00294340.c   # alSynSetPitch
    func_002943D0.c   # alSynSetPriority
    func_002943E0.c   # alSynSetVol
    func_002944A0.c   # alSynStartVoice
    func_00294650.c   # alSynStopVoice
    func_00291140.c   # alEvtqFlush
    func_002911B8.c   # alEvtqFlushType
    func_00293BA8.c   # _removeEvents
    func_0028FA00.c   # alCSPGetState
    func_00296340.c   # alFilterNew
    func_002954F8.c   # alSaveNew
    func_002973E0.c   # alSaveParam
    func_00290E90.c   # alCSPSetSeq
    func_00290E10.c   # alCSPSetBank
    func_00290EC0.c   # alCSPSetVol
    func_00290E40.c   # alCSPSetChlFXMix
    func_00290DE0.c   # alCSPPlay
    func_00290EF0.c   # alCSPStop
    func_00296690.c   # alResampleParam
    func_0026FFA0.c   # AudioDMA_CacheBlock
    # the scheduler, typed for the host (port/game/t2sched.c: its views overran on LP64)
    func_0028A020.c   # __scMain
    func_0028A17C.c   # __scHandleRetrace
    func_0028A43C.c   # __scHandleRSP
    func_0028A914.c   # __scHandleRDP
    func_0028ABE0.c   # __scSchedule
    func_0028ADF0.c   # osCreateScheduler
    func_0028AF00.c   # osScAddClient
    func_0028AF58.c   # osScRemoveClient
    func_0028AFE8.c   # osScGetCmdQ
    func_0028AFF0.c   # __scAppendList
    func_0028B048.c   # __scTaskReady
    func_0028B0A8.c   # __scTaskComplete
    func_0028B0FC.c   # __scExec
    func_0028B274.c   # __scYield
    func_0028B2AC.c   # Sched_HandleCmdMsg
    func_0028B3E0.c   # Sched_GetInterruptQ
    func_0028B3E8.c   # Sched_SubmitTask
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
