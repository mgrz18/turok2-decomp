/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B7520_5.c, func_802B2780_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

s32 func_00293C50(s32 val, f32 div);
extern const f32 D_800AA484;
extern void func_002936E0(struct ALSndPlayer * arg0, union ALSndpEvent * arg1);
extern void func_00291034(ALEventQueue *, ALEvent *, s32);
extern s32 alSynAllocVoice(ALSynth_func_802B3000_de *, ALVoice_s *, ALVoiceConfig_s *);
extern void func_002944A0(ALSynth_func_802B3000_de *, ALVoice_s *, void *);
extern void func_002942B0(ALSynth_func_802B3000_de *, ALVoice_s *, u8);
extern void alSynSetVol(ALSynth_func_802B3000_de *, ALVoice_s *, s16, s32);
extern void func_00294340(ALSynth_func_802B3000_de *, ALVoice_s *, f32);
extern void func_00294200(ALSynth_func_802B3000_de *, ALVoice_s *, u8);
extern void alSynStopVoice(ALSynth_func_802B3000_de *, ALVoice_s *);
extern void func_00294160(ALSynth_func_802B3000_de *, ALVoice_s *);
extern void func_00293BA8(ALEventQueue *, ALSoundState *);
void func_002936E0(ALSndPlayer *sndp, ALSndpEvent *event)
{
    ALVoiceConfig_s vc;
    ALSound_s *snd;
    ALVoice_s *voice;
    u8 pan;
    f32 pitch;
    ALSndpEvent evt;
    s32 delta;
    s16 vol;
    s16 tmp;
    s32 vtmp;
    ALSoundState *state;
    const ALSndpEvent_func_802B2780_de *event_view =
        (const ALSndpEvent_func_802B2780_de *)event;
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
            tmp = state->pan - 64 + ((const ALSound_s_func_802B2780_de *)snd)->samplePan;
            tmp = ((tmp) > (0) ? (tmp) : (0));
            pan = (u8)((tmp) < (127) ? (tmp) : (127));
            pitch = state->pitch;
            delta = snd->envelope->attackTime;
            func_002944A0(sndp->drvr, voice, snd->wavetable);
            state->state = 1;
            func_002942B0(sndp->drvr, voice, pan);
            alSynSetVol(sndp->drvr, voice, vol, delta);
            func_00294340(sndp->drvr, voice, pitch);
            func_00294200(sndp->drvr, voice, state->fxMix);
            evt.common.type = 6;
            evt.common.state = state;
            delta = (s32)func_00293C50(snd->envelope->attackTime, state->pitch);
            func_00291034(&sndp->evtq, (ALEvent *)&evt, delta);
            break;
        case 1:
            if (state->state != 1 || !snd)
                return;
            delta = (s32)func_00293C50(snd->envelope->releaseTime, state->pitch);
            alSynSetVol(sndp->drvr, &state->voice, 0, delta);
            if (delta) {
                evt.common.type = 7;
                evt.common.state = state;
                func_00291034(&sndp->evtq, (ALEvent *)&evt, delta);
                state->state = 2;
            } else {
                alSynStopVoice(sndp->drvr, &state->voice);
                func_00294160(sndp->drvr, &state->voice);
                func_00293BA8(&sndp->evtq, state);
                state->state = 0;
            }
            break;
        case 2:
            state->pan = event_view->pan.pan;
            if (state->state == 1 && snd) {
                tmp = state->pan - 64 + ((const ALSound_s_func_802B2780_de *)snd)->samplePan;
                tmp = ((tmp) > (0) ? (tmp) : (0));
                pan = (u8)((tmp) < (127) ? (tmp) : (127));
                func_002942B0(sndp->drvr, &state->voice, pan);
            }
            break;
        case 4:
            {
                f32 min = D_800AA484;
                if ((state->pitch = event_view->pitch.pitch) < min)
                    state->pitch = min;
            }
            if (state->state == 1) {
                func_00294340(sndp->drvr, &state->voice, state->pitch);
            }
            break;
        case 8:
            state->fxMix = event_view->fx.mix;
            if (state->state == 1)
                func_00294200(sndp->drvr, &state->voice, state->fxMix);
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
                delta = (s32)func_00293C50(snd->envelope->decayTime, state->pitch);
                alSynSetVol(sndp->drvr, &state->voice, (s16)vtmp, delta);
                evt.common.type = 1;
                evt.common.state = state;
                func_00291034(&sndp->evtq, (ALEvent *)&evt, delta);
            }
            break;
        case 7:
            alSynStopVoice(sndp->drvr, &state->voice);
            func_00294160(sndp->drvr, &state->voice);
            func_00293BA8(&sndp->evtq, state);
            state->state = 0;
            break;
        default:
            break;
    }
}
