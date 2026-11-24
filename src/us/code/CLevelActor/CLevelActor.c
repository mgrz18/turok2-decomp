/* WRONG CUT: splat cut a symbol at 0x2150B8 (500 bytes): its first 8 bytes are junk fill
 * ("ICKUP_SA" text), the real function starts at the 16-aligned 0x2150C0 (ROM symbol CLevelActor__Construct),
 * 492 bytes; CScene__DecompressGridSection calls it for the level file's third table.
 * Sets up a placed level actor from its 0x20-byte record: position, bounds (position +- radius),
 * model, segment, angle (s8 in 1/254 turns), flags (always with bit 1), default 24-byte state. */

#include "common.h"
#include "structs/vector.h"
#include "structs/level_actor.h"
#ifdef TARGET_PORT
#include "structs/region.h"
#endif
#include "structs/light.h"
#include "globals/scene.h"
#include "globals/engine.h"
#include "protos/scene.h"

/* A placed actor's record in the level data. */
typedef struct {
    /* 0x00 */ s32 value;
    /* 0x04 */ CVector3 pos;
    /* 0x10 */ f32 radius;
    /* 0x14 */ u16 id;
    /* 0x16 */ u16 segment; /* 0xFFFF = none */
    /* 0x18 */ u16 flags;
    /* 0x1A */ u16 unk1A;
    /* 0x1C */ u16 model;   /* 0xFFFF = none */
    /* 0x1E */ s8 angle;
} PlacedActorRecord;
f32 GameObject_GetCollisionHeight(CLevelActor *actor);

void CLevelActor__Construct(CLevelActor *actor, PlacedActorRecord *record, s32 owner, u8 *segments) {
    CVector3 top;

    actor->header.m_Type = 3;
    actor->header.m_nObjType = record->id;
    actor->m_ViewMask = record->value;
    actor->header.m_vPos = record->pos;
    actor->m_RotY = record->angle * 0.02473695f;
    actor->m_ModelScale = 0.35f;
    actor->m_Height = 0;
    actor->m_Bounds.m_vMin.x = record->pos.x - record->radius;
    actor->m_Bounds.m_vMin.y = record->pos.y - record->radius;
    actor->m_Bounds.m_vMin.z = record->pos.z - record->radius;
    actor->m_Bounds.m_vMax.x = record->pos.x + record->radius;
    actor->m_Bounds.m_vMax.y = record->pos.y + record->radius;
    actor->m_Bounds.m_vMax.z = record->pos.z + record->radius;
    actor->m_Resource = owner;
    if (record->model == 0xFFFF) {
        actor->header.m_pIntelligence = (CIntelligence *) &gLevelActorIntelligence;
    } else {
        actor->header.m_pIntelligence = CScene__GetIntelligence(&gScene, record->model);
    }
    if (record->segment == 0xFFFF) {
        actor->header.m_pCurrentRegion = NULL;
    } else {
#ifdef TARGET_PORT /* the region array at the host CGameRegion stride (0x40; 32 on the N64), as the static
                    * objects' (func_00214564.c, round 11 z5): at 32 a placed actor's region was a wrong address */
        actor->header.m_pCurrentRegion = (struct CGameRegion *) segments + record->segment;
#else
        actor->header.m_pCurrentRegion = (struct CGameRegion *) (segments + record->segment * 32);
#endif
    }
    actor->m_wFlags = record->flags | 2;
    actor->m_Timer = 0;
    actor->m_AnimSpeed = 0;
    actor->m_nKilledIndex = record->unk1A;
    top.x = actor->header.m_vPos.x;
    top.y = actor->header.m_vPos.y + GameObject_GetCollisionHeight(actor) * 0.5f;
    top.z = actor->header.m_vPos.z;
    actor->m_Lights = gSceneLights;
    actor->m_DrawnFrame = actor->m_UpdatedFrame = (u8)gFrameCount - 1;
}

#include "structs/intelligence.h"
#include "globals/game.h"
#include "protos/cache.h"
#include "protos/iset.h"

s32 *CLevelActor__LoadGeometry(void *obj);
void CQuaternion__ToMatrix(void *, void *);
void CMtxF__SetRotateY(void *, f32);
void CMtxF__ScaleAxes(void *, f32, f32, f32);
s32 CVector3__ClampLength(void *, f32);
void CMtxF__Translate(void *, f32, f32, f32);
void CMtxF__SwapYZRows(void *);
void CMtxF__ToMtxAffine(void *, void *);
void Model_AdvanceMorph(sptr *, f32); /* the model handle: a host pointer (--high-memory) */
void Render_QueueModelInstance(s32 *, f32 *, s32, void *, sptr, s32, s32);
#ifdef TARGET_PORT
#define getObjectTypeID CScene__GetObjectTypeID /* ABI: returns u16 (the host leaves the upper bits of the return register undefined) */
#else
/* The ROM's caller takes the u16 result as an s32 (no andi): called through a type of its own. */
#define getObjectTypeID ((s32 (*)(struct CScene *, s32))CScene__GetObjectTypeID)
#endif

