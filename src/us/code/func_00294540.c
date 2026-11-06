/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B8370_5.c, func_802B35E0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern Node_func_80239AF4_de *__allocParam(void);
extern s32 _timeToSamplesNoRound(ALSynth *, s32);
void alSynStartVoiceParams(ALSynth *s, ALVoice *v, void *w, f32 pitch, s16 vol, u8 pan, s8 fxmix, s32 t)
{
    ALStartParamAlt *update;
    ALFilter *f;
    if (v->pvoice) {
        update = (ALStartParamAlt *)__allocParam();
        if (update == 0) { return; };
        if (fxmix < 0) {
            fxmix = -fxmix;
        }
        update->delta = s->paramSamples + v->pvoice->offset;
        update->next = 0;
        update->type = 13;
        update->unity = v->unityPitch;
        update->pan = pan;
        update->volume = vol;
        update->fxMix = fxmix;
        update->pitch = pitch;
        update->samples = _timeToSamplesNoRound(s, t);
        update->wave = w;
        f = v->pvoice->channelKnob;
        (*f->setParam)(f, 3, update);
    }
}
