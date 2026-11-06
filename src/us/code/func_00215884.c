#include "common.h"
#include "structs/intelligence.h"
#include "structs/vector.h"
#include "globals/scene.h"
#include "globals/engine.h"

void CQuaternion__ToMatrix(void *src, CMtxF *dst);
void CMtxF__SetRotateY(CMtxF *mtx, f32 angle);
void CMtxF__ScaleAxes(CMtxF *mtx, f32 x, f32 y, f32 z);
void CVector3__ClampLength(void *pos, f32 value);
void CMtxF__Translate(CMtxF *mtx, f32 x, f32 y, f32 z);
void CMtxF__SwapYZRows(CMtxF *mtx);
void CMtxF__ToMtxAffine(CMtxF *mtx, void *dst);

typedef struct {
    /* 0x000 */ u8 pad000[4];
    /* 0x004 */ f32 pos[3];
    /* 0x010 */ u8 pad010[4];
    /* 0x014 */ CIntelligence *def;
    /* 0x018 */ u8 pad018[0x20];
    /* 0x038 */ CMtxF mtx[2];
    /* 0x0B8 */ u8 pad0B8[0x8C];
    /* 0x144 */ f32 angle;
    /* 0x148 */ u8 pad148[0x1C];
    /* 0x164 */ f32 scale;
    /* 0x168 */ f32 yOffset;
} Func00215884Obj;

/* Builds the object transform (rotation, uniform scale, translation) into its current matrix slot. */
void CLevelActor__BuildMatrix(Func00215884Obj *obj) {
    CMtxF mtx;

    if (obj->def->m_Type == 8) {
        CQuaternion__ToMatrix(&gScene.m_qSky, &mtx);
    } else {
        CMtxF__SetRotateY(&mtx, obj->angle);
    }
    CMtxF__ScaleAxes(&mtx, obj->scale, obj->scale, obj->scale);
    CVector3__ClampLength(obj->pos, 20000.0f);
    CMtxF__Translate(&mtx, obj->pos[0], obj->pos[1] + obj->yOffset, obj->pos[2]);
    CMtxF__SwapYZRows(&mtx);
    CMtxF__ToMtxAffine(&mtx, &obj->mtx[gEvenOdd]);
}
