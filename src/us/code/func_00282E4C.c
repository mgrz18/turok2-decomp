#include "common.h"
#include "structs/camera.h"
#include "structs/region.h"
#include "structs/emitter.h"

#define MIN(a, b) ((a) > (b) ? (b) : (a))

extern u8 gScene[];
extern CEmitterDef *D_8010FF14;  /* emitter definitions */
extern s32 D_8010FF18;          /* how many */
extern f32 gFrameIncrement;          /* the frame's time step */
extern f32 D_800B6D34;

CRegionInfo *CScene__GetRegionInfo(void *world, s32 region);
f32 CGameRegion__GetHeight(s32 region, f32 x, f32 z);
CVector3 CCamera__GetViewOrigin(CCamera *camera);
void func_00207914(s32 *resource, s32 id);
void CList__Remove(CList *list, void *element);
void CList__AddHead(CList *list, void *element);
void CList__AddTail(CList *list, void *element);

/* CEmitterPool__TakeEmitter, inlined: takes a free emitter. */
static __inline__ CEmitter *TakeEmitter(CEmitterPool *owner) {
    CEmitter *e = owner->m_FreeList.m_pHead;

    if (e != NULL) {
        CList__Remove(&owner->m_FreeList, e);
        CList__AddTail(&owner->m_ActiveList, e);
    }
    return e;
}

/* CEmitterPool__ReleaseEmitter, inlined: gives an emitter back. */
static __inline__ void FreeEmitter(CEmitterPool *owner, CEmitter *e) {
    CList__Remove(&owner->m_ActiveList, e);
    CList__AddHead(&owner->m_FreeList, e);
}

/* CEmitter__Start, inlined. */
static __inline__ void InitEmitter(CEmitter *e, CEmitterDef *def, s16 mask, f32 scale,
                                   f32 offset0, f32 offset1, f32 offset2) {
    e->m_pDef = def;
    e->m_Mask = mask;
    func_00207914(e->m_Texture, def->m_Texture);
    e->m_ScrollS = 0;
    e->m_ScrollT = 0;
    e->m_Throttle = 0;
    e->m_Phase = 0;
    e->m_PhaseScale = scale;
    e->m_TextureScale = offset0;
    e->m_HeightOffset = offset1;
    e->m_Opacity = offset2;
}

/* CEmitter__Advance, inlined: returns 1 once the emitter has wound down. */
static __inline__ s32 UpdateEmitter(CEmitter *e, s32 mask, f32 fallback, s32 boosting, f32 boost, CCamera *camera) {
    CEmitterDef *def = e->m_pDef;
    f32 x;
    f32 step;
    f32 t;
    f32 wrap;

    if (e->m_Mask & mask) {
        f32 up = e->m_Throttle;
        up += def->m_ThrottleRate * gFrameIncrement;
        if (1.0f < up) {
            up = 1.0f;
        }
        e->m_Throttle = up;
    } else {
        f32 dn;
        if (e->m_Throttle == 0.0f) {
            return 1;
        }
        dn = e->m_Throttle - def->m_ThrottleRate * gFrameIncrement;
        dn = (dn < 0.0f) ? 0.0f : dn;
        e->m_Throttle = dn;
    }
    e->m_Phase = e->m_Phase + gFrameIncrement * def->m_PhaseRate * e->m_PhaseScale;
    step = gFrameIncrement * D_800B6D34 * def->m_SpinRate;
    t = e->m_ScrollS + step * 1.536f;
    wrap = 20480.0f;
    e->m_ScrollS = t;
    if (wrap < t) {
        e->m_ScrollS = t - wrap;
    }
    t = e->m_ScrollT + step * 3.072f;
    e->m_ScrollT = t;
    if (wrap < t) {
        e->m_ScrollT = t - wrap;
    }
    x = def->m_BaseHeight + e->m_HeightOffset;
    if (boosting != 0 && camera->m_ViewParams.m_nType == 0) {
        x += boost;
    } else {
        u8 b = def->m_Flags;
        if (b & EMITTER_DEF_FLAG_CAMERA) {
            x += camera->m_vPos.y * def->m_CameraScale;
        } else if (b & EMITTER_DEF_FLAG_FLOOR) {
            x += fallback;
        }
    }
    e->m_Height = x;
    return 0;
}

/* Starts the emitters the camera's region asks for (one per bit of its mask,
 * from the table at D_8010FF14) and updates the running ones, giving back
 * those that have wound down. Under a region flagged 1, the emitters get a
 * boost while the camera is below the region's height. */
void CEmitterPool__Advance(CEmitterPool *owner, CCamera *camera) {
    CRegionInfo *info;
    CEmitterDef *def;
    CEmitter *e;
    CEmitter *next;
    s32 region;
    s32 n;
    s32 bit;
    s32 mask;
    s32 below;
    f32 ground;
    f32 boost;
    f32 depth;

    region = (s32)camera->m_ViewParams.m_pCurrentRegion;
    if (region == 0) {
        return;
    }
    info = CScene__GetRegionInfo(gScene, region);
    if (info == NULL) {
        return;
    }
    boost = 0.0f;
    ground = CGameRegion__GetHeight(region, camera->m_ViewParams.m_vPos.x, camera->m_ViewParams.m_vPos.z);
    below = 0;
    if (info->m_dwFlags & 1) {
        depth = MIN((info->m_WaterHeight - CCamera__GetViewOrigin(camera).y) * 0.5f, 20.48f);
        below = CCamera__GetViewOrigin(camera).y < info->m_WaterHeight;
        boost = info->m_WaterHeight - depth;
    }
    def = D_8010FF14;
    mask = info->m_EmitterMask;
    bit = 1;
    n = D_8010FF18;
    while (--n != -1) {
        if ((mask & bit) && def->m_Texture != 0 && !(owner->m_ActiveMask & bit)) {
            e = TakeEmitter(owner);
            if (e != NULL) {
                InitEmitter(e, def, bit, 1.0f, 1.0f, 0.0f, 1.0f);
                owner->m_ActiveMask |= bit;
            }
        }
        def++;
        bit <<= 1;
    }
    e = owner->m_ActiveList.m_pHead;
    while (e != NULL) {
        next = e->m_pNext;
        if (UpdateEmitter(e, mask, ground, below, boost, camera)) {
            owner->m_ActiveMask &= ~e->m_Mask;
            FreeEmitter(owner, e);
        }
        e = next;
    }
}
