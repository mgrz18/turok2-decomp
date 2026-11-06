#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"

extern void AI_UpdateTargets(CGameObjectInstance *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern void CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);
extern f32 Random_Blend(f32 lo, f32 hi);
extern s32 Primagen_RunAttackStep(CGameObjectInstance *obj, CAI *ai, f32 dist);

extern s32 D_800F1F50[];
extern f32 gFrameIncrement;          /* per-frame scale */
extern f32 gLevelScript_script;         /* cooldown */
extern s32 gLevelScript_script_cursor;         /* cursor */

void Primagen_UpdateMode1(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;

    D_800F1F50[0] |= 0x8000;
    AI_UpdateTargets(obj, ai, &t, 1, 1);
    D_800F1F50[0] &= ~0x8000;

    if (gLevelScript_bossState.state == 10 && ai->m_AnimRepeatedCount != 0 && t.m_Sight.m_pTarget != 0) {
        gLevelScript_bossState.mode = 2;
        CAI__SetMode(obj, ai, 7);
        return;
    }
    if (gLevelScript_bossState.state == 5 && ai->m_AnimRepeatedCount != 0 && gLevelScript_bossState.timer <= 0.0f) {
        gLevelScript_bossState.mode = 1;
        CAI__SetMode(obj, ai, 5);
        return;
    }
    if (gLevelScript_bossState.timer > 0.0f) {
        gLevelScript_bossState.timer -= gFrameIncrement;
    }
    if (gLevelScript_bossState.scriptOn && gLevelScript_bossState.state != 10) {
        if (gLevelScript_bossState.script.current == -1) {
            if (gLevelScript_script_cursor != -1) {
                gLevelScript_script = gLevelScript.script.steps[gLevelScript_script_cursor].delay;
            } else {
                gLevelScript_script = 0.0f;
            }
            do {
                gLevelScript_script_cursor++;
                if (gLevelScript.script.steps[gLevelScript_script_cursor].type == 5) {
                    gLevelScript_script_cursor = 0;
                }
                gLevelScript_bossState.script.current = gLevelScript_script_cursor;
            } while ((f32) gLevelScript.script.steps[gLevelScript_script_cursor].chance <= Random_Blend(0.0f, 100.0f));
        }
        if (Primagen_RunAttackStep(obj, ai, t.m_Sight.m_Dist)) {
            return;
        }
    }
    if (gLevelScript_bossState.state == 6 && t.m_Sight.m_Dist > 460.8f) {
        gLevelScript_bossState.mode = 2;
        CAI__SetMode(obj, ai, 7);
    }
}
