#include "common.h"
#include "structs/game_object.h"
#include "structs/region.h"
#include "structs/collision.h"
#include "globals/scene.h"


CRegionInfo *CScene__GetRegionInfo(s32 *list, CGameRegion *region);
void CGameRegion__GetNormalRaw(CVector3 *normal, CGameRegion *region);
s32 CGameRegion__IsSteep(CGameRegion *region);
s32 func_00267EA8(CGameObjectInstance *obj);
s32 func_00267EE8(CGameObjectInstance *obj);
f32 CGameRegion__GetHeight(CGameRegion *region, f32 x, f32 z);
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
f32 func_002982F0(f32);   /* sinf */
f32 func_002974C0(f32);   /* cosf */
f32 func_00298470(f32);   /* sqrtf */

/* Whether the sweep stays on the ground of its region; if so, end is put
 * on the region's surface at the same distance along the slope. */
void Sweep_FollowGround(Sweep *sweep) {
    CGameObjectInstance *obj;
    CRegionInfo *info;
    CVector3 normal;
    CVector3 toEnd;
    s32 allowed;
    s32 ok;
    f32 startY;
    f32 endY;
    f32 dy;
    f32 dx;
    f32 dz;
    f32 sum;
    f32 len;
    f32 k;
    f32 x;
    s32 flag;

    if (sweep->def->m_dwFlags & 0x01000000) {
        sweep->onGround = 0;
        return;
    }
    obj = sweep->obj;
    if (sweep->checkRegionInfo) {
        info = CScene__GetRegionInfo(&gScene, (CGameRegion *)obj->header.m_pCurrentRegion);
        if (info != NULL && (info->m_dwFlags & 0x4000)) {
            flag = 0;
        } else {
            flag = sweep->def->m_GroundBehavior;
        }
    } else {
        flag = sweep->def->m_GroundBehavior;
    }
    if (!(sweep->def->m_dwFlags & 0x8000)) {
        sweep->onGround = 0;
        return;
    }
    if (flag == 0) {
        sweep->onGround = 0;
        return;
    }
    CGameRegion__GetNormalRaw(&normal, (CGameRegion *)obj->header.m_pCurrentRegion);
    if (CGameRegion__IsSteep((CGameRegion *)obj->header.m_pCurrentRegion)) {
        if (func_00267EA8(obj)) {
            if (!(((CGameRegion *)obj->header.m_pCurrentRegion)->m_wFlags & 0x10)) {
                sweep->onGround = 0;
                return;
            }
            if (obj->header.m_Type == 1 &&
                normal.x * -func_002982F0(obj->m_RotY) + normal.z * -func_002974C0(obj->m_RotY) >= 0.0f) {
                sweep->onGround = 0;
                return;
            }
        } else {
            allowed = 0;
            if ((sweep->def->m_dwFlags & 0x10000) || func_00267EE8(obj)) {
                allowed = 1;
            }
            if (allowed == 0) {
                sweep->onGround = 0;
                return;
            }
        }
    }
    startY = CGameRegion__GetHeight((CGameRegion *)obj->header.m_pCurrentRegion, obj->header.m_vPos.x, obj->header.m_vPos.z);
    endY = CGameRegion__GetHeight((CGameRegion *)obj->header.m_pCurrentRegion, sweep->end.x, sweep->end.z);
    if (obj->header.m_pCurrentRegion != NULL) {
        CVector3__Subtract(&toEnd, &sweep->end, ((CGameRegion *)obj->header.m_pCurrentRegion)->m_pVertices[0]);
    } else {
        toEnd = sweep->end;
    }
    sum = normal.x * toEnd.x + normal.y * toEnd.y + normal.z * toEnd.z;
    ok = sum <= 0.0f;
    sweep->onGround = ok;
    if (ok) {
        dy = endY - startY;
        dx = sweep->end.x - obj->header.m_vPos.x;
        dz = sweep->end.z - obj->header.m_vPos.z;
        sum = dx * dx + dz * dz;
        len = sum + dy * dy;
        if (len == 0.0f) {
            sweep->end.y = startY;
            return;
        }
        k = func_00298470(sum / len);
        x = obj->header.m_vPos.x + k * (sweep->end.x - obj->header.m_vPos.x);
        sweep->end.y = startY + k * dy;
        sweep->end.x = x;
        sweep->end.z = obj->header.m_vPos.z + k * (sweep->end.z - obj->header.m_vPos.z);
    }
}
