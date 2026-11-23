/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B7520_5.c, func_802B2650_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libaudio.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B7520_5.c, func_802B2780_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B7520_6.c, func_802B2BBC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "common.h"

s32 _sndpVoiceHandler(void *node);
extern void *alHeapDBAlloc(u8 *, s32, ALHeap *, s32, s32);
extern void alEvtqNew(ALEventQueue *, ALEventListItem *, s32);
extern void alSynAddPlayer(ALSynth *, ALPlayer *);
extern void alEvtqPostEvent(ALEventQueue *, ALEvent *, s32);
extern s32 alEvtqNextEvent(ALEventQueue *, ALEvent *);

void alSndpNew(ALSndPlayer *sndp, ALSndpConfig *c)
{
    u8 *ptr;
    ALEvent evt;
    ALSoundState *sState;
    u32 i;
    sndp->maxSounds = c->maxSounds;
    sndp->target = -1;
    sndp->frameTime = 16000;
    sState = (ALSoundState *)alHeapDBAlloc(0, 0, c->heap, 1, c->maxSounds * sizeof(ALSoundState));
    sndp->sndState = sState;
    for (i = 0; i < c->maxSounds; i++)
        sState[i].sound = 0;
    ptr = alHeapDBAlloc(0, 0, c->heap, 1, c->maxEvents * sizeof(ALEventListItem));
    alEvtqNew(&sndp->evtq, (ALEventListItem *)ptr, c->maxEvents);
    sndp->drvr = &alGlobals->drvr;
    sndp->node.next = 0;
    sndp->node.handler = _sndpVoiceHandler;
    sndp->node.clientData = sndp;
    alSynAddPlayer(sndp->drvr, &sndp->node);
    evt.type = 5;
    alEvtqPostEvent(&sndp->evtq, (ALEvent *)&evt, sndp->frameTime);
    sndp->nextDelta = alEvtqNextEvent(&sndp->evtq, &sndp->nextEvent);
}
s32 _DivS32ByF32(s32 arg0, f32 arg1);
void _handleEvent(ALSndPlayer *sndp, ALSndpEvent *event);
extern s32 alSynAllocVoice(ALSynth *, ALVoice *, ALVoiceConfig *);
extern void alSynStartVoice(ALSynth *, ALVoice *, void *);
extern void alSynSetPan(ALSynth *, ALVoice *, u8);
extern void alSynSetVol(ALSynth *, ALVoice *, s16, s32);
extern void alSynSetPitch(ALSynth *, ALVoice *, f32);
extern void alSynSetFXMix(ALSynth *, ALVoice *, u8);
extern void alSynStopVoice(ALSynth *, ALVoice *);
extern void alSynFreeVoice(ALSynth *, ALVoice *);
void _removeEvents(void *arg0, sptr arg1);

void _handleEvent(ALSndPlayer *sndp, ALSndpEvent *event)
{
    ALVoiceConfig vc;
    ALSound *snd;
    ALVoice *voice;
    u8 pan;
    f32 pitch;
    ALSndpEvent evt;
    s32 delta;
    s16 vol;
    s16 tmp;
    s32 vtmp;
    ALSoundState *state;
    const ALSndpEvent *event_view =
        (const ALSndpEvent *)event;
    state = event->common.state;
    snd = state->sound;
    switch (event->msg.type) {
        case 0:
            if (state->state != 0 || !snd)
                return;
            vc.fxBus = 0;
            vc.priority = state->priority;
            vc.unityPitch = 0;
            voice = &state->voice;
            alSynAllocVoice(sndp->drvr, voice, &vc);
            vol = (s16)((s32)snd->envelope->attackVolume * state->vol / 127);
            tmp = state->pan - 64 + ((const ALSound *)snd)->samplePan;
            tmp = ((tmp) > (0) ? (tmp) : (0));
            pan = (u8)((tmp) < (127) ? (tmp) : (127));
            pitch = state->pitch;
            delta = snd->envelope->attackTime;
            alSynStartVoice(sndp->drvr, voice, snd->wavetable);
            state->state = 1;
            alSynSetPan(sndp->drvr, voice, pan);
            alSynSetVol(sndp->drvr, voice, vol, delta);
            alSynSetPitch(sndp->drvr, voice, pitch);
            alSynSetFXMix(sndp->drvr, voice, state->fxMix);
            evt.common.type = 6;
            evt.common.state = state;
            delta = (s32)_DivS32ByF32(snd->envelope->attackTime, state->pitch);
            alEvtqPostEvent(&sndp->evtq, (ALEvent *)&evt, delta);
            break;
        case 1:
            if (state->state != 1 || !snd)
                return;
            delta = (s32)_DivS32ByF32(snd->envelope->releaseTime, state->pitch);
            alSynSetVol(sndp->drvr, &state->voice, 0, delta);
            if (delta) {
                evt.common.type = 7;
                evt.common.state = state;
                alEvtqPostEvent(&sndp->evtq, (ALEvent *)&evt, delta);
                state->state = 2;
            } else {
                alSynStopVoice(sndp->drvr, &state->voice);
                alSynFreeVoice(sndp->drvr, &state->voice);
                _removeEvents(&sndp->evtq, state);
                state->state = 0;
            }
            break;
        case 2:
            state->pan = event_view->pan.pan;
            if (state->state == 1 && snd) {
                tmp = state->pan - 64 + ((const ALSound *)snd)->samplePan;
                tmp = ((tmp) > (0) ? (tmp) : (0));
                pan = (u8)((tmp) < (127) ? (tmp) : (127));
                alSynSetPan(sndp->drvr, &state->voice, pan);
            }
            break;
        case 4:
            {
                f32 min = 0.0001f;
                if ((state->pitch = event_view->pitch.pitch) < min)
                    state->pitch = min;
            }
            if (state->state == 1) {
                alSynSetPitch(sndp->drvr, &state->voice, state->pitch);
            }
            break;
        case 8:
            state->fxMix = event_view->fx.mix;
            if (state->state == 1)
                alSynSetFXMix(sndp->drvr, &state->voice, state->fxMix);
            break;
        case 3:
            state->vol = event_view->vol.vol;
            if (state->state == 1 && snd) {
                vtmp = snd->envelope->decayVolume * state->vol / 127;
                alSynSetVol(sndp->drvr, &state->voice, (s16)vtmp, 1000);
            }
            break;
        case 6:
            if (snd->envelope->decayTime != -1) {
                vtmp = snd->envelope->decayVolume * state->vol / 127;
                delta = (s32)_DivS32ByF32(snd->envelope->decayTime, state->pitch);
                alSynSetVol(sndp->drvr, &state->voice, (s16)vtmp, delta);
                evt.common.type = 1;
                evt.common.state = state;
                alEvtqPostEvent(&sndp->evtq, (ALEvent *)&evt, delta);
            }
            break;
        case 7:
            alSynStopVoice(sndp->drvr, &state->voice);
            alSynFreeVoice(sndp->drvr, &state->voice);
            _removeEvents(&sndp->evtq, state);
            state->state = 0;
            break;
        default:
            break;
    }
}

