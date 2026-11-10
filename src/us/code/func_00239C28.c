#include "common.h"
#include "structs/game_object.h"
#include "structs/region.h"
#include "globals/engine.h"


void CGameRegion__GetCentroid(CGameRegion *region, CVector3 *centre);
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
s32 GameObject_CanMoveTo(CInstanceHeader *inst, CVector3 pos, sptr arg);

/* Picks the neighbouring region whose centre is farthest from pos and still
 * passes GameObject_CanMoveTo; its centre goes to *out. */
CGameRegion *Path_FindFleeRegion(CInstanceHeader *inst, CGameRegion *exclude, CVector3 pos, CGameRegion *none,
                      sptr arg, CVector3 *out) {
    CGameRegion *cur;
    CGameRegion *region;
    CGameRegion *best;
    CVector3 centre;
    CVector3 delta;
    f32 bestDist;
    f32 dist;
    s32 skipExcluded;
    s32 i;

    *out = pos;
    cur = (CGameRegion *)inst->m_pCurrentRegion;
    if (cur == NULL) {
        return none;
    }
    best = NULL;
    bestDist = 0.0f;
    skipExcluded = gFrameCountGameplay >> 4;
    skipExcluded &= 1;
    for (i = 0; i < 3; i++) {
        region = cur->m_pNeighbours[i];
        if (region == NULL) {
            continue;
        }
        if (skipExcluded && exclude == region) {
            continue;
        }
        CGameRegion__GetCentroid(region, &centre);
        CVector3__Subtract(&delta, &centre, &pos);
        dist = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
        if (bestDist < dist && GameObject_CanMoveTo(inst, centre, arg) != 0) {
            bestDist = dist;
            best = region;
            *out = centre;
        }
    }
    return best;
}
