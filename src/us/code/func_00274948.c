/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0025C6F4.c, func_0025C6F4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/sound.h"

SndEnv *func_002717A8(SndMgr *m, s16 id);
s32 func_002718E0(SndMgr *m, s16 id);
s16 func_00271E20(CVector3 *pos, SndMgr *m);
f32 func_0026D210(u16 v);
void func_00274948(SndVoice *v, SoundEffectInfo *info) {
    f32 d;
    s16 id;
    v->vp.pan = 0x40;
    id = info->unkC;
    if (id != -1) {
        v->env3 = *func_002717A8(v->mgr, id);
        d = func_0026D210(v->env3.unk2);
        do { v->env3Step = (d - func_0026D210(v->env3.target)) / (f32)v->env3.frames;
        v->env3Value = func_0026D210(v->env3.unk2);
        v->vp.pan = func_0026D210(v->env3.unk2);
        v->hasEnv = 1; return; } while (0);
    } else {
        v->hasEnv = 0;
        if (v->soundId < 0x100) {
            if (v->reset == 0 && func_002718E0(v->mgr, id) != 0) {
                v->vp.pan = (s8)v->mgr->cueVolume;
            }
        } else if (v->reset == 0 && func_002718E0(v->mgr, id) != 0) {
            v->vp.pan = func_00271E20(&v->vp.pos, (SndMgr *)v->mgr->listener);
        }
    }
}
