/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B7520_5.c, func_802B2780_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

s32 _DivS32ByF32(s32 val, f32 div);
extern void _handleEvent(ALSndPlayer * arg0, ALSndpEvent * arg1);
extern void alEvtqPostEvent(ALEventQueue *, ALEvent *, s32);
extern s32 alSynAllocVoice(ALSynth *, ALVoice *, ALVoiceConfig *);
extern void alSynStartVoice(ALSynth *, ALVoice *, void *);
extern void alSynSetPan(ALSynth *, ALVoice *, u8);
extern void alSynSetVol(ALSynth *, ALVoice *, s16, s32);
extern void alSynSetPitch(ALSynth *, ALVoice *, f32);
extern void alSynSetFXMix(ALSynth *, ALVoice *, u8);
extern void alSynStopVoice(ALSynth *, ALVoice *);
extern void alSynFreeVoice(ALSynth *, ALVoice *);
extern void _removeEvents(ALEventQueue *, ALSoundState *);
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
