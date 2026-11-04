#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern void func_00246108(CGameObjectInstance *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern void func_00245BAC(CGameObjectInstance *obj, CAI *ai, f32 turnRate, f32 angle);
extern void func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);

extern s32 D_800F1F50[];

void func_004078B8(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;

    D_800F1F50[0] |= 0x8000;
    func_00246108(obj, ai, &t, 1, 1);
    D_800F1F50[0] &= ~0x8000;
    func_00245BAC(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount == 0) {
        return;
    }
    D_8012F9D8.script.current = -1;
    obj->m_AI.m_AnimSpeed = 1.0f;
    if (D_8012F9D8.state == 6 || t.m_Sight.m_Dist <= 460.8f) {
        D_8012F9D8.mode = 6;
        func_00243414(obj, ai, 1);
    } else if (D_8012F9D8.state == 2) {
        func_00243414(obj, ai, 7);
    }
}
