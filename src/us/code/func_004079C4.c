#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ CGameObjectInstance *obj;
    /* 0x08 */ f32 angle;
    /* 0x0C */ u8 pad0C[0x88 - 0x0C];
} Targets;

/* Boss module state at 0x8012F9D8. */
typedef struct {
    /* 0x00 */ s32 state;
    /* 0x04 */ s32 mode;
    /* 0x08 */ u8 pad08[0x24];
    /* 0x2C */ s32 index;           /* 0x8012FA04 */
} BossState;

extern void func_00246108(CGameObjectInstance *obj, CAI *ai, Targets *t, s32 f1, s32 f2);
extern void func_00245BAC(CGameObjectInstance *obj, CAI *ai, f32 turnRate, f32 angle);
extern void func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);

extern s32 D_800F1F50[];
extern f32 D_8012F628;          /* last angle to the target */
extern BossState D_8012F9D8;

void func_004079C4(CGameObjectInstance *obj, CAI *ai) {
    Targets t;

    D_800F1F50[0] |= 0x8000;
    func_00246108(obj, ai, &t, 1, 1);
    D_800F1F50[0] &= ~0x8000;
    if (t.obj != NULL) {
        func_00245BAC(obj, ai, 0.261799425f, t.angle);
        D_8012F628 = t.angle;
    } else {
        func_00245BAC(obj, ai, 0.261799425f, D_8012F628);
    }
    if (ai->m_AnimRepeatedCount != 0) {
        obj->m_AI.m_AnimSpeed = 1.0f;
        D_8012F9D8.index = -1;
        D_8012F9D8.mode = 6;
        func_00243414(obj, ai, 1);
    }
}
