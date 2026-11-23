#include "common.h"
#include "structs/collision.h"
#include "globals/collision.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8023A294_de_24.c, func_8023EE60_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
void CCollisionSetup__Construct(EntryRecAt24 *arg0);

void CCollisionInfo__Reset(void) {
    gCollisionResultsP = &gCollisionResults;
    CCollisionSetup__Construct(gCollisionSetupTable);
}

/* Clears the collision results: nothing hit yet. */
void CCollisionResults__Clear(void) {
    CCollisionResults *p = gCollisionResultsP;

    p->m_pInstance = 0;
    p->m_nInstanceNode = -1;
    p->m_pShape = 0;
    p->m_pWallRegion = 0;
    p->m_nFloorKind = 0;
    p->m_pFloorRegion = 0;
    p->m_pWaterRegion = 0;
}

/* The port: the behaviours are words in the table (gendata swaps them as such), read as words;
 * the N64 view read the low byte of each big-endian word (u8 at +3), which on the host is the
 * high byte: every behaviour came out 0 and nothing collided with walls or floors. */
#include "structs/collision.h"
typedef struct {
    CCollisionInfo *dst;
    s32 val0;
    s32 flag0;
    s32 flag1;
    s32 flag2;
    f32 x;
    f32 y;
    f32 z;
    f32 w;
    f32 extra;
} HostCollisionSetup;

void CCollisionSetup__Construct(EntryRecAt24 *arg0) {
    HostCollisionSetup *e = (HostCollisionSetup *) arg0;
    s32 counter = -1;

    while (e->dst != NULL) {
        CCollisionInfo *dst = e->dst;

        gCollisionResultsP->m_pInstance = NULL;
        gCollisionResultsP->m_nInstanceNode = counter;
        gCollisionResultsP->m_pShape = NULL;
        gCollisionResultsP->m_pWallRegion = NULL;
        gCollisionResultsP->m_nFloorKind = 0;
        gCollisionResultsP->m_pFloorRegion = NULL;
        gCollisionResultsP->m_pWaterRegion = NULL;
        dst->m_dwFlags = e->val0;
        dst->m_InstanceBehavior = e->flag0;
        dst->m_WallBehavior = e->flag1;
        dst->m_GroundBehavior = e->flag2;
        dst->m_GravityAcceleration = e->x;
        dst->m_BounceReturnEnergy = e->y;
        dst->m_GroundFriction = e->z;
        dst->m_AirFriction = e->w;
        dst->m_WaterFriction = e->extra;
        e += 1;
    }
}
