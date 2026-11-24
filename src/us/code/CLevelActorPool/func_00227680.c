#include "common.h"
#include "structs/level_actor.h"
#include "structs/vector.h"
#include "globals/scene.h"
#include "globals/engine.h"
#include "globals/collision.h"



void CLevelActor__Update(CPooledLevelActor *obj);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define Collision_MoveInstance Collision_MoveInstance_f3
#endif
void Collision_MoveInstance(CPooledLevelActor *obj, f32 x, f32 y, f32 z, void *arg4);

/* Counts the timer down, moves the object (CLevelActor__Update), stops it unless its definition is
 * gLevelActorIntelligence, places it at its position and refreshes its bounding box, 81.92 each way. */
void CPooledLevelActor__Update(CPooledLevelActor *obj) {
    CVector3 prev;

    if (obj->m_Timer > 0.0f) {
        obj->m_Timer -= gFrameIncrement;
        prev = obj->m_Actor.header.m_vPos;
    }
    CLevelActor__Update(obj);
    if (obj->m_Actor.header.m_pIntelligence != &gLevelActorIntelligence) {
        obj->m_Actor.m_vVelocity.x = 0.0f;
        obj->m_Actor.m_vVelocity.y = 0.0f;
        obj->m_Actor.m_vVelocity.z = 0.0f;
    }
    Collision_MoveInstance(obj, obj->m_Actor.header.m_vPos.x, obj->m_Actor.header.m_vPos.y, obj->m_Actor.header.m_vPos.z, &gCollisionInfoPooledActor);
    obj->m_Actor.m_Bounds.m_vMin.x = obj->m_Actor.header.m_vPos.x - 81.92f;
    obj->m_Actor.m_Bounds.m_vMin.y = obj->m_Actor.header.m_vPos.y - 81.92f;
    obj->m_Actor.m_Bounds.m_vMin.z = obj->m_Actor.header.m_vPos.z - 81.92f;
    obj->m_Actor.m_Bounds.m_vMax.x = obj->m_Actor.header.m_vPos.x + 81.92f;
    obj->m_Actor.m_Bounds.m_vMax.y = obj->m_Actor.header.m_vPos.y + 81.92f;
    obj->m_Actor.m_Bounds.m_vMax.z = obj->m_Actor.header.m_vPos.z + 81.92f;
}