void CLevelActor__Draw(CLevelActor *obj) {
    sptr *geometry;
    f32 matrix[16];
    s32 variant;
    s32 index;

    geometry = CLevelActor__LoadGeometry(obj);
    if (geometry == NULL) {
        return;
    }
    if (obj->m_DrawnFrame != (u8)gFrameCount) {
        if (obj->header.m_pIntelligence->m_Type == 8) {
            CQuaternion__ToMatrix(&gScene.m_qSky, matrix);
        } else {
            CMtxF__SetRotateY(matrix, obj->m_RotY);
        }
        CMtxF__ScaleAxes(matrix, obj->m_ModelScale, obj->m_ModelScale, obj->m_ModelScale);
        CVector3__ClampLength(&obj->header.m_vPos, 20000.0f);
        CMtxF__Translate(matrix, obj->header.m_vPos.x, obj->header.m_vPos.y + obj->m_Height, obj->header.m_vPos.z);
        CMtxF__SwapYZRows(matrix);
        CMtxF__ToMtxAffine(matrix, obj->m_Matrices[gEvenOdd]);
    }
    if (obj->header.m_pIntelligence->m_Type == 12) {
        if (obj->m_DrawnFrame != (u8)gFrameCount) {
            Model_AdvanceMorph(geometry, obj->m_AnimSpeed);
        }
        Render_QueueModelInstance(geometry, obj->m_Matrices[gEvenOdd], 0, &obj->m_Lights, 0, gEvenOdd, obj->header.m_pIntelligence->m_nTexture);
    } else {
        variant = obj->header.m_pIntelligence->m_nModel;
        index = getObjectTypeID(&gScene, obj->header.m_nObjType);
        if (index < 0x6AB) {
            if (index >= 0x6A9) {
                if ((gEngineApp_m_Settings.cheatFlags & 0x800) && *(s32 *)CISet__GetBlock((void *)*geometry, 2) >= 2) {
                    variant = 1;
                }
                if ((gEngineApp_m_Settings.cheatFlags & 0x1000) && *(s32 *)CISet__GetBlock((void *)*geometry, 2) >= 3) {
                    variant = 2;
                }
            }
        }
        Render_QueueModelInstance(geometry, obj->m_Matrices[gEvenOdd], 0, &obj->m_Lights, 0, variant, obj->header.m_pIntelligence->m_nTexture);
    }
    CCache__Unlock(0, geometry);
    obj->m_DrawnFrame = (u8)gFrameCount;
}

#define MIN_LT(a, b) ((a) < (b) ? (a) : (b))

typedef struct {
    /* 0x00 */ u8 pad00[4];
    /* 0x04 */ f32 x;
    /* 0x08 */ f32 y;
    /* 0x0C */ f32 z;
    /* 0x10 */ s32 region;
    /* 0x14 */ u8 pad14[0xA4];
    /* 0xB8 */ f32 shadowMatrices[4][16];
} ShadowActor;
f32 CGameRegion__GetHeight(sptr, f32, f32);
f32 GameObject_GetWallRadius(ShadowActor *);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar): the struct result through an out pointer */
void GameObject_GetGroundRotation_sret(CQuaternion *out, void *obj);
#endif

CQuaternion GameObject_GetGroundRotation(void *obj);
void Render_DrawShadowQuad(f32 *, s32, s32);

/* Places the actor's ground shadow: scaled by its radius, shrinking with its height above the ground. */
void Actor_DrawGroundShadow(ShadowActor *actor) {
    f32 matrix[16];
    CVector3 position;
    CQuaternion rotation;
    f32 ground;
    f32 height;
    f32 size;

    ground = CGameRegion__GetHeight(actor->region, actor->x, actor->z);
    height = actor->y - ground;
    if (!(MIN_LT(GameObject_GetWallRadius(actor) * 0.5f, GameObject_GetWallRadius(actor) * 0.5f - height * 0.1f) < 0.0f)) {
        size = MIN_LT(GameObject_GetWallRadius(actor) * 0.5f, GameObject_GetWallRadius(actor) * 0.5f - height * 0.1f);
    } else {
        size = 0.0f;
    }
#ifdef TARGET_PORT
    GameObject_GetGroundRotation_sret(&rotation, actor);
#else
    rotation = GameObject_GetGroundRotation(actor);
#endif
    position.x = actor->x;
    position.z = actor->z;
    position.y = ground;
    CVector3__ClampLength(&position, 20000.0f);
    CQuaternion__ToMatrix(&rotation, matrix);
    CMtxF__ScaleAxes(matrix, size, 1.0f, size);
    CMtxF__Translate(matrix, position.x, position.y, position.z);
    CMtxF__ToMtxAffine(matrix, actor->shadowMatrices[gEvenOdd]);
    Render_DrawShadowQuad(actor->shadowMatrices[gEvenOdd], 1, 0x96);
}

CQuaternion Actor_GetGroundRotation(void *obj) {
    return GameObject_GetGroundRotation(obj);
}

