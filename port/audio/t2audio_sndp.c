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

void alEvtqPostEvent(ALEventQueue *evtq, ALEvent *evt, ALMicroTime delta);

static ALSoundState *target_state(ALSndPlayer *sndp) {
    return &((ALSoundState *)sndp->sndState)[sndp->target];
}

static void post(ALSndPlayer *sndp, s16 type, ALSndpEvent *e) {
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
    return target_state(sndp)->state;
}

/* func_00293E60 */
void alSndpSetSound(ALSndPlayer *sndp, ALSndId id) {
    sndp->target = id;
}

/* func_00293CE0 */
void alSndpPlay(ALSndPlayer *sndp) {
    ALSndpEvent e;
    if (target_state(sndp)->state == 0)
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
