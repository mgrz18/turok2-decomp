/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/8024D018_3.CPP, func_8024D4AC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/math.h"
#include "protos/game_object.h"
#include "protos/mtxf.h"

CQuaternion CGameObjectInstance__GetRotation(CGameObjectInstance *actor);
void CQuaternion__ToMatrix(f32 *q, f32 *m);
void CQuaternion__Multiply(CQuaternion *out, CQuaternion *a, CQuaternion *b);
CGameObjectInstance *CPlayerPool__FindNearest(void *pool, CVector3 p);

/* The rotation that turns an actor (or the world's z axis, without one) toward its sight target, or
 * else the nearest player, at three quarters of its collision height. Round 28: on
 * CGameObjectInstance (Rage Wars' LevelActor view read the sight target at 0x1C0 and the collision
 * y offset at 0x54, which on the host missed). */
CQuaternion GameObject_GetAimRotation(CGameObjectInstance *actor, CVector3 point) {
    CVector3 origin;
    CVector3 dir;
    CVector3 forward;
    CVector3 local;
    CVector3 axis;
    CQuaternion turn;
    CQuaternion aim;
    CQuaternion result;
    CMtxF matrix;
    CGameObjectInstance *target;
    CGameObjectInstance *found;
    f32 angle;
    f32 scale;

    target = NULL;
    if (actor != NULL && actor->header.m_Type == 1) {
        target = actor->m_AI.m_pSightTarget;
        aim = CGameObjectInstance__GetRotation(actor);
        CQuaternion__ToMatrix((f32 *) &aim, (f32 *) matrix);
        local.x = 0.0f;
        local.y = 0.0f;
        local.z = 1.0f;
        CMtxF__TransformPoint(matrix, &local, &forward);
    } else {
        forward.x = 0.0f;
        forward.y = 0.0f;
        forward.z = 1.0f;
    }
    if (target == NULL) {
        found = CPlayerPool__FindNearest(&gEngineApp_m_Game.m_PlayerList, point);
        if (found != NULL) {
            target = found;
        }
        if (target == NULL) {
            result.x = result.y = result.z = 0.0f;
            result.w = 1.0f;
            return result;
        }
    }
    origin = target->header.m_vPos;
    origin.y += target->m_CollisionYOffset;
    origin.y += GameObject_GetCollisionHeight(target) * 0.75f;
    CVector3__Subtract(&dir, &origin, &point);
    CVector3__Normalize(&dir);
    CVector3__Cross(&axis, &forward, &dir);
    CVector3__Normalize(&axis);
    angle = Math_AcosLookup(forward.x * dir.x + forward.y * dir.y + forward.z * dir.z) * 0.5f;
    scale = func_002982F0(angle);
    turn.x = axis.x * scale;
    turn.y = axis.y * scale;
    turn.z = axis.z * scale;
    gQuaternionSin = scale;
    turn.w = func_002974C0(angle);
    if (actor != NULL && actor->header.m_Type == 1) {
        CQuaternion__Multiply(&result, &aim, &turn);
    } else {
        result = turn;
    }
    return result;
}
