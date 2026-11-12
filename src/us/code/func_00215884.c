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

#ifdef TARGET_PORT
typedef struct Func00215884Obj {
/* the view at the host offsets of CLevelActor (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Func00215884Obj(m) __builtin_offsetof(CLevelActor, m)
    union {
        struct { u8 _hp_pos[T2_HOFF_Func00215884Obj(header.m_vPos)]; f32 pos[3]; }; /* N64 0x4 */
        struct { u8 _hp_def[T2_HOFF_Func00215884Obj(header.m_pIntelligence)]; CIntelligence *def; }; /* N64 0x14 */
        struct { u8 _hp_mtx[T2_HOFF_Func00215884Obj(m_Matrices)]; CMtxF mtx[2]; }; /* N64 0x38 */
        struct { u8 _hp_angle[T2_HOFF_Func00215884Obj(m_RotY)]; f32 angle; }; /* N64 0x144 */
        struct { u8 _hp_scale[T2_HOFF_Func00215884Obj(m_ModelScale)]; f32 scale; }; /* N64 0x164 */
        struct { u8 _hp_yOffset[T2_HOFF_Func00215884Obj(m_Height)]; f32 yOffset; }; /* N64 0x168 */
    };
} Func00215884Obj;
#else
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
#endif

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
