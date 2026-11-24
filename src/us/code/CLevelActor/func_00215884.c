#include "common.h"
#include "structs/intelligence.h"
#include "structs/vector.h"
#include "structs/level_actor.h"
#include "globals/scene.h"
#include "globals/engine.h"

void CQuaternion__ToMatrix(void *, void *);
void CMtxF__SetRotateY(void *, f32);
void CMtxF__ScaleAxes(void *, f32, f32, f32);
s32 CVector3__ClampLength(void *, f32);
void CMtxF__Translate(void *, f32, f32, f32);
void CMtxF__SwapYZRows(void *);
void CMtxF__ToMtxAffine(void *, void *);

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
