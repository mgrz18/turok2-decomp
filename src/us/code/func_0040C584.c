#include "common.h"
#include "structs/game_object.h"

/* AI state: turn toward the target (15 degrees per step). */

extern void func_00246108(void *obj, void *ai, CAITargets *t, s32 f1, s32 f2);
extern void func_00245BAC(void *obj, void *ai, f32 angle, f32 dist);

void func_0040C584(void *obj, void *ai) {
    CAITargets t;

    func_00246108(obj, ai, &t, 0, 1);
    func_00245BAC(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
}
