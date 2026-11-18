/* t2audio_sndp.c: the sound player's API calls for the host (docs/port/AUDIO.md §5).
 *
 * The game's alSndp* entry points (libultra sndplayer.c, imported from the Rage Wars decomps)
 * reach ALSndPlayer and ALSoundState through m2c views with N64 offsets and 0x30-byte
 * strides (PoolRecAt44, SlotRecAt2F, ObjRecAt40V2 in rw/abu_types.h, local Pool/Slot types) and
 * post events whose state pointer is an s32. On the host those files are left out of the link
 * (port/cmake/link.cmake) and these typed versions stand in, with the same behaviour and the
 * same event numbers as the game's (0 play, 1 stop, 2 pan, 3 volume, 4 pitch, 8 fx mix;
 * _handleEvent, func_002936E0, is the game's own and typed). To be retired when the 64-bit
 * typing reaches these files (64BIT.md). */
#include "common.h"
#include "structs/libaudio.h"
#include <stdio.h>
#include <stdlib.h>

extern u32 gFrameCountGameplay;

void alEvtqPostEvent(ALEventQueue *evtq, ALEvent *evt, ALMicroTime delta);

static ALSoundState *target_state(ALSndPlayer *sndp) {
    return &((ALSoundState *)sndp->sndState)[sndp->target];
}

/* (round 18 merge) The game selects sounds the player never allocated: voice 16 of the sound world
 * has no player sound (SndVoicePool_AllocVoice), yet stopping it (SndVoicePool_StopByOwner,
 * voice_stop in t2audio_world.c) selects handles[16], which is the u16 counter after the handle
 * array (0xFC: the number of sounds allocated so far, ~20,000 late in a game), and
 * alSndpDeallocate leaves the target at -1. The N64 then reads (and may post a stop to) a state
 * far outside sndState; on the host that read crashed (SIGSEGV in alSndpGetState, a whole-game run
 * at ~201,900). Here a target outside 0..maxSounds-1 reads as stopped and posts nothing.
 *
 * (round 19 h2, n64trace.py --sndp) What the N64 reads: sndState[target].state, the word at
 * sndState + target * 0x30 + 0x28 (func_00293580; the target an s32 sign-extended from the s16
 * handle). sndState is 0x807740D0 on the N64 (in the audio heap, a cache block allocated at boot).
 * - target -1 (a voice of the stopped group whose handle was already freed; 140 calls in a
 *   whole default run, at level loads through StopSoundsByFlags): the word 8 bytes before the
 *   array, the end of the audio heap block allocated before it. It held 0 on every call traced
 *   (emulator frames 600-1,500: a level, its sounds and a load): AL_STOPPED, no stop posted.
 * - voice 16 (handles[16] = the u16 count of sounds allocated): a count of 11,942..32,767 puts
 *   the address past the end of the 8 MB of RDRAM (0x80800000), where the read gives 0 (the
 *   emulator; no memory there), so again stopped. The default run's four cases had counts
 *   18,400-21,728. Only a count below 11,942 (early in a game) or above 32,767 (negative) would
 *   read game memory on the N64 (and post a stop to it when that word is not 0); the host cannot
 *   model that memory and reads it as stopped as well.
 * So the guard gives the N64's result in every case a whole game reaches. */
static int target_ok(ALSndPlayer *sndp) {
    return sndp->target >= 0 && sndp->target < sndp->maxSounds;
}

static void post(ALSndPlayer *sndp, s16 type, ALSndpEvent *e) {
    if (!target_ok(sndp)) {
        return;
    }
    e->common.type = type;
    e->common.state = target_state(sndp);
    alEvtqPostEvent(&sndp->evtq, &e->msg, 0);
}

/* func_00293470 */
ALSndId alSndpAllocate(ALSndPlayer *sndp, ALSound *sound) {
    ALSoundState *st = (ALSoundState *)sndp->sndState;
    s16 i;
    for (i = 0; i < sndp->maxSounds; i++) {
        ALSoundState *s = &st[i];
        if (s->sound == NULL) {
            s->sound = sound;
            s->priority = 5;
            s->state = 0; /* AL_STOPPED */
            s->pitch = 1.0f;
            s->pan = 0x40;
            s->fxMix = 0;
            s->vol = (s16)((sound->sampleVolume * 0x7FFF) / 127);
            return i;
        }
    }
    return -1;
}

/* func_00293530 */
void alSndpDeallocate(ALSndPlayer *sndp, ALSndId id) {
    ALSoundState *s = &((ALSoundState *)sndp->sndState)[id];
    if (s->state == 0) {
        s->sound = NULL;
        if (sndp->target == id)
            sndp->target = -1;
    }
}

/* func_00293580 */
s32 alSndpGetState(ALSndPlayer *sndp) {
    static int sLog = -1;
    if (sLog < 0)
        sLog = getenv("T2_SNDP_LOG") != NULL;
    if (sLog && !target_ok(sndp))
        fprintf(stderr, "sndp: gf %u target %d max %d (out of range: read as stopped)\n",
                gFrameCountGameplay, (int)sndp->target, (int)sndp->maxSounds);
    return target_ok(sndp) ? target_state(sndp)->state : 0; /* AL_STOPPED */
}

/* func_00293E60 */
void alSndpSetSound(ALSndPlayer *sndp, ALSndId id) {
    sndp->target = id;
}

/* func_00293CE0 */
void alSndpPlay(ALSndPlayer *sndp) {
    ALSndpEvent e;
    if (target_ok(sndp) && target_state(sndp)->state == 0)
        post(sndp, 0, &e);
}

/* func_00293EC0 */
void alSndpStop(ALSndPlayer *sndp) {
    ALSndpEvent e;
    post(sndp, 1, &e);
}

/* func_00293D90 */
void alSndpSetPan(ALSndPlayer *sndp, ALPan pan) {
    ALSndpEvent e;
    e.pan.pan = pan;
    post(sndp, 2, &e);
}

/* func_00293E70 */
void alSndpSetVol(ALSndPlayer *sndp, s16 vol) {
    ALSndpEvent e;
    e.vol.vol = vol;
    post(sndp, 3, &e);
}

/* func_00293DE0 (the game passes the pitch ratio as a float) */
void alSndpSetPitch(ALSndPlayer *sndp, f32 pitch) {
    ALSndpEvent e;
    e.pitch.pitch = pitch;
    post(sndp, 4, &e);
}

/* func_00293D40 */
void alSndpSetFXMix(ALSndPlayer *sndp, u8 mix) {
    ALSndpEvent e;
    e.fx.mix = mix;
    post(sndp, 8, &e);
}

/* func_00293E30 (SeqPlayer view: the state array at 0x40, 0x30-byte states, priority at 0x20) */
void alSndpSetPriority(ALSndPlayer *sndp, ALSndId id, u8 priority) {
    ((ALSoundState *)sndp->sndState)[id].priority = priority;
}
