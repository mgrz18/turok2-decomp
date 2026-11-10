/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00246FF4.c, func_00246FF4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "globals/engine.h"
#ifdef TARGET_PORT
#include "structs/game_object.h"
#endif
typedef struct B10Obj B10Obj;
typedef struct B10H B10H;
#ifdef TARGET_PORT
struct B10Obj {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_B10Obj(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_p14[T2_HOFF_B10Obj(header.m_pCurrentRegion)]; B10H* p14; }; /* N64 0x10 */
        struct { u8 _hp_f5C[T2_HOFF_B10Obj(m_qGround.x)]; f32 f5C; }; /* N64 0x40 */
        struct { u8 _hp_flags100[T2_HOFF_B10Obj(m_dwFlags)]; u32 flags100; }; /* N64 0xD4 */
    };
};
#else
struct B10Obj { /* Turok 2 layout (Rage Wars: p14, f5C, flags100) */
    u8 pad0[0x10];
    B10H* p14;          /* 0x10 */
    u8 pad14[0x2C];
    f32 f5C;            /* 0x40: quaternion */
    u8 pad44[0x90];
    u32 flags100;       /* 0xD4 */
};
#endif
struct B10H {
    u8 pad0[0x2];
                u16 h2;
};
extern f32 D_800A5AF8;
s32 GameObject_IsOnGround();
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar) */
#define GameObject_GetGroundRotation GameObject_GetGroundRotation_sret
#endif
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
