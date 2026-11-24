/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002168B8.c, func_002168B8.
 * The same code in Turok 2, on Turok 2's types: the target's position goes to AI_FindClearHeading
 * as one CVector3 by value (round 29: Rage Wars' view passed its three words, which the port had
 * to route through AI_FindClearHeading_i3). */
#include "common.h"
#include "protos/game_object.h"
#include "protos/cai.h"
#include "structs/game_object.h"
#include "globals/engine.h"

/* Turns the object toward `t` (around what blocks the way), at most `lim` a frame. */
void AI_TurnTowardTarget(CGameObjectInstance *o, CAI *ai, CGameObjectInstance *t, f32 lim) {
    f32 r, v, a;
    if (t != NULL) {
        r = AI_FindClearHeading(o, t->header.m_vPos, t, 1.5f);
        v = r;
        if (lim < r) r = lim;
        else if (r < -lim) r = -lim;
        r = r * gFrameIncrement;
        a = r;
        if (a < 0.0f) a = -a;
        if (v < 0.0f) {
            if (-v < a) goto blk;
        } else if (v < a) {
blk:
            r = v;
        }
        Actor_TurnBy(o, r);
    }
}
