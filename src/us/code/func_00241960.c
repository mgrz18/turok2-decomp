#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "globals/math.h"
#include "protos/game.h"

typedef f32 Mtx4[4][4];


f32 CGameRegion__GetHeight(void *region, f32 x, f32 z);
f32 GameObject_GetCollisionRadius(CGameObjectInstance *collider);
f32 GameObject_GetCollisionHeight(CGameObjectInstance *collider);
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
void CMtxF__SetRotateX(Mtx4 *mtx, f32 angle);
void CQuaternion__Multiply(CQuaternion *out, CQuaternion *a, CQuaternion *b);
void CQuaternion__ToMatrix(CQuaternion *q, Mtx4 *mtx);
void CMtxF__MultiplyAffine(Mtx4 *out, Mtx4 *a, Mtx4 *b);
void CMtxF__Translate(Mtx4 *mtx, f32 x, f32 y, f32 z);
void CMtxF__TransformPoints(Mtx4 *mtx, CVector3 *in, CVector3 *out, s32 count);

/* The eight corners of the mover's collision box (top face first), in world space. */
void Platform_GetCollisionBox(CGameObjectInstance *mover, CVector3 *points, CGameObjectInstance *collider) {
    Mtx4 mtx;
    Mtx4 rot;
    Mtx4 spinMtx;
    CQuaternion q;
    CQuaternion spin;
    CPlatformIntelligence *info;
    CAI *ai;
    f32 halfX;
    f32 halfZ;
    f32 height;
    f32 bottomOff;
    f32 minX, maxX, minZ, maxZ;
    f32 top, bottom;
    f32 x;
    f32 z;
    f32 sinHalf;

    info = &mover->header.m_pIntelligence->m_Platform;
    ai = &mover->m_AI;
    if (info->m_CollisionType == 0) {
        x = mover->header.m_vPos.x;
        minX = x + -10240.0f;
        maxX = x + 10240.0f;
        top = CGameRegion__GetHeight(mover->header.m_pCurrentRegion, x, mover->header.m_vPos.z) + 0.001f;
        z = mover->header.m_vPos.z;
        minZ = z + -10240.0f;
        maxZ = z + 10240.0f;
        bottom = top - 10240.0f;
        points[0].x = minX;
        points[1].x = maxX;
        points[2].x = maxX;
        points[3].x = minX;
        points[4].x = minX;
        points[5].x = maxX;
        points[6].x = maxX;
        points[7].x = minX;
        points[0].y = top;
        points[1].y = top;
        points[2].y = top;
        points[3].y = top;
        points[0].z = maxZ;
        points[1].z = maxZ;
        points[2].z = minZ;
        points[3].z = minZ;
        points[4].z = maxZ;
        points[5].z = maxZ;
        points[6].z = minZ;
        points[7].z = minZ;
    } else {
        switch (info->m_CollisionType) {
        default:
            halfX = info->m_CollisionWidth * 0.5f;
            height = info->m_CollisionHeight;
            bottomOff = 0.0f;
            halfZ = info->m_CollisionLength * 0.5f;
            if (collider != NULL) {
                if (collider->m_dwFlags & 0x300000) {
                    halfX += GameObject_GetCollisionRadius(collider) * 0.5f;
                    bottomOff = -GameObject_GetCollisionHeight(collider);
                    halfZ += GameObject_GetCollisionRadius(collider) * 0.5f;
                } else {
                    halfX += GameObject_GetCollisionRadius(collider);
                    bottomOff = -GameObject_GetCollisionHeight(collider);
                    halfZ += GameObject_GetCollisionRadius(collider);
                }
            }
            break;
        case 1:
            if (collider != NULL) {
                halfZ = info->m_CollisionRadius + GameObject_GetCollisionRadius(collider);
            } else {
                halfZ = info->m_CollisionRadius;
            }
            height = info->m_CollisionHeight;
            bottomOff = 0.0f;
            do { /* block boundary keeps the copy last */
                halfX = halfZ;
            } while (0);
        }
        minX = info->m_CollisionXOffset - halfX;
        maxX = info->m_CollisionXOffset + halfX;
        bottom = info->m_CollisionYOffset + bottomOff;
        top = info->m_CollisionYOffset + height;
        minZ = info->m_CollisionZOffset - halfZ;
        maxZ = info->m_CollisionZOffset + halfZ;
        points[0].x = minX;
        points[1].x = maxX;
        points[2].x = maxX;
        points[3].x = minX;
        points[4].x = minX;
        points[5].x = maxX;
        points[6].x = maxX;
        points[7].x = minX;
        points[0].y = top;
        points[1].y = top;
        points[2].y = top;
        points[3].y = top;
        points[4].y = bottom;
        points[5].y = bottom;
        points[6].y = bottom;
        points[7].y = bottom;
        points[0].z = maxZ;
        points[1].z = maxZ;
        points[2].z = minZ;
        points[3].z = minZ;
        points[4].z = maxZ;
        points[5].z = maxZ;
        points[6].z = minZ;
        points[7].z = minZ;

        CMtxF__SetRotateX(&rot, ai->m_Platform.m_Pitch);
        sinHalf = func_002982F0(mover->m_RotY * 0.5f);
        q.x = 0.0f;
        q.y = sinHalf;
        q.z = 0.0f;
        gQuaternionSin = sinHalf;
        q.w = func_002974C0(mover->m_RotY * 0.5f);
        CQuaternion__Multiply(&spin, &q, &mover->m_qGround);
        CQuaternion__ToMatrix(&spin, &spinMtx);
        CMtxF__MultiplyAffine(&mtx, &rot, &spinMtx);
        CMtxF__Translate(&mtx, mover->header.m_vPos.x, mover->header.m_vPos.y, mover->header.m_vPos.z);
        CMtxF__TransformPoints(&mtx, points, points, 8);
        if (!(info->m_dwFlags & 1)) {
            return;
        }
        bottom = GameObject_GetRegionFloorHeight(mover, 0);
    }
    points[4].y = bottom;
    points[5].y = bottom;
    points[6].y = bottom;
    points[7].y = bottom;
}
