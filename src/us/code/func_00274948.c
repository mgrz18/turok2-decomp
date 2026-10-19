/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0025C6F4.c, func_0025C6F4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

SndEnv *func_002717A8(SndMgr *m, s16 id);
s32 func_002718E0(SndMgr *m, s16 id);
s16 func_00271E20(Vec3f *pos, SndMgr *m);
f32 func_0026D210(u16 v);
void func_00274948(SndVoice *v, SndInfo *info) {
    f32 d;
    s16 id;
    v->pan = 0x40;
    id = info->wC;
    if (id != -1) {
        v->env = *func_002717A8(v->mgr, id);
        d = func_0026D210(v->env.a);
        do { v->unk9C = (d - func_0026D210(v->env.b)) / (f32)v->env.c;
        v->unk98 = func_0026D210(v->env.a);
        v->pan = func_0026D210(v->env.a);
        v->unk88 = 1; return; } while (0);
    } else {
        v->unk88 = 0;
        if (v->tag < 0x100) {
            if (v->unkC0 == 0 && func_002718E0(v->mgr, id) != 0) {
                v->pan = (s8)v->mgr->unk2B94;
            }
        } else if (v->unkC0 == 0 && func_002718E0(v->mgr, id) != 0) {
            v->pan = func_00271E20(&v->pos, (SndMgr *)v->mgr->unk2B98);
        }
    }
}
