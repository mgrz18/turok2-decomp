/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00216F44.c, func_80216F44_de.
 * The same code in Turok 2, on CGameObjectInstance (one body for the N64 and the port since
 * round 29: Rage Wars' view read the yaw at the N64 0x50, which on the host is not m_RotY, so
 * every AI saw its targets at an angle that ignored its own rotation; round 7 merge, demo 1). */

#include "common.h"
#include "protos/game_object.h"
#include "protos/cai.h"
#include "structs/vector.h"
#include "structs/game_object.h"

/* The signed angle from the actor's heading to `point`, in the XZ plane. */
f32 AI_AngleToPoint(CGameObjectInstance *actor, CVector3 point) {
    f32 dx;
    f32 dz;
    f32 dist;
    f32 s;
    f32 c;
    f32 dot;
    f32 angle;

    dx = point.x - actor->header.m_vPos.x;
    dz = point.z - actor->header.m_vPos.z;
    dist = func_00298470(dx * dx + dz * dz);
    if (dist == 0.0f) {
        return 0.0f;
    }
    do {
        s = func_002982F0(actor->m_RotY);
    } while (0);
    c = -func_002974C0(actor->m_RotY);
    dot = (dx * -s + dz * c) / dist;
    dot = ((((dot) > (1.0f) ? (1.0f) : (dot))) < ((-1.0f)) ? ((-1.0f)) : (((dot) > (1.0f) ? (1.0f) : (dot))));
    angle = Math_AcosLookup(dot);
    if (dx * c + dz * s > 0.0f) {
        return angle;
    }
    return -angle;
}
