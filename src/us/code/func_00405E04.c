#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern void AI_UpdateTargets(CGameObjectInstance *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern void CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);
extern void AI_TurnClamped(CGameObjectInstance *obj, CAI *ai, f32 turnRate, f32 angle);
extern f32 Random_Blend(f32 lo, f32 hi);
extern s32 Primagen_RunAttackStep(CGameObjectInstance *obj, CAI *ai, f32 dist);

extern s32 D_800F1F50[];
extern f32 D_8012F9F8;         /* cooldown */
extern BossStep *D_8012F9FC;   /* script */
extern s32 D_8012FA00;         /* cursor */

void Primagen_UpdateMode7(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;

    D_800F1F50[0] |= 0x8000;
    AI_UpdateTargets(obj, ai, &t, 1, 1);
    D_800F1F50[0] &= ~0x8000;

    if (t.m_Sight.m_pTarget == NULL) {
        obj->m_AI.m_AnimSpeed = 1.0f;
        D_8012F9D8.mode = 10;
        CAI__SetMode(obj, ai, 1);
        return;
    }
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (!D_8012F9D8.scriptOn) {
        return;
    }
    if (D_8012F9D8.script.current == -1) {
        if (D_8012FA00 != -1) {
            D_8012F9F8 = D_8012F9FC[D_8012FA00].delay;
        } else {
            D_8012F9F8 = 0.0f;
        }
        do {
            D_8012FA00++;
            if (D_8012F9FC[D_8012FA00].type == 5) {
                D_8012FA00 = 0;
            }
            D_8012F9D8.script.current = D_8012FA00;
        } while ((f32) D_8012F9FC[D_8012FA00].chance <= Random_Blend(0.0f, 100.0f));
    }
    if (Primagen_RunAttackStep(obj, ai, t.m_Sight.m_Dist)) {
        return;
    }
    if (t.m_Sight.m_Dist <= 460.8f) {
        obj->m_AI.m_AnimSpeed = 1.0f;
        CAI__SetMode(obj, ai, 1);
    }
}
