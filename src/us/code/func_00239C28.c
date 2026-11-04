#include "common.h"
#include "structs/game_object.h"
#include "structs/region.h"

extern u32 D_800B6D20;

void func_0026CB2C(CGameRegion *region, CVector3 *centre);
void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);
s32 func_002698AC(CInstanceHeader *inst, CVector3 pos, s32 arg);

/* Picks the neighbouring region whose centre is farthest from pos and still
 * passes func_002698AC; its centre goes to *out. */
CGameRegion *func_00239C28(CInstanceHeader *inst, CGameRegion *exclude, CVector3 pos, CGameRegion *none,
                      s32 arg, CVector3 *out) {
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
    skipExcluded = D_800B6D20 >> 4;
    skipExcluded &= 1;
    for (i = 0; i < 3; i++) {
        region = cur->m_pNeighbours[i];
        if (region == NULL) {
            continue;
        }
        if (skipExcluded && exclude == region) {
            continue;
        }
        func_0026CB2C(region, &centre);
        func_0020EF2C(&delta, &centre, &pos);
        dist = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
        if (bestDist < dist && func_002698AC(inst, centre, arg) != 0) {
            bestDist = dist;
            best = region;
            *out = centre;
        }
    }
    return best;
}
