#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* AI state: turn toward the target (when there is one); once the anim is done, sends the module's six other
 * objects to anim state 0 and goes to anim state 2. */

typedef struct Obj {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[1];
} Obj;


extern void func_00246108(Obj *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern void func_00245BAC(Obj *obj, CAI *ai, f32 angle, f32 dist);
extern void func_00243414(Obj *obj, void *ai, s32 state);

void func_0040B0B0(Obj *obj, CAI *ai) {
    CAITargets t;
    CAITargets t2;
    MotherBossState *state;
    s32 i;

    func_00246108(obj, ai, &t, 0, 1);
    if (t.m_Sight.m_pTarget != 0) {
        func_00246108(obj, ai, &t2, 0, 1);
        func_00245BAC(obj, ai, 0.261799425f, t2.m_Sight.m_Angle);
    }
    if (ai->m_AnimRepeatedCount) {
        state = &D_8012FB70;
        for (i = 0; i < 6; i++) {
            func_00243414((Obj *)state->parts[i], ((Obj *)state->parts[i])->ai, 0);
        }
        func_00243414(obj, ai, 2);
    }
}
