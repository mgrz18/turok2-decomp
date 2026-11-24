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
    # (The libultra modules the N64 build compiles from lib/ultralib keep only the copies the
    # port still uses, in src/port_libultra: versions/ultralib_objects.us.txt, docs/LIBULTRA.md.
    # Only the files that still exist are listed.)
    # threads, queues, events (t2os.c)
    func_0029B060.c   # osCreateThread
    func_0029DE00.c   # osDestroyThread
    # time (t2ultra.c: virtual clock)
    # VI
    vimgr.c           # osCreateViManager, viMgrMain (the merged module)
    # SP / DP
    sptask.c          # osSpTaskLoad, osSpTaskStartGo, _VirtualToPhysicalTask (the merged module)
    # AI
    func_002985C0.c   # osAiSetNextBuffer
    # PI
    func_0029D5D0.c   # osCartRomInit
    func_0029D450.c   # osCreatePiManager
    func_002A3060.c   # osPiGetCmdQueue
    func_0029B410.c   # osPiRawReadIo
    func_002A2FB0.c   # osPiStartDma
    # SI: controllers, rumble
    # (osContInit: under #ifndef TARGET_PORT in the merged controller.c)
    func_0029A080.c   # __osSiRawStartDma
    # system, TLB, 64DD
    func_0029B1F0.c   # osInitialize
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
    # (Sound__Initialize: under #ifndef TARGET_PORT in the merged Sound.c)
    # (amCreateAudioMgr: under #ifndef TARGET_PORT in the merged AudioMgr.c)
    # (__amMain: under #ifndef TARGET_PORT in the merged AudioMgr.c)
    # (__amDmaNew: under #ifndef TARGET_PORT in the merged AudioMgr.c)
    # (__amHandleFrameMsg: under #ifndef TARGET_PORT in the merged AudioMgr.c)
    # (__amDMA: under #ifndef TARGET_PORT in the merged AudioMgr.c)
    # the sound player API (m2c views with N64 offsets): port/audio/t2audio_sndp.c
    # the sound world (Rage Wars views of CAudioWorld/SndVoice/SndQueue at N64 offsets):
    # port/audio/t2audio_world.c (docs/port/AUDIO.md §8)
    # (CAudioWorld__StartVoice: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (CAudioWorld__StopHandle: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (CAudioWorld__PlayLayer: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (CAudioWorld__PlaySound: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (CAudioWorld__PlaySoundIndex: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (CAudioWorld__Update: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (CAudioWorld__StopSound: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (CAudioWorld__StopSoundsByOwner: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (CAudioWorld__StopSoundsByFlags: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (CAudioWorld__PlaySoundPitched: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (CAudioWorld__UpdateAmbientTrack: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (CAudioWorld__GetPanEnvelope: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (CAudioWorld__FadeOutMusic: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (CAudioWorld__SetAmbientTrackEnabled: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (CAudioWorld__GetStereo: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (CAudioWorld__GetSndPlayer: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (CAudioWorld__StopMusic: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (CAudioWorld__SetFxMix: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (CAudioWorld__DuckMusic: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (CAudioWorld__UnduckMusic: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (CAudioWorld__DetachAllVoices: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (CAudioWorld__PlaySpeech: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (CAudioWorld__StopSpeech: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (CAudioWorld__SortInstrumentSounds: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (CAudioWorld__PickVariant: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (CAudioWorld__GetVariantLayers: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (CAudioWorld__IsSoundBusy: under #ifndef TARGET_PORT in the merged CAudioWorld.c)
    # (Sound_CalcPan: under #ifndef TARGET_PORT in the merged SndVoice.c)
    # (SndVoice_InitPitch: under #ifndef TARGET_PORT in the merged SndVoice.c)
    # (SndVoice_InitVolume: under #ifndef TARGET_PORT in the merged SndVoice.c)
    # (SndVoice_UpdatePan: under #ifndef TARGET_PORT in the merged SndVoice.c)
    # (SndVoice_Update: under #ifndef TARGET_PORT in the merged SndVoice.c)
    # (SndVoicePool_FindSlot: under #ifndef TARGET_PORT in the merged SndVoice.c)
    # (SndVoicePool_AllocVoice: under #ifndef TARGET_PORT in the merged SndVoice.c)
    # (SndVoicePool_GetVoiceState: under #ifndef TARGET_PORT in the merged SndVoice.c)
    # (SndVoicePool_PlayVoice: under #ifndef TARGET_PORT in the merged SndVoice.c)
    # (SndVoicePool_StopGroupVoice: under #ifndef TARGET_PORT in the merged SndVoice.c)
    # (SndVoicePool_IsSoundPlaying: under #ifndef TARGET_PORT in the merged SndVoice.c)
    # (SndVoicePool_StopVoice: under #ifndef TARGET_PORT in the merged SndVoice.c)
    # (SndVoice_SetPosition: under #ifndef TARGET_PORT in the merged SndVoice.c)
    # (SndVoice_SetSoundId: under #ifndef TARGET_PORT in the merged SndVoice.c)
    # (SndVoice_Setup: under #ifndef TARGET_PORT in the merged SndVoice.c)
    # (SndVoicePool_Update: under #ifndef TARGET_PORT in the merged SndVoice.c)
    # (SndVoicePool_FindSoundWithOwner: under #ifndef TARGET_PORT in the merged SndVoice.c)
    # (SndVoice_SetVolume: under #ifndef TARGET_PORT in the merged SndVoice.c)
    # (SndVoice_SetPitch: under #ifndef TARGET_PORT in the merged SndVoice.c)
    # (SndVoice_SetPan: under #ifndef TARGET_PORT in the merged SndVoice.c)
    # (SndVoice_GetState: under #ifndef TARGET_PORT in the merged SndVoice.c)
    # (SndVoice_Play: under #ifndef TARGET_PORT in the merged SndVoice.c)
    # (SndVoice_Stop: under #ifndef TARGET_PORT in the merged SndVoice.c)
    # (SndVoice_Release: under #ifndef TARGET_PORT in the merged SndVoice.c)
    # (SndVoice_InitPan: under #ifndef TARGET_PORT in the merged SndVoice.c)
    # (SndVoicePool_ReleaseVoice: under #ifndef TARGET_PORT in the merged SndVoice.c)
    # (SndSeq_Update: under #ifndef TARGET_PORT in the merged SndSeq.c)
    # (SndSeq_UpdateFade: under #ifndef TARGET_PORT in the merged SndSeq.c)
    # (SndSeq_SelectSong: under #ifndef TARGET_PORT in the merged SndSeq.c)
    # (SndSeq_Duck: under #ifndef TARGET_PORT in the merged SndSeq.c)
    # (SndSeq_Unduck: under #ifndef TARGET_PORT in the merged SndSeq.c)
    # (SndSeq_Stop: under #ifndef TARGET_PORT in the merged SndSeq.c)
    # (SndSeq_GetSongData: under #ifndef TARGET_PORT in the merged SndSeq.c)
    # (SndSeq_SetVolume: under #ifndef TARGET_PORT in the merged SndSeq.c)
    # (SndSeq_UpdateVolume: under #ifndef TARGET_PORT in the merged SndSeq.c)
    # (SndSeq_Load: under #ifndef TARGET_PORT in the merged SndSeq.c)
    # (Sound__IsPlaying: under #ifndef TARGET_PORT in the merged Sound.c)
    # (LoopSounds_Construct .. LoopSounds_StopAll: under #ifndef TARGET_PORT in the merged LoopSounds.c)
    # (SndQueue_Add: under #ifndef TARGET_PORT in the merged SndQueue.c)
    # (SndQueue_Update: under #ifndef TARGET_PORT in the merged SndQueue.c)
    # (SndQueue_InitNode: under #ifndef TARGET_PORT in the merged SndQueue.c)
    # (SndQueue_RequeueVoice: under #ifndef TARGET_PORT in the merged SndQueue.c)
    # (SpeechPlayer_Update .. SpeechPlayer_StopUnlocked, all but SpeechPlayer_ReadHeader: under #ifndef TARGET_PORT in the merged SpeechPlayer.c)
    # libaudio's synthesizer driver, voice calls and event helpers (views at N64 offsets):
    # port/audio/t2audio_lib.c
    # (_freePVoice, _collectPVoices, _timeToSamples, __nextSampleTime, _timeToSamplesNoRound: under
    # #ifndef TARGET_PORT in the merged synthesizer.c)
    func_00294200.c   # alSynSetFXMix
    # (alSaveNew: under #ifndef TARGET_PORT in the merged drvrnew.c)
    # (AudioDMA_CacheBlock: under #ifndef TARGET_PORT in the merged AudioMgr.c)
    # libultra's compressed sequence player, cseq.c and the shared seqplayer.c helpers (Rage Wars
    # views at N64 offsets, s32 pointers): port/audio/t2audio_csp.c (docs/port/AUDIO.md §9)
    # (__getTrackByte: under #ifndef TARGET_PORT in the merged cseq.c)
    # (__readVarLen: under #ifndef TARGET_PORT in the merged cseq.c)
    func_0028F518.c   # alCSeqNew
    func_0028EF90.c   # __alCSeqGetTrackEvent
    func_0028F61C.c   # alCSeqNextEvent
    func_0028F910.c   # __alCSeqNextDelta
    func_0028F71C.c   # alCSeqTicksToSec
    func_0028F784.c   # alCSeqSecToTicks
    func_0028F710.c   # alCSeqGetTicks
    func_0028F1F0.c   # alCSeqNewMarker
    func_0028F828.c   # alCSeqSetLoc
    func_0028F89C.c   # alCSeqGetLoc
    # the adec speech decoder and its loader pull (views at N64 offsets, the frame pointer in an
    # s32): port/audio/t2audio_adec.c (docs/port/AUDIO.md §10)
    func_002A1170.c   # adecStreamOpen
    func_002A122C.c   # adecNextFrame
    func_002A12CC.c   # adecGetStopFlag
    func_002A12DC.c   # adecClearStopFlag
    # the scheduler, typed for the host (port/game/t2sched.c: its views overran on LP64)
    # (__scMain .. Sched_SubmitTask: all 17 functions of the merged __scHandleRSP.c, round 28)
    __scHandleRSP.c
)

# True if NAME (a game C file name: src/us/vm2/<module>/func_*.c) is module 2 (.virtual_1).
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
