#include "common.h"
#include "structs/game_object.h"

/* A navigation region: a triangle with its three corners and the regions
 * across each edge. */
typedef struct Region {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 flags;
    /* 0x04 */ CVector3 *corner[3];
    /* 0x10 */ struct Region *neighbour[3];
} Region;

typedef struct SweepDef {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s8 onGround;
} SweepDef;

typedef struct RegionInfo {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ s32 flags;
} RegionInfo;

/* A segment swept from an instance's position to end. */
typedef struct Sweep {
    /* 0x00 */ CGameObjectInstance *obj;
    /* 0x04 */ SweepDef *def;
    /* 0x08 */ u8 pad8[0x44];
    /* 0x4C */ CVector3 end;
    /* 0x58 */ u8 pad58[0x3C];
    /* 0x94 */ s32 onGround;
    /* 0x98 */ s32 checkRegionInfo;
} Sweep;

extern s32 D_800F7078[];

RegionInfo *func_0022425C(s32 *list, Region *region);
void func_0026BC54(CVector3 *normal, Region *region);
s32 func_0026AFC0(Region *region);
s32 func_00267EA8(CGameObjectInstance *obj);
s32 func_00267EE8(CGameObjectInstance *obj);
f32 func_0026BE60(Region *region, f32 x, f32 z);
void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);
f32 func_002982F0(f32);   /* sinf */
f32 func_002974C0(f32);   /* cosf */
f32 func_00298470(f32);   /* sqrtf */

/* Whether the sweep stays on the ground of its region; if so, end is put
 * on the region's surface at the same distance along the slope. */
void func_0023BE24(Sweep *sweep) {
    CGameObjectInstance *obj;
    RegionInfo *info;
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

    if (sweep->def->flags & 0x01000000) {
        sweep->onGround = 0;
        return;
    }
    obj = sweep->obj;
    if (sweep->checkRegionInfo) {
        info = func_0022425C(D_800F7078, (Region *)obj->header.m_pCurrentRegion);
        if (info != NULL && (info->flags & 0x4000)) {
            flag = 0;
        } else {
            flag = sweep->def->onGround;
        }
    } else {
        flag = sweep->def->onGround;
    }
    if (!(sweep->def->flags & 0x8000)) {
        sweep->onGround = 0;
        return;
    }
    if (flag == 0) {
        sweep->onGround = 0;
        return;
    }
    func_0026BC54(&normal, (Region *)obj->header.m_pCurrentRegion);
    if (func_0026AFC0((Region *)obj->header.m_pCurrentRegion)) {
        if (func_00267EA8(obj)) {
            if (!(((Region *)obj->header.m_pCurrentRegion)->flags & 0x10)) {
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
            if ((sweep->def->flags & 0x10000) || func_00267EE8(obj)) {
                allowed = 1;
            }
            if (allowed == 0) {
                sweep->onGround = 0;
                return;
            }
        }
    }
    startY = func_0026BE60((Region *)obj->header.m_pCurrentRegion, obj->header.m_vPos.x, obj->header.m_vPos.z);
    endY = func_0026BE60((Region *)obj->header.m_pCurrentRegion, sweep->end.x, sweep->end.z);
    if (obj->header.m_pCurrentRegion != NULL) {
        func_0020EF2C(&toEnd, &sweep->end, ((Region *)obj->header.m_pCurrentRegion)->corner[0]);
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
