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
extern BossState D_8012F9D8;

void func_004078B8(CGameObjectInstance *obj, CAI *ai) {
    Targets t;

    D_800F1F50[0] |= 0x8000;
    func_00246108(obj, ai, &t, 1, 1);
    D_800F1F50[0] &= ~0x8000;
    func_00245BAC(obj, ai, 0.261799425f, t.angle);
    if (ai->m_AnimRepeatedCount == 0) {
        return;
    }
    D_8012F9D8.index = -1;
    obj->m_AI.m_AnimSpeed = 1.0f;
    if (D_8012F9D8.state == 6 || t.dist <= 460.8f) {
        D_8012F9D8.mode = 6;
        func_00243414(obj, ai, 1);
    } else if (D_8012F9D8.state == 2) {
        func_00243414(obj, ai, 7);
    }
}
