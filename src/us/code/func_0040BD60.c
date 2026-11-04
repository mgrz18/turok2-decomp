#include "common.h"
#include "structs/game_object.h"

/* Returns the distance to the AI's second target when it is an actor with flag 0x300000 (and stores that
 * actor in *found), else -1 (and NULL). */

typedef struct {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[1];
} Obj;

extern void func_00246108(Obj *obj, void *ai, CAITargets *t, s32 f1, s32 f2);

f32 func_0040BD60(Obj *obj, CGameObjectInstance **found) {
    CAITargets t;
    f32 dist;

    dist = -1.0f;
    *found = NULL;
    func_00246108(obj, obj->ai, &t, 1, 0);
    if (t.m_Path.m_pTarget != NULL && (t.m_Path.m_pTarget->m_dwFlags & 0x300000)) {
        dist = t.m_Path.m_FlatDist;
        *found = t.m_Path.m_pTarget;
    }
    return dist;
}
