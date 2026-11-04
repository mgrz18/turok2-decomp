#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ CGameObjectInstance *obj;
    /* 0x08 */ u8 pad08[0x88 - 0x08];
} Targets;

extern void func_00246108(CGameObjectInstance *obj, CAI *ai, Targets *t, s32 f1, s32 f2);

extern s32 D_800F1F50[];
extern CVector3 *D_800C18F0[];  /* boss paths */
extern s32 D_8012F600;          /* point on the path */
extern s32 D_8012FB48;          /* path */

void func_004074D0(CGameObjectInstance *obj, CAI *ai) {
    Targets t;

    D_800F1F50[0] |= 0x8000;
    func_00246108(obj, ai, &t, 0, 1);
    D_800F1F50[0] &= ~0x8000;
    obj->m_AI.m_AnimSpeed = 1.0f;
    obj->m_dwFlags |= 0x2000;
    if (D_8012F608.bossMode == 11) {
        D_8012F608.bossState = 11;
    }
    if (D_8012F608.bossMode == 4) {
        if (D_8012F608.stage != 1 && D_8012F608.bossState != 4) {
            D_8012FB48 = 0;
            D_8012F600 = 0;
            D_8012F608.dest = D_800C18F0[0][0];
        }
        D_8012F608.bossState = 4;
        D_8012F608.unk3CC = 3;
    }
    if (D_8012F608.bossMode == 3) {
        D_8012F608.bossState = 3;
        D_8012F608.unk3CC = 3;
    }
}
