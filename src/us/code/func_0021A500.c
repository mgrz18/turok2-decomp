/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00246FF4.c, func_00246FF4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "globals/engine.h"

typedef struct B10X B10X;
typedef struct B10Q4 B10Q4;
typedef struct B10Pq B10Pq;
typedef struct B10Ow B10Ow;
typedef struct B10Mt B10Mt;
typedef struct B10B24 B10B24;
typedef struct B10An B10An;
typedef struct B10Obj B10Obj;
typedef struct B10H B10H;
typedef struct B10Pt B10Pt;
typedef struct B10Tri B10Tri;
typedef struct B10Fl B10Fl;
typedef struct B10Z B10Z;
typedef struct B10C B10C;
struct B10X {
    u8 pad0[0x68];
                f32 m68[16];
    u8 padA8[0xC];
                s32** pB4;
};
struct B10Q4 {
                f32 x;
                f32 y;
                f32 z;
                f32 w;
};
struct B10Pq {
    u8 pad0[0x140];
                B10Q4 q140;
};
struct B10Ow {
    u8 pad0[0x5DC];
                B10Pq* p5DC;
};
struct B10Mt {
                f32 m[16];
};
struct B10B24 {
                s32 w[6];
};
struct B10An {
    u8 pad0[0x4];
                s16 h4;
    u8 pad6[0x4];
                s8 b122;
                s8 b123;
    u8 padC[0x8];
};
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
struct B10Pt {
                f32 x;
    u8 pad4[0x4];
                f32 z;
                f32 y;
};
struct B10Tri {
    u8 pad0[0x4];
                B10Pt* pt[3];
};
struct B10Fl {
                u32 flags;
                s8 b4;
    u8 pad5[0x1];
                s8 b6;
    u8 pad7[0x11];
                f32 f18;
};
struct B10Z {
    u8 pad0[0xC];
                B10C* pC;
};
struct B10C {
    u8 pad0[0x8];
                f32 f8;
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