/* CLevelActor__LoadGeometry and CLevelActor__TouchGeometry: DEFERRED (output after the file's
 * other functions, as the ROM's .text has them) but defined here, where the ROM's .rodata has
 * their strings: between Actor_DrawGroundShadow's literals and CLevelActor__Update's. */
typedef CLevelActor ResourceOwner;
#define LEVEL_ACTOR_KEY(obj) (((CLevelActor *) (obj))->m_Resource)

void Model_TouchTextures(s32 *);
extern void Model_DecompressCallbackTerminated();

/* Loads a simple object geometry: reads its index, then the geometry block it points at. */
DEFERRED s32 *CLevelActor__LoadGeometry(void *obj) {
    sptr *index;
    s32 *geometry;
    sptr src;
    s32 size;

    index = CCache__Request(0, LEVEL_ACTOR_KEY(obj), 0x18, 4, 0, NULL, "simp object index", 0);
    if (index == NULL) {
        geometry = NULL;
    } else {
        src = CISet__GetROMAddressAndSize(*index, LEVEL_ACTOR_KEY(obj), 1, &size);
        CCache__Unlock(0, index);
        geometry = CCache__Request(0, src, size, 4, 0, Model_DecompressCallbackTerminated, "simp geometry", 0);
    }
    return geometry;
}

DEFERRED void CLevelActor__TouchGeometry(ResourceOwner *arg0) {
    s32 sp20;
    sptr *temp_v0;
    s32 *var_s0;
    sptr temp_s0;

    temp_v0 = CCache__Request(0, arg0->m_Resource, 0x18, 4, 0, NULL, "simp object index", 0);
    if (temp_v0 != NULL) {
        temp_s0 = CISet__GetROMAddressAndSize(*temp_v0, arg0->m_Resource, 1, &sp20);
        CCache__Unlock(0, temp_v0);
        var_s0 = CCache__Request(0, temp_s0, sp20, 4, 0, Model_DecompressCallbackTerminated, "simp geometry", 0);
    } else {
        var_s0 = NULL;
    }
    if (var_s0 != NULL) {
        Model_TouchTextures(var_s0);
        CCache__Unlock(0, var_s0);
    }
}

#include "structs/region.h"
#include "globals/camera.h"

/* Once-per-frame update. */
void CLevelActor__Update(CLevelActor *obj) {
    CIntelligence *def;
    f32 timer;

    if (obj->m_UpdatedFrame == (u8)gFrameCount || gEngineApp_m_Settings.paused != 0) {
        return;
    }
    def = obj->header.m_pIntelligence;
    if (def->m_Type == 12) {
        obj->m_AnimSpeed = gFrameIncrement * def->m_Morpher.m_AnimSpeed;
    } else if (obj->m_wFlags & 0x10) {
        timer = obj->m_Timer + gFrameIncrement * (1.0f / 15.0f);
        obj->m_Timer = timer;
        if (timer < 0.75f) {
            obj->m_ModelScale = timer * (0.35f / 0.75f);
        } else {
            obj->m_wFlags &= ~0x10;
            obj->m_ModelScale = 0.35f;
        }
    }
    if (obj->header.m_pCurrentRegion != NULL) {
        obj->m_ViewMask = obj->header.m_pCurrentRegion->m_VisBits;
    } else {
        obj->m_ViewMask = gDefaultVisBits;
    }
    obj->m_UpdatedFrame = (u8)gFrameCount;
}

/* Grow-in effect: ramp the scale up to 0.35 over 0.75 time units. */
void CLevelActor__UpdateGrowIn(CLevelActor *obj) {
    f32 t;

    if (obj->m_wFlags & 0x10) {
        t = obj->m_Timer + gFrameIncrement * 0.06666667f;
        obj->m_Timer = t;
        if (t < 0.75f) {
            obj->m_ModelScale = t * 0.46666667f;
            return;
        }
        obj->m_wFlags &= ~0x10;
        obj->m_ModelScale = 0.35f;
    }
}

void CLevelActor__UpdateAnimSpeed(CLevelActor *arg0) {
    arg0->m_AnimSpeed = (f32) (gFrameIncrement * arg0->header.m_pIntelligence->m_Morpher.m_AnimSpeed);
}

/* Builds the object transform (rotation, uniform scale, translation) into its current matrix slot. */
void CLevelActor__BuildMatrix(CLevelActor *obj) {
    CMtxF mtx;

    if (obj->header.m_pIntelligence->m_Type == 8) {
        CQuaternion__ToMatrix(&gScene.m_qSky, &mtx);
    } else {
        CMtxF__SetRotateY(&mtx, obj->m_RotY);
    }
    CMtxF__ScaleAxes(&mtx, obj->m_ModelScale, obj->m_ModelScale, obj->m_ModelScale);
    CVector3__ClampLength(&obj->header.m_vPos, 20000.0f);
    CMtxF__Translate(&mtx, obj->header.m_vPos.x, obj->header.m_vPos.y + obj->m_Height, obj->header.m_vPos.z);
    CMtxF__SwapYZRows(&mtx);
    CMtxF__ToMtxAffine(&mtx, &obj->m_Matrices[gEvenOdd]);
}
