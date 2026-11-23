#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "protos/ai.h"
#include "globals/collision.h"

extern f32 func_00298470(f32 x);                                          /* sqrt */


void Primagen_UpdateMode23(CGameObjectInstance *obj, CAI *ai) {
    CVector3 flat;

    if (ai->m_AnimRepeatedCount == 0) {
        return;
    }
    if (!(obj->header.m_vPos.y < 10.0f)) {
        return;
    }
    gLevelScript.script.current = -1;
    gLevelScript.unk3CA = 0;
    gLevelScript.bossMode = 2;
    flat.x = obj->header.m_vPos.x;
    flat.y = 0.0f;
    flat.z = obj->header.m_vPos.z;
    if (func_00298470(flat.x * flat.x + flat.z * flat.z) < 1638.3999f) {
        gCollisionInfoEnemyB.m_dwFlags &= ~0x300;
        gCollisionInfoFlyingEnemy.m_dwFlags &= ~0x300;
    }
    if (!gLevelScript.partActive) {
        CAI__SetMode(obj, ai, 7);
    } else {
        CAI__SetMode(obj, ai, 0x10);
    }
}