extern void alEvtqPostEvent(ALEventQueue *, ALEvent *, ALMicroTime);

s32 _sndpVoiceHandler(void *node)
{
    ALSndPlayer *sndp = (ALSndPlayer *)node;
    ALSndpEvent evt;
    ALSndpEvent *next = (ALSndpEvent *)&sndp->nextEvent;
    do {
        switch (sndp->nextEvent.type) {
            case 5:
                evt.common.type = 5;
                alEvtqPostEvent(&sndp->evtq, (ALEvent *)&evt, sndp->frameTime);
                break;
            default:
                _handleEvent(sndp, next);
                break;
        }
        sndp->nextDelta = alEvtqNextEvent(&sndp->evtq, &sndp->nextEvent);
    } while (sndp->nextDelta == 0);
    sndp->curTime += sndp->nextDelta;
    return sndp->nextDelta;
}

#ifndef COMMON_H
#ifdef TARGET_PORT
typedef __INTPTR_TYPE__ sptr;
typedef __UINTPTR_TYPE__ uptr;
#else
typedef signed int sptr;
typedef unsigned int uptr;
#endif
#endif

s32 osSetIntMask(s32);
s32 alUnlink(void *);
s32 alLink(void *, void *);

#ifndef TARGET_PORT /* the port defines _removeEvents in port/audio/t2audio_lib.c */
void _removeEvents(void *arg0, sptr arg1)
{
  long long new_var;
  s32 temp_s3;
  void *temp_s1;
  void *var_s0;
  temp_s3 = osSetIntMask(1);
  var_s0 = *((void **) (((s8 *) arg0) + 8));
  if (var_s0 != 0)
  {
    do
    {
      temp_s1 = *((void **) (((s8 *) var_s0) + 0));
      if ((*((s32 *) (((s8 *) var_s0) + 0x10))) == arg1)
      {
        new_var = 0;
        if (temp_s1 != new_var)
        {
          *((s32 *) (((s8 *) temp_s1) + 8)) = (s32) ((*((s32 *) (((s8 *) temp_s1) + 8))) + (*((s32 *) (((s8 *) var_s0) + 8))));
        }
        alUnlink(var_s0);
        alLink(var_s0, arg0);
      }
      var_s0 = temp_s1;
    }
    while (var_s0 != 0);
  }
  osSetIntMask(temp_s3);
}
#endif /* !TARGET_PORT */

void __assert(void *, void *, s32); /* extern */

s32 _DivS32ByF32(s32 arg0, f32 arg1) {
    f64 temp_f0;
    s32 var_v0;

    if (arg1 == 0.0f) {
        __assert("EX", "audio/sndplayer.c", 0x11D);
    }
    temp_f0 = (f64) ((f32) arg0 / arg1);
    var_v0 = 0x7FFFFFFF;
    if (!(2147483647.0 < temp_f0)) {
        var_v0 = (s32) temp_f0;
    }
    return var_v0;
}
