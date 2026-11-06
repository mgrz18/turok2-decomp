#include "common.h"
#include "structs/vector.h"
#include "structs/region.h"
#include "structs/camera.h"
#include "structs/model.h"
#include "globals/game.h"
#include "globals/scene.h"

/* Draw pass for one actor in one view: if the actor is drawable (flags
 * 0x40000 and a draw callback) and active (0x2000), load its "anim object
 * info", decide whether it is in the view (bounding box against the view
 * frustum, or a box grown by 512 units for actors casting shadows,
 * flag 0x10000000), make sure its model is loaded (CGameObjectInstance__BuildBoneMatrices, playing its
 * spawn effect once, flag 0x200) and call its draw callback.
 *
 * The do { } while (0) blocks (macro bodies in the original, most likely)
 * count as loops for GCC's register priorities (refs weighted by loop
 * depth): they put anim before inView and model before view, as in the ROM. */

typedef struct Actor Actor;

struct Actor {
    u8 pad0[0x4];
    CVector3 pos;               /* 0x04 */
    CGameRegion *region;         /* 0x10 */
    u8 pad14[0x40];
    f32 height;             /* 0x54 */
    u8 matrix[0x4C];        /* 0x58 */
    s32 animFile;           /* 0xA4 */
    u8 padA8[0x8];
    s32 animIndex;          /* 0xB0 */
    u8 padB4[0x8];
    f32 boxMin[3];          /* 0xBC */
    f32 boxMax[3];          /* 0xC8 */
    s32 flags;              /* 0xD4 */
    u8 padD8[0x38];
    u8 lights[2][0x18];     /* 0x110 */
    u8 ai[0xF4];            /* 0x140 */
    void (*draw)(Actor *, void *, CDrawArgs *);  /* 0x234 */
    u8 pad238[0xC];
    void (*update)(Actor *, void *);            /* 0x244 */
};

extern char D_800A5AC4[];   /* "anim object info" */
extern s32 gEvenOdd;      /* current view */

void **CCache__Request(s32, s32, s32, s32, s32, s32, void *, s32);
s32 CISet__GetBlock(void *, s32);
void CCache__Unlock(s32, void **);
void CMtxF__TransformBox(void *, s32, f32 *);
s32 Frustum_TestBox(void *, f32 *);
f32 GameObject_GetRegionFloorHeight(Actor *, s32);
void **CGameObjectInstance__LoadGeometry(Actor *);
s32 CGameObjectInstance__BuildBoneMatrices(Actor *, void *, s32);
f32 GameObject_GetCollisionHeight(Actor *);
void CScene__GetLightsAt(void *, CVector3 *, void *);

void CGameObjectInstance__Draw(Actor *actor, CCamera *view) {
    f32 offset[3];
    f32 box[6];
    CVector3 pos;
    CDrawArgs args;
    s32 flags;
    void **anim;
    void **model;
    s32 info;
    s32 inView;
    s32 near;
    f32 y;

    flags = actor->flags;
    if (!(flags & 0x40000) || actor->draw == NULL) {
        return;
    }
    if (!(flags & 0x2000)) {
        actor->flags = flags & ~0x200;
        return;
    }
    do {
        anim = CCache__Request(0, actor->animFile, actor->animIndex, 0, 0, 0, D_800A5AC4, 1);
        if (anim == NULL) {
            return;
        }
    } while (0);
    info = CISet__GetBlock(*anim, 0);
    if (gEngineApp_m_Settings.paused != 0) {
        actor->update(actor, actor->ai);
        CMtxF__TransformBox(actor->matrix, info, actor->boxMin);
    }
    if (!(actor->flags & 0x300000)) {
        inView = 0;
        near = 0;
        if (actor->region == NULL || (view->m_ViewParams.m_VisBits & actor->region->m_VisBits)) {
            if (view->m_ViewMax.x > actor->boxMin[0] && view->m_ViewMin.x < actor->boxMax[0]
                && view->m_ViewMax.z > actor->boxMin[2] && view->m_ViewMin.z < actor->boxMax[2]
                && view->m_ViewMax.y > actor->boxMin[1] && view->m_ViewMin.y < actor->boxMax[1]
                && Frustum_TestBox(view->m_ViewVolume, actor->boxMin)) {
                inView = 1;
                if (actor->region != NULL) {
                    near = 1;
                }
            }
            if (!near && actor->region != NULL && (actor->flags & 0x10000000)) {
                y = GameObject_GetRegionFloorHeight(actor, 0);
                box[0] = actor->boxMin[0] - 512.0f;
                box[1] = y - 0.5f;
                box[2] = actor->boxMin[2] - 512.0f;
                box[3] = actor->boxMax[0] + 512.0f;
                box[5] = actor->boxMax[2] + 512.0f;
                box[4] = y + 0.5f;
                if (box[0] < view->m_ViewMax.x && view->m_ViewMin.x < box[3]
                    && box[2] < view->m_ViewMax.z && view->m_ViewMin.z < box[5]
                    && box[1] < view->m_ViewMax.y && view->m_ViewMin.y < box[4]
                    && Frustum_TestBox(view->m_ViewVolume, box)) {
                set_near:
                    near = 1;
                }
            }
        }
    } else {
        inView = 1;
        goto set_near;
    }
    if (inView || near) {
        do {
            model = CGameObjectInstance__LoadGeometry(actor);
        } while (0);
        if (model != NULL) {
            if (!(actor->flags & 0x200)) {
                if (!CGameObjectInstance__BuildBoneMatrices(actor, *model, info)) {
                    goto free_model;
                }
                if (!(actor->flags & 8)) {
                    pos = actor->pos;
                    pos.y += actor->height;
                    pos.y += GameObject_GetCollisionHeight(actor) * 0.5f;
                    CScene__GetLightsAt(&gScene, &pos, actor->lights[gEvenOdd]);
                }
                actor->flags |= 0x200;
            }
            args.m_bDraw = inView;
            args.m_bShadow = near;
            args.m_pCamera = view;
            args.m_hModel = (s32)model;
            args.m_pOffset = offset;
            args.m_nVariant = 0;
            args.m_nModel = 0;
            args.m_nShadowModel = 0;
            offset[0] = offset[1] = offset[2] = 0.0f;
            actor->draw(actor, actor->ai, &args);
        free_model:
            CCache__Unlock(0, model);
        }
    }
    do {
        CCache__Unlock(0, anim);
    } while (0);
}
