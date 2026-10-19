/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B8370_5.c, func_802B35E0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern Node_func_80239AF4_de *func_00294B58(void);
extern s32 func_00294C30(ALSynth_func_802B3000_de *, s32);
void func_00294540(ALSynth_func_802B3000_de *s, ALVoice_s_func_802B3000_de *v, void *w, f32 pitch, s16 vol, u8 pan, s8 fxmix, s32 t)
{
    ALStartParamAlt *update;
    ALFilter_s_func_802B3000_de *f;
    if (v->pvoice) {
        update = (ALStartParamAlt *)func_00294B58();
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
        update->samples = func_00294C30(s, t);
        update->wave = w;
        f = v->pvoice->channelKnob;
        (*f->setParam)(f, 3, update);
    }
}
