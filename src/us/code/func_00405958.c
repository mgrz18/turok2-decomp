#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "globals/engine.h"

extern void AI_UpdateTargets(CGameObjectInstance *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern void AI_StopSound(CGameObjectInstance *obj, CAI *ai);
extern void CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);

extern s32 D_800F1F50[];
extern s32 gLevelScript_values[]; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern s32 D_8012F9A8[];

void Primagen_UpdateMode2(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;
    s32 limit;
    u32 bit;

    D_800F1F50[0] |= 0x8000;
    AI_UpdateTargets(obj, ai, &t, 1, 1);
    D_800F1F50[0] &= ~0x8000;

    limit = 0x4B00;
    if (gLevelScript.stage == 0) {
        limit = 0x2580;
    }
    if (gLevelScript.damage >= limit) {
        AI_StopSound(obj, ai);
        ai->m_VisBits &= ~(1 << gLevelScript.part);
        gLevelScript.partActive = 0;
        CAI__SetMode(obj, ai, 7);
    } else if (gLevelScript.partTimer <= 0.0f) {
        AI_StopSound(obj, ai);
        gLevelScript_values[gLevelScript.part] = D_8012F9A8[gLevelScript.part];
        bit = 1 << gLevelScript.part;
        gLevelScript.partActive = 0;
        gLevelScript.partsDone |= bit;
        ai->m_VisBits |= bit;
        CAI__SetMode(obj, ai, 7);
    } else {
        gLevelScript.partTimer -= gFrameIncrement;
    }
}
