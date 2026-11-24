#include "common.h"
#include "protos/game_object.h"
#include "protos/cai.h"
#include "structs/vector.h"
#include "structs/game_object.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0021740C.c, func_0021740C.
 * The same code in Turok 2, on Turok 2's types: the new position goes to GameObject_TraceMove as
 * one CVector3 by value (round 29: Rage Wars' view passed three floats, which the port had to
 * route through GameObject_TraceMove_f3). */

/* Pushes `b` out of `a`'s way, away from `c` (or in place without one): to the sum of the two
 * collision radii (plus 51.2) from `c`, traced from b's position into *outPos / *outRegion. */
void AI_PushAway(CGameObjectInstance *a, s32 unused, CGameObjectInstance *b, CGameObjectInstance *c,
                 CVector3 *outPos, CGameRegion **outRegion) {
    CVector3 d;
    CVector3 r;
    if (c != NULL) {
        CVector3__Subtract(&d, &b->header.m_vPos, &c->header.m_vPos);
        d.y = 0;
        CVector3__Normalize(&d);
    } else {
        d.x = 0;
        d.y = 0;
        d.z = 0;
    }
    CVector3__Scale(&d, &d, GameObject_GetCollisionRadius(b) + GameObject_GetCollisionRadius(a) + 51.199997f);
    CVector3__Add(&r, &b->header.m_vPos, &d);
    GameObject_TraceMove(&b->header, r, outPos, outRegion, 0, 0);
}
