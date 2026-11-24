/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8024E140_de_3.c, func_8024E59C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "protos/game_object.h"

/* The floor point under point: traced from its x and z at height 0, then the height of the region
 * the trace ends in. Round 28: the position is a CVector3 (Rage Wars' TripleRecAt8 had an s32 y,
 * which on the host needed the GameObject_TraceMove_fif wrapper of port/game/t2abi.c). */
void GameObject_GetFloorPointAt(CInstanceHeader *obj, CVector3 *point, CVector3 *out) {
    CVector3 pos;
    CGameRegion *region;

    pos.x = point->x;
    pos.y = 0.0f;
    pos.z = point->z;
    GameObject_TraceMove(obj, pos, out, &region, 0, 0);
    out->y = CGameRegion__GetHeight(region, out->x, out->z);
}
