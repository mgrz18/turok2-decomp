#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"

extern void AI_UpdateTargets(CGameObjectInstance *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);

extern s32 D_800F1F50[];
extern s32 gLevelScript_script_current[]; /* local type: the field's real type (or the member path) changes the ROM's code here */        /* boss script index; an array so the store stays after the obj store */

void Primagen_UpdateMode11(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;

    D_800F1F50[0] |= 0x8000;
    AI_UpdateTargets(obj, ai, &t, 1, 1);
    D_800F1F50[0] &= ~0x8000;
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount != 0) {
        obj->m_AI.m_AnimSpeed = 1.0f;
        gLevelScript_script_current[0] = -1;
        CAI__SetMode(obj, ai, 7);
    }
}
