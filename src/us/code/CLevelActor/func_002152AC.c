#include "common.h"
#include "structs/vector.h"
#include "structs/intelligence.h"
#include "structs/level_actor.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "globals/engine.h"
#include "protos/cache.h"
#include "protos/iset.h"

/* Draws a simple object: rebuilds its matrix for this frame's slot once per
 * frame (from a fixed rotation for type 8, else its yaw), then draws its
 * geometry. Type 12 animates by its speed; the others pick a variant, which
 * models 0x6A9..0x6AA override from the game flags (cf. Rage Wars'
 * func_8024F634_de and func_8024F858_de). */

s32 *CLevelActor__LoadGeometry(void *);
void CQuaternion__ToMatrix(void *, void *);
void CMtxF__SetRotateY(void *, f32);
void CMtxF__ScaleAxes(void *, f32, f32, f32);
s32 CVector3__ClampLength(void *, f32);
void CMtxF__Translate(void *, f32, f32, f32);
void CMtxF__SwapYZRows(void *);
void CMtxF__ToMtxAffine(void *, void *);
void Model_AdvanceMorph(sptr *, f32); /* the model handle: a host pointer (--high-memory) */
void Render_QueueModelInstance(s32 *, f32 *, s32, void *, sptr, s32, s32);
#include "protos/scene.h"
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
