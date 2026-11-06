/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00246FF4.c, func_00246FF4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "globals/engine.h"
typedef struct B10Obj B10Obj;
typedef struct B10H B10H;
struct B10Obj { /* Turok 2 layout (Rage Wars: p14, f5C, flags100) */
    u8 pad0[0x10];
    B10H* p14;          /* 0x10 */
    u8 pad14[0x2C];
    f32 f5C;            /* 0x40: quaternion */
    u8 pad44[0x90];
    u32 flags100;       /* 0xD4 */
};
struct B10H {
    u8 pad0[0x2];
                u16 h2;
};
extern f32 D_800A5AF8;
s32 GameObject_IsOnGround();
void GameObject_GetGroundRotation(f32 *loc, B10Obj *o);
s32 func_002682A4(B10Obj *o);
void CQuaternion__TurnToward(f32 *a, f32 *loc);
void CQuaternion__SlerpFast(f32 *a, f32 t, f32 *b, f32 *loc);
void func_0021A500(B10Obj *o) {
    f32 loc[4];
    u32 fl;
    u32 mask;
    s32 r;
    s32 s;
    register f32 t __asm__("$f0");
    f32 k;
    fl = o->flags100;
    mask = fl & 0x300000;
    if (fl & 1) {
        return;
    }
    r = GameObject_IsOnGround();
    if (o->p14 == 0) {
        return;
    }
    GameObject_GetGroundRotation(loc, o);
    s = o->p14->h2 & 1;
    if (func_002682A4(o) == 0) {
        return;
    }
    if (s) {
        loc[0] = loc[1] = loc[2] = 0.0f;
        loc[3] = 1.0f;
    }
    if (mask) {
        if ((!(o->flags100 & 0x1000) && r) || s) {
            CQuaternion__TurnToward(&o->f5C, loc);
        }
    } else {
        if (r && !(o->flags100 & 0x1000)) {
            t = gFrameIncrement;
            t *= 0.125f;
        } else {
            t = gFrameIncrement;
            t *= 0.0375f;
        }
        CQuaternion__SlerpFast(&o->f5C, (D_800A5AF8 < t) ? D_800A5AF8 : t, &o->f5C, loc);
    }
}
