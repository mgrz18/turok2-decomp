/* NON-MATCHING: 52 of 112 words differ: GCC folds 10.0f * 256.0f (the ROM multiplies at run time) and keeps t.obj in t0 instead of v1. */
#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ CGameObjectInstance *obj;
    /* 0x08 */ f32 angle;
    /* 0x0C */ CVector3 a;
    /* 0x18 */ CVector3 b;
    /* 0x24 */ f32 dist;
    /* 0x28 */ u8 pad28[0x88 - 0x28];
} Targets;

/* An effect's parameters, passed by value. */
typedef struct {
    /* 0x0 */ s16 upper;
    /* 0x2 */ s16 lower;
    /* 0x4 */ u8 b[4];
} __attribute__((aligned(4))) EffectParams;

extern void func_00246108(CGameObjectInstance *obj, CAI *ai, Targets *t, s32 f1, s32 f2);
extern void func_00236EA0(CGameObjectInstance *src, CGameObjectInstance *dst, s32 effect, s32 id,
                          CVector3 pos, EffectParams params);

extern s32 D_800F1F50[];
extern s32 D_8012F988;          /* parts left (mode 1) */
extern s32 D_8012F990;          /* parts left (mode 2) */

void func_00406C4C(s32 arg0, CGameObjectInstance *obj, s32 arg2, CVector3 pos, s32 event) {
    Targets t;
    EffectParams params;
    f32 scale;

    D_800F1F50[0] |= 0x8000;
    func_00246108(obj, &obj->m_AI, &t, 1, 1);
    D_800F1F50[0] &= ~0x8000;
    if (t.obj == NULL) {
        return;
    }
    if (event == 2 && t.dist <= 409.6f && D_8012F990 > 0) {
        params.upper = 0xA00;
        params.lower = 0x400;
        params.b[0] = 0;
        params.b[1] = 0;
        params.b[2] = 0;
        params.b[3] = 0;
        func_00236EA0(obj, obj, 3, -1, t.obj->header.m_vPos, params);
    }
    if (event == 1 && t.dist <= 716.8f && D_8012F988 > 0) {
        scale = 256.0f;
        params.upper = 10.0f * scale;
        params.lower = 0x800;
        params.b[0] = 0;
        params.b[1] = 0;
        params.b[2] = 0;
        params.b[3] = 0;
        func_00236EA0(obj, obj, 3, -1, t.obj->header.m_vPos, params);
    }
}
