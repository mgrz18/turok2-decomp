/* Second-module twin of Primagen_RunAttackStep (VM module 2 is linked over module 1; same source, its own strings and pool). */
#include "common.h"
#include "structs/game_object.h"
#define BOSS_STAGE_AS_WORD
#include "structs/boss.h"
#include "globals/boss.h"

extern s32 gLevelScript; /* local type: globals/boss.h declares it; this view (its type here) is what keeps the ROM's code */           /* stage */
extern f32 Random_Blend(f32 lo, f32 hi);
extern void CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);

typedef struct {
    /* 0x0 */ f32 waitMin;       /* 0x8012F618 */
    /* 0x4 */ f32 waitMax;       /* 0x8012F61C */
} BossWait;

extern BossWait gLevelScript_swayPhase; /* local type: the field's real type (or the member path) changes the ROM's code here */

s32 Primagen_RunAttackStep_2(CGameObjectInstance *obj, CAI *ai, f32 dist) {
    f32 scale;

    if (gLevelScript_bossMode.script.timer > 0.0f) {
        return 0;
    }
    switch (gLevelScript_bossMode.script.steps[gLevelScript_bossMode.script.current].type) {
        case 2:
            if (gLevelScript_bossMode.script.steps[gLevelScript_bossMode.script.current].arg == 1) {
                gLevelScript_swayPhase.waitMax = 2.5f;
                gLevelScript_swayPhase.waitMin = 0.0f;
            }
            if (gLevelScript_bossMode.script.steps[gLevelScript_bossMode.script.current].arg == 2) {
                gLevelScript_swayPhase.waitMin = Random_Blend(0.0f, 2.0f);
                gLevelScript_swayPhase.waitMax = Random_Blend(3.0f, 4.5f);
            }
            if (gLevelScript_bossMode.script.steps[gLevelScript_bossMode.script.current].arg == 3) {
                gLevelScript_swayPhase.waitMax = 4.0f;
                gLevelScript_swayPhase.waitMin = 1.0f;
            }
            gLevelScript_bossMode.mode = 9;
            CAI__SetMode(obj, ai, 0x12);
            return 1;
        case 3:
            scale = 10.24f;
            if (sPrimagenAttacks_2[gLevelScript_bossMode.script.steps[gLevelScript_bossMode.script.current].arg].minDist * scale <= dist
                && dist <= sPrimagenAttacks_2[gLevelScript_bossMode.script.steps[gLevelScript_bossMode.script.current].arg].maxDist * scale) {
                obj->m_AI.m_AnimSpeed = gLevelScript_bossMode.script.steps[gLevelScript_bossMode.script.current].animSpeed;
                CAI__SetMode(obj, ai, sPrimagenAttacks_2[gLevelScript_bossMode.script.steps[gLevelScript_bossMode.script.current].arg].mode);
                return 1;
            }
            break;
        case 4:
        case 5:
            break;
        case 6:
            if (gLevelScript == 1) {
                gLevelScript_bossMode.script.steps = sPrimagenAttackScript2Stage1_2;
            }
            if (gLevelScript == 2) {
                gLevelScript_bossMode.script.steps = sPrimagenAttackScript2Stage2_2;
            }
            if (gLevelScript == 3) {
                gLevelScript_bossMode.script.steps = sPrimagenAttackScript2Stage3_2;
            }
            gLevelScript_bossMode.script.cursor = -1;
            gLevelScript_bossMode.script.current = -1;
            gLevelScript_bossMode.mode = 4;
            CAI__SetMode(obj, ai, 0x12);
            return 1;
        case 7:
            if (gLevelScript == 1) {
                gLevelScript_bossMode.script.steps = sPrimagenAttackScriptStage1_2;
            }
            if (gLevelScript == 2) {
                gLevelScript_bossMode.script.steps = sPrimagenAttackScriptStage2_2;
            }
            if (gLevelScript == 3) {
                gLevelScript_bossMode.script.steps = sPrimagenAttackScriptStage3_2;
            }
            gLevelScript_bossMode.script.cursor = -1;
            gLevelScript_bossMode.script.current = -1;
            gLevelScript_bossMode.mode = 2;
            CAI__SetMode(obj, ai, 0x18);
            return 1;
    }
    return 0;
}
