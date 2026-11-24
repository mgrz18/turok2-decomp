#include "common.h"
#include "protos/game_object.h"
#include "protos/cai.h"
#include "structs/vector.h"
#include "structs/game_object.h"
#include "structs/particle.h"
#include "globals/scene.h"

/* Spawns a particle of `kind` from `obj` at `at`, moving with it and oriented like it: a game
 * object (type 1) gives its velocity, rotation and sight target (m_AI.m_pSightTarget), a particle
 * (type 3) its velocity and ground rotation. One body for the N64 and the port since round 29
 * (the N64 view, SpawnObj, had the velocity at 0x18 and the target at 0x1C0, which missed the
 * host members, so the port had its own copy). */
void Actor_SpawnParticle(CInstanceHeader *obj, s32 kind, CVector3 at) {
    CVector3 pos;
    CVector3 vel;
    CQuaternion rot;
    CGameRegion *region;
    s32 mode;
    sptr target;

    mode = 1;
    GameObject_TraceMove(obj, at, &pos, &region, 0, 1);
    target = 0;
    switch (obj->m_Type) {
    case 0:
    case 2:
        vel.x = vel.y = vel.z = 0.0f;
        rot.x = rot.y = rot.z = 0.0f;
        rot.w = 1.0f;
        break;
    case 1:
        vel = ((CGameObjectInstance *) obj)->m_vVelocity;
        rot = CGameObjectInstance__GetRotation((CGameObjectInstance *) obj);
        target = (sptr) ((CGameObjectInstance *) obj)->m_AI.m_pSightTarget;
        if (CGameObjectInstance__GetGeometryKind(obj) == 5) {
            mode = 3;
        }
        break;
    case 3:
        vel = ((CParticle *) obj)->m_vVelocity;
        rot = Actor_GetGroundRotation(obj);
        break;
    }
    CParticleSystem__Spawn(&gScene.m_ParticleSystem, obj, obj, NULL, target, kind, vel, rot, pos, region, -1, mode);
}
