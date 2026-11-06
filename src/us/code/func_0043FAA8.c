/* Second-module twin of Primagen_UpdateMode24 (VM module 2 is linked over module 1; same source, its own strings and pool). */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

extern void CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);

extern s32 D_800F1C40[];
extern s32 D_800F1F50[];
extern BossStep2 sPrimagenSpawnScript1_2[];
extern BossStep2 sPrimagenSpawnScript2_2[];
extern BossStep2 sPrimagenSpawnScript3_2[];

/* Primagen_SetSpawnScript: start the second script for a stage. */
static __inline__ void StartScript2(s32 stage) {
    gLevelScript.script2.cursor = -1;
    gLevelScript.script2On = 1;
    gLevelScript.script2.current = -1;
    if (stage == 0) {
        gLevelScript.script2.steps = NULL;
    }
    if (stage == 1) {
        gLevelScript.script2.steps = sPrimagenSpawnScript1_2;
    }
    if (stage == 2) {
        gLevelScript.script2.steps = sPrimagenSpawnScript2_2;
    }
    if (stage == 3) {
        gLevelScript.script2.steps = sPrimagenSpawnScript3_2;
    }
}

void Primagen_UpdateMode24_2(CGameObjectInstance *obj, CAI *ai) {
    f32 dy;

    if (ai->m_AnimRepeatedCount == 0) {
        return;
    }
    dy = obj->header.m_vPos.y - gLevelScript.dest.y;
    if (!(ABS(dy) < 10.0f)) {
        return;
    }
    obj->m_vVelocity.x = 0.0f;
    obj->m_vVelocity.y = 0.0f;
    obj->m_vVelocity.z = 0.0f;
    gLevelScript.unk3CA = 0;
    gLevelScript.script.current = -1;
    D_800F1C40[0] &= ~0x300;
    D_800F1F50[0] &= ~0x300;
    if (gLevelScript.bossState == 3) {
        obj->header.m_vPos.x = gLevelScript.home.x;
        obj->header.m_vPos.y = gLevelScript.home.y;
        obj->header.m_vPos.z = gLevelScript.home.z;
        obj->m_RotY = 0.0f;
        gLevelScript.bossMode = 1;
        if (gLevelScript.stage == 1) {
            StartScript2(1);
        }
        if (gLevelScript.stage == 2) {
            StartScript2(2);
        }
        if (gLevelScript.stage == 3) {
            StartScript2(3);
        }
        CAI__SetMode(obj, ai, 5);
    } else if (!gLevelScript.partActive) {
        gLevelScript.bossMode = 2;
        CAI__SetMode(obj, ai, 7);
    } else {
        CAI__SetMode(obj, ai, 0x10);
    }
}
