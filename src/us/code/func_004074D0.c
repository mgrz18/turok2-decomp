#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern void AI_UpdateTargets(CGameObjectInstance *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);

extern s32 D_800F1F50[];
extern CVector3 *gPrimagenPaths[];  /* boss paths */
extern s32 sPrimagenPathPoint;          /* point on the path */
extern s32 gLevelScript_path;          /* path */

void Primagen_EnterMode19(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;

    D_800F1F50[0] |= 0x8000;
    AI_UpdateTargets(obj, ai, &t, 0, 1);
    D_800F1F50[0] &= ~0x8000;
    obj->m_AI.m_AnimSpeed = 1.0f;
    obj->m_dwFlags |= 0x2000;
    if (gLevelScript.bossMode == 11) {
        gLevelScript.bossState = 11;
    }
    if (gLevelScript.bossMode == 4) {
        if (gLevelScript.stage != 1 && gLevelScript.bossState != 4) {
            gLevelScript_path = 0;
            sPrimagenPathPoint = 0;
            gLevelScript.dest = gPrimagenPaths[0][0];
        }
        gLevelScript.bossState = 4;
        gLevelScript.unk3CC = 3;
    }
    if (gLevelScript.bossMode == 3) {
        gLevelScript.bossState = 3;
        gLevelScript.unk3CC = 3;
    }
}
