/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0025EA3C.c, func_0025EA3C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

extern const f32 D_800A58F4;
void *CISet__GetBlock(void *t, s32 i);
void CQuaternion__Slerp(f32 *out, f32 t, void *a, void *b);
void CAnimationState__GetBoneRotation(R05Anim *a, s32 idx, f32 time, f32 *out)
{
    void *root;
    void *blk;
    R05CurveCfg *cfg;
    f32 scale;
    f32 n;
    f32 frac;
    s16 sel;
    void *keys;
    u32 ua;
    s32 i0;
    s32 i1;
    u8 *tab;
    u8 *p;
    s32 *v2;
    s32 off;
    u32 ub;
    root = *a->data;
    blk = CISet__GetBlock(root, 5);
    cfg = CISet__GetBlock(blk, 0);
    tab = CISet__GetBlock(root, 1);
    scale = cfg->scale;
    sel = *(s16 *)(tab + (idx << 2) + 0xA);
    n = cfg->n;
    time = time * scale;
    if (sel == -1) {
        v2 = CISet__GetBlock(root, 2);
        off = idx * *v2 + 8;
        p = (u8 *)v2 + off;
        out[0] = (f32)*(s16 *)(p + 0xC) * D_800A58F4;
        out[1] = (f32)*(s16 *)(p + 0xE) * D_800A58F4;
        out[2] = (f32)*(s16 *)(p + 0x10) * D_800A58F4;
        out[3] = (f32)*(s16 *)(p + 0x12) * D_800A58F4;
        return;
    }
    keys = CISet__GetBlock(CISet__GetBlock(blk, 2), sel);
    if (a->loopStart == 0xFFFF) {
        i0 = (((time) >= 0.0f) ? (s32)(time) : (((f32)(s32)(time) == (time)) ? (s32)(time) : (s32)(time) - 1));
        if (n <= (f32)i0) {
            i0 = 0;
        }
        i1 = i0 + 1;
        if (n <= (f32)i1) {
            i1 = 0;
        }
    } else {
        i0 = (((time) >= 0.0f) ? (s32)(time) : (((f32)(s32)(time) == (time)) ? (s32)(time) : (s32)(time) - 1));
        if (n - 1.0f <= (f32)i0) {
            ua = a->loopStart;
            i0 = ((((f32)ua * scale) <= 0.0f) ? (s32)((f32)ua * scale) : ({ s32 _c = ((((f32)(s32)((f32)ua * scale)) == ((f32)ua * scale)) ? 0 : 1); _c + (s32)((f32)ua * scale); }));
        }
        i1 = i0 + 1;
        if (n - 1.0f <= (f32)i1) {
            ub = a->loopStart;
            i1 = ((((f32)ub * scale) <= 0.0f) ? (s32)((f32)ub * scale) : ({ s32 _c = ((((f32)(s32)((f32)ub * scale)) == ((f32)ub * scale)) ? 0 : 1); _c + (s32)((f32)ub * scale); }));
        }
    }
    frac = time - (f32)(((time) >= 0.0f) ? (s32)(time) : (((f32)(s32)(time) == (time)) ? (s32)(time) : (s32)(time) - 1));
    CQuaternion__Slerp(out, frac, (u8 *)keys + i0 * 16, (u8 *)keys + i1 * 16);
}
