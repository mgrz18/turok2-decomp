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

/* One step of a boss attack script (0x14 bytes). */
typedef struct {
    /* 0x00 */ u8 type;
    /* 0x04 */ s32 arg;
    /* 0x08 */ f32 animSpeed;
    /* 0x0C */ s32 chance;
    /* 0x10 */ f32 delay;
} BossStep;

/* Boss module state at 0x8012F9D8. */
typedef struct {
    /* 0x00 */ s32 state;
    /* 0x04 */ s32 mode;
    /* 0x08 */ u8 pad08;
    /* 0x09 */ u8 scriptOn;
    /* 0x0A */ u8 pad0A[0x2];
    /* 0x0C */ f32 timer;
    /* 0x10 */ u8 pad10[0x1C];
    /* 0x2C */ s32 index;
} BossState;

extern void func_00246108(CGameObjectInstance *obj, CAI *ai, Targets *t, s32 f1, s32 f2);
extern void func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);
extern void func_00245BAC(CGameObjectInstance *obj, CAI *ai, f32 turnRate, f32 angle);
extern f32 func_002119FC(f32 lo, f32 hi);
extern s32 func_00404C44(CGameObjectInstance *obj, CAI *ai, f32 dist);

extern s32 D_800F1F50[];
extern BossState D_8012F9D8;
extern f32 D_8012F9F8;         /* cooldown */
extern BossStep *D_8012F9FC;   /* script */
extern s32 D_8012FA00;         /* cursor */

void func_00405E04(CGameObjectInstance *obj, CAI *ai) {
    Targets t;

    D_800F1F50[0] |= 0x8000;
    func_00246108(obj, ai, &t, 1, 1);
    D_800F1F50[0] &= ~0x8000;

    if (t.obj == NULL) {
        obj->m_AI.m_AnimSpeed = 1.0f;
        D_8012F9D8.mode = 10;
        func_00243414(obj, ai, 1);
        return;
    }
    func_00245BAC(obj, ai, 0.261799425f, t.angle);
    if (!D_8012F9D8.scriptOn) {
        return;
    }
    if (D_8012F9D8.index == -1) {
        if (D_8012FA00 != -1) {
            D_8012F9F8 = D_8012F9FC[D_8012FA00].delay;
        } else {
            D_8012F9F8 = 0.0f;
        }
        do {
            D_8012FA00++;
            if (D_8012F9FC[D_8012FA00].type == 5) {
                D_8012FA00 = 0;
            }
            D_8012F9D8.index = D_8012FA00;
        } while ((f32) D_8012F9FC[D_8012FA00].chance <= func_002119FC(0.0f, 100.0f));
    }
    if (func_00404C44(obj, ai, t.dist)) {
        return;
    }
    if (t.dist <= 460.8f) {
        obj->m_AI.m_AnimSpeed = 1.0f;
        func_00243414(obj, ai, 1);
    }
}
