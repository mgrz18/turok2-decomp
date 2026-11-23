#include "common.h"
#include "structs/vector.h"
#include "globals/scene.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

typedef struct ObjDef {
    u16 pad0;
    u16 flags;                  /* 0x02 */
} ObjDef;

typedef struct ShadowActor {
    u8 pad0[0x4];
    CVector3 pos;                   /* 0x04 */
    ObjDef *def;                /* 0x10 */
    u8 pad14[0x40 - 0x14];
    u8 rot[0x54 - 0x40];        /* 0x40 */
    f32 unk54;                  /* 0x54 */
    u8 pad58[0x10E - 0x58];
    u8 shadowAlpha;             /* 0x10E */
} ShadowActor;


s32 CGameRegion__IsSteep(ObjDef *def);
void *MtxPool_Alloc(void *pool, s32 count);
f32 GameObject_GetGroundHeight(ShadowActor *actor, s32 arg1);    /* ground height */
f32 GameObject_GetWallRadius(ShadowActor *actor);              /* radius */
void CVector3__ClampLength(CVector3 *v, f32 arg1);
void CQuaternion__ToMatrix(void *rot, CMtxF *m);
void CMtxF__ScaleAxes(CMtxF *m, f32 x, f32 y, f32 z);  /* scale */
void CMtxF__Translate(CMtxF *m, f32 x, f32 y, f32 z);  /* translate */
void CMtxF__ToMtxAffine(CMtxF *m, void *dst);
void Render_DrawShadowQuad(void *mtx, s32 arg1, s32 alpha);

/* Draws the blob shadow under an actor. */
#ifdef TARGET_PORT
#include "structs/game_object.h"
/* The port (round 11 z1): the actor by member of CGameObjectInstance (the N64 view: 0x04 m_vPos,
 * 0x10 the region (its flags at 0x02), 0x40 m_qGround, 0x54 m_CollisionYOffset, 0x10E
 * m_ShadowAlpha). Through the N64 offsets the alpha's read-modify-write landed in byte 2 of
 * m_CurrentAnimation.pmeAnimData: the first-person weapon's blob shadow (CPlayerPool__ReloadAnimInfo)
 * turned a released animation (NULL) into 0x630000 and the next weapon animation switch copied it
 * into the blend channel: CAnimationState__FindKeys crashed (level 135 of the River of Souls). */
void Shadow_DrawBlob(ShadowActor *actor_, CVector3 *offset, s32 arg2) {
    CGameObjectInstance *actor = (CGameObjectInstance *) actor_;
    ObjDef *def = (ObjDef *) actor->header.m_pCurrentRegion;
    CMtxF mtx;
    CVector3 pos;
    void *gmtx;
    s32 visible;
    f32 ground;
    f32 height;
    f32 size;
    f32 lift;
    f32 a;
    u8 alpha;

    visible = !CGameRegion__IsSteep(def);
    if (def != NULL && (def->flags & 0x4000)) {
        visible = 0;
    }
    if (!visible) {
        return;
    }
    gmtx = MtxPool_Alloc(gScene.m_Heap, 1);
    if (gmtx == NULL) {
        return;
    }
    ground = GameObject_GetGroundHeight(actor_, 0);
    height = actor->header.m_vPos.y + actor->m_CollisionYOffset - ground;
    size = MAX(MIN(GameObject_GetWallRadius(actor_), GameObject_GetWallRadius(actor_) - height * 0.1f), 0.0f);
    if (arg2) {
        lift = 15.36f;
        size *= 0.5f;
    } else {
        lift = 0.0f;
        size *= 0.7f;
    }
    pos.x = actor->header.m_vPos.x + offset->x;
    pos.z = actor->header.m_vPos.z + offset->z;
    pos.y = ground + lift + offset->y;
    CVector3__ClampLength(&pos, 20000.0f);
    CQuaternion__ToMatrix(&actor->m_qGround, &mtx);
    CMtxF__ScaleAxes(&mtx, size, 1.0f, size);
    CMtxF__Translate(&mtx, pos.x, pos.y, pos.z);
    CMtxF__ToMtxAffine(&mtx, gmtx);
    a = 200.0f - height * 1.2207031f;
    alpha = a > 255.0f ? 255 : a < 0.0f ? 0 : a > 255.0f ? 255 : (u8)a;
    actor->m_ShadowAlpha = actor->m_ShadowAlpha + (alpha - actor->m_ShadowAlpha) * 0.5f;
    Render_DrawShadowQuad(gmtx, arg2 == 0, actor->m_ShadowAlpha);
}
#else
void Shadow_DrawBlob(ShadowActor *actor, CVector3 *offset, s32 arg2) {
    CMtxF mtx;
    CVector3 pos;
    void *gmtx;
    s32 visible;
    f32 ground;
    f32 height;
    f32 size;
    f32 lift;
    f32 a;
    u8 alpha;

    visible = !CGameRegion__IsSteep(actor->def);
    if (actor->def != NULL && (actor->def->flags & 0x4000)) {
        visible = 0;
    }
    if (!visible) {
        return;
    }
    gmtx = MtxPool_Alloc(gScene.m_Heap, 1);
    if (gmtx == NULL) {
        return;
    }
    ground = GameObject_GetGroundHeight(actor, 0);
    height = actor->pos.y + actor->unk54 - ground;
    size = MAX(MIN(GameObject_GetWallRadius(actor), GameObject_GetWallRadius(actor) - height * 0.1f), 0.0f);
    if (arg2) {
        lift = 15.36f;
        size *= 0.5f;
    } else {
        lift = 0.0f;
        size *= 0.7f;
    }
    pos.x = actor->pos.x + offset->x;
    pos.z = actor->pos.z + offset->z;
    pos.y = ground + lift + offset->y;
    CVector3__ClampLength(&pos, 20000.0f);
    CQuaternion__ToMatrix(actor->rot, &mtx);
    CMtxF__ScaleAxes(&mtx, size, 1.0f, size);
    CMtxF__Translate(&mtx, pos.x, pos.y, pos.z);
    CMtxF__ToMtxAffine(&mtx, gmtx);
    a = 200.0f - height * 1.2207031f;
    alpha = a > 255.0f ? 255 : a < 0.0f ? 0 : a > 255.0f ? 255 : (u8)a;
    actor->shadowAlpha = actor->shadowAlpha + (alpha - actor->shadowAlpha) * 0.5f;
    Render_DrawShadowQuad(gmtx, arg2 == 0, actor->shadowAlpha);
}
#endif
