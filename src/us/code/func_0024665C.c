#include "structs/game_object.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/WEAPONS/80213ED4_22.CPP, func_80217290_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* Turok 2: actor position at 0x4 (Rage Wars 0x8). */
/* Squared horizontal (XZ) distance from the actor to point. */
f32 AI_FlatDistSqToPoint(CGameObjectInstance *arg0, CVector3 point)
{
    f32 dx = point.x - arg0->header.m_vPos.x;
    f32 dz = point.z - arg0->header.m_vPos.z;
    return dx * dx + dz * dz;
}
