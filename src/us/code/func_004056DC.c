#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern void func_00246108(CGameObjectInstance *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern void func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);
extern f32 func_002119FC(f32 lo, f32 hi);
extern s32 func_00404C44(CGameObjectInstance *obj, CAI *ai, f32 dist);

extern s32 D_800F1F50[];
extern f32 D_800B6D28;          /* per-frame scale */
extern f32 D_8012F9F8;         /* cooldown */
extern BossStep *D_8012F9FC;   /* script */
extern s32 D_8012FA00;         /* cursor */

void func_004056DC(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;

    D_800F1F50[0] |= 0x8000;
    func_00246108(obj, ai, &t, 1, 1);
    D_800F1F50[0] &= ~0x8000;

    if (D_8012F9D8.state == 10 && ai->m_AnimRepeatedCount != 0 && t.m_Sight.m_pTarget != 0) {
        D_8012F9D8.mode = 2;
        func_00243414(obj, ai, 7);
        return;
    }
    if (D_8012F9D8.state == 5 && ai->m_AnimRepeatedCount != 0 && D_8012F9D8.timer <= 0.0f) {
        D_8012F9D8.mode = 1;
        func_00243414(obj, ai, 5);
        return;
    }
    if (D_8012F9D8.timer > 0.0f) {
        D_8012F9D8.timer -= D_800B6D28;
    }
    if (D_8012F9D8.scriptOn && D_8012F9D8.state != 10) {
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
            } while ((f32) D_8012F9FC[D_8012FA00].chance <= func_002119FC(0.0f, 100.0f));
        }
        if (func_00404C44(obj, ai, t.m_Sight.m_Dist)) {
            return;
        }
    }
    if (D_8012F9D8.state == 6 && t.m_Sight.m_Dist > 460.8f) {
        D_8012F9D8.mode = 2;
        func_00243414(obj, ai, 7);
    }
}
