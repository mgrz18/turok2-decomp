#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern void CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);
extern f32 func_00298470(f32 x);                                          /* sqrt */

extern s32 D_800F1C40[];
extern s32 D_800F1F50[];

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
        D_800F1C40[0] &= ~0x300;
        D_800F1F50[0] &= ~0x300;
    }
    if (!gLevelScript.partActive) {
        CAI__SetMode(obj, ai, 7);
    } else {
        CAI__SetMode(obj, ai, 0x10);
    }
}
