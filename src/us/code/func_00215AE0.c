/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0025E5D0.c, func_0025E5D0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

void *func_002017D4(void *t, s32 i);
void func_00215AE0(R05Anim *a, s32 idx, f32 time, f32 *out)
{
    void *root;
    void *blk;
    R05CurveCfg *cfg;
    f32 scale;
    f32 n;
    f32 frac;
    f32 w;
    s16 sel;
    f32 *t0;
    u32 ua;
    s32 i0;
    s32 i1;
    s32 k;
    s32 off;
    R05Vec3i *src;
    s32 *v2;
    u8 *tab;
    f32 *dst;
    root = *a->data;
    blk = func_002017D4(root, 5);
    cfg = func_002017D4(blk, 0);
    tab = func_002017D4(root, 1);
    scale = cfg->scale;
    sel = *(s16 *)(tab + (idx << 2) + 8);
    n = cfg->n;
    time = time * scale;
    if (sel == -1) {
        v2 = func_002017D4(root, 2);
        off = idx * *v2 + 8;
        *(R05Vec3i *)out = *(R05Vec3i *)((u8 *)v2 + off);
        return;
    }
    t0 = func_002017D4(func_002017D4(blk, 1), sel);
    frac = time - (f32)(((time) >= 0.0f) ? (s32)(time) : (((f32)(s32)(time) == (time)) ? (s32)(time) : (s32)(time) - 1));
    ua = a->loopStart;
    dst = out;
    if (ua == 0xFFFF) {
        ua = 0;
    }
    i0 = (((time) >= 0.0f) ? (s32)(time) : (((f32)(s32)(time) == (time)) ? (s32)(time) : (s32)(time) - 1));
    if (n <= (f32)i0) {
        i0 = ((((f32)ua * scale) <= 0.0f) ? (s32)((f32)ua * scale) : ({ i0 = ((((f32)(s32)((f32)ua * scale)) == ((f32)ua * scale)) ? 0 : 1); i0 + (s32)((f32)ua * scale); }));
    }
    i1 = i0 + 1;
    if (n <= (f32)i1) {
        i1 = ((((f32)ua * scale) <= 0.0f) ? (s32)((f32)ua * scale) : ({ i1 = ((((f32)(s32)((f32)ua * scale)) == ((f32)ua * scale)) ? 0 : 1); i1 + (s32)((f32)ua * scale); }));
    }
    k = 0;
    w = 1.0f - frac;
    do {
        f32 r = (1.0f - frac) * t0[i0 * 4 + k] + frac * t0[i1 * 4 + k];
        k++;
        *dst = r;
        dst++;
    } while (k < 3);
}
