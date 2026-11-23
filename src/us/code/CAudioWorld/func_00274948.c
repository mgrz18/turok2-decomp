/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0025C6F4.c, func_0025C6F4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/sound.h"

SndEnv *CAudioWorld__GetPanEnvelope(CAudioWorld *m, s16 id);
s32 CAudioWorld__GetStereo(CAudioWorld *m, s16 id);
s16 Sound_CalcPan(CVector3 *pos, CAudioWorld *m);
f32 HalfToFloat(u16 v);
void SndVoice_InitPan(SndVoice *v, SoundEffectInfo *info) {
    f32 d;
    s16 id;
    v->vp.pan = 0x40;
    id = info->unkC;
    if (id != -1) {
        v->env3 = *CAudioWorld__GetPanEnvelope(v->mgr, id);
        d = HalfToFloat(v->env3.unk2);
        do { v->env3Step = (d - HalfToFloat(v->env3.target)) / (f32)v->env3.frames;
        v->env3Value = HalfToFloat(v->env3.unk2);
        v->vp.pan = HalfToFloat(v->env3.unk2);
        v->hasEnv = 1; return; } while (0);
    } else {
        v->hasEnv = 0;
        if (v->soundId < 0x100) {
            if (v->reset == 0 && CAudioWorld__GetStereo(v->mgr, id) != 0) {
                v->vp.pan = (s8)v->mgr->cueVolume;
            }
        } else if (v->reset == 0 && CAudioWorld__GetStereo(v->mgr, id) != 0) {
            v->vp.pan = Sound_CalcPan(&v->vp.pos, (CAudioWorld *)v->mgr->listener);
        }
    }
}
