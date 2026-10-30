/* NON-MATCHING: 5 of 46 words differ: GCC schedules li a2,7 first; the ROM has it in the jal delay slot (same as func_00405AC8). */
#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ CGameObjectInstance *obj;
    /* 0x08 */ f32 angle;
    /* 0x0C */ u8 pad0C[0x88 - 0x0C];
} Targets;

extern void func_00246108(CGameObjectInstance *obj, CAI *ai, Targets *t, s32 f1, s32 f2);
extern void func_00245BAC(CGameObjectInstance *obj, CAI *ai, f32 turnRate, f32 angle);
extern void func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);

extern s32 D_800F1F50[];
extern s32 D_8012FA04;          /* boss script index */

void func_00407AC8(CGameObjectInstance *obj, CAI *ai) {
    Targets t;

    D_800F1F50[0] |= 0x8000;
    func_00246108(obj, ai, &t, 1, 1);
    D_800F1F50[0] &= ~0x8000;
    func_00245BAC(obj, ai, 0.261799425f, t.angle);
    if (ai->m_AnimRepeatedCount != 0) {
        obj->m_AI.m_AnimSpeed = 1.0f;
        D_8012FA04 = -1;
        func_00243414(obj, ai, 7);
    }
}
