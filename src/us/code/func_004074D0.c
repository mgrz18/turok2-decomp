#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern void AI_UpdateTargets(CGameObjectInstance *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);

extern s32 D_800F1F50[];
extern CVector3 *D_800C18F0[];  /* boss paths */
extern s32 D_8012F600;          /* point on the path */
extern s32 D_8012FB48;          /* path */

void Primagen_EnterMode19(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;

    D_800F1F50[0] |= 0x8000;
    AI_UpdateTargets(obj, ai, &t, 0, 1);
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
