/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0024AA08.c, func_0024AA08, with Turok 2's layouts. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "globals/scene.h"
#include "globals/engine.h"
#include "structs/light.h"
#include "protos/game.h"

/* Draw an actor's shadow. Without flag 0x10000000 it gets the blob shadow
 * (Shadow_DrawBlob). Otherwise its projected shadow model is built once, while
 * above the ground: the current view's light direction (signed bytes) and
 * strength (colour) are sampled at the actor (DLights_GetAtPoint), the alpha is
 * 128 + 56 per unit of light (faded by gBlendFade during a fade), the
 * direction is kept at least 0.15 from horizontal and the model is projected
 * along it onto a plane just under the actor (0.9 of its height, up to 512),
 * grown by 0.003 per unit of height, turned like the actor and set on the
 * ground; then it is submitted (CGameObjectInstance__SubmitShadowModel). */

typedef struct {
    u8 pad0[0x10];
    s32 blobType;           /* 0x10 */
} ShadowParams;

extern u8 D_8010FF34[];

s32 func_00268160(void);
void *MtxPool_Alloc(void *, s32);
f32 func_00298470(f32);     /* sqrtf */
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define DLights_GetAtPoint DLights_GetAtPoint_f3
#endif
void DLights_GetAtPoint(void *, f32, f32, f32, CVector3 *, f32 *);
void CVector3__Normalize(CVector3 *);
void CMtxF__SetShadowProjection(void *, CVector3 *, f32);
void CMtxF__Translate(void *, f32, f32, f32);
void CMtxF__ScaleColumns(void *, f32, f32, f32);
void CQuaternion__ToMatrix(void *, void *);
void CMtxF__MultiplyAffine(void *, void *, void *);
void CGameObjectInstance__SubmitShadowModel(CGameObjectInstance *, ShadowParams *, void *, u8);
void Shadow_DrawBlob(CGameObjectInstance *, s32, s32);

void CGameObjectInstance__DrawShadow(CGameObjectInstance *a, sptr unused, ShadowParams *s) {
    CVector3 dir;
    f32 r;
    u8 m[0x40];
    u8 q[0x40];
    f32 dy;
    f32 h;
    f32 ty;
    f32 t;
    f32 u2;
    s32 v;
    CLights1 *light;
    register f32 py __asm__("$f1");

    if (a->header.m_pCurrentRegion != 0 && func_00268160() != 0 && !(a->m_dwFlags & 0x200000)) {
        if (a->m_dwFlags & 0x10000000) {
            if (a->m_pmfShadow == NULL) {
                h = GameObject_GetRegionFloorHeight(a, 0);
                py = a->header.m_vPos.y;
                if (py < h) {
                    goto end;
                }
                t = py - h;
                dy = t;
                if (512.0f < t) {
                    dy = 512.0f;
                }
                a->m_pmfShadow = MtxPool_Alloc(gScene.m_Heap, 1);
                if (a->m_pmfShadow == NULL) {
                    goto end;
                }
                light = &a->m_Lights[gEvenOdd];
                dir.x = light->m_Dir[0] * 0.007874016f;
                dir.y = light->m_Dir[1] * 0.007874016f;
                dir.z = light->m_Dir[2] * 0.007874016f;
                t = (f32) light->m_Col[0] * (f32) light->m_Col[0];
                u2 = (f32) light->m_Col[1] * (f32) light->m_Col[1];
                t = t + u2;
                r = func_00298470(t + u2) * -0.00090497744f;
                DLights_GetAtPoint(D_8010FF34, a->header.m_vPos.x, a->header.m_vPos.y, a->header.m_vPos.z, &dir, &r);
                if (r < -1.0f) {
                    r = -1.0f;
                }
                v = (s32) (r * 56.0f + 128.0f);
                if (gBlendState == 1) {
                    v = (s32) ((f32) v * gBlendState_m_Color[3] * 0.003921569f);
                }
                a->m_ShadowAlpha = v;
                dir.y = 1.0f;
                CVector3__Normalize(&dir);
                if (dir.y < 0.0f) {
                    if (-dir.y < 0.15f) {
                        goto clampy;
                    }
                } else if (dir.y < 0.15f) {
clampy:
                    if (0.0f <= dir.y) {
                        dir.y = 0.15f;
                    } else {
                        dir.y = -0.15f;
                    }
                    CVector3__Normalize(&dir);
                }
                ty = h + dy * 0.9f;
                CMtxF__SetShadowProjection(m, &dir, ty);
                CMtxF__Translate(m, -a->header.m_vPos.x, -ty, -a->header.m_vPos.z);
                t = dy * 0.003f + 1.0f;
                CMtxF__ScaleColumns(m, t, t, t);
                CQuaternion__ToMatrix(&a->m_qGround, q);
                CMtxF__MultiplyAffine(a->m_pmfShadow, m, q);
                CMtxF__Translate(a->m_pmfShadow, a->header.m_vPos.x, h, a->header.m_vPos.z);
                if (a->m_pmfShadow == NULL) {
                    goto end;
                }
            }
            CGameObjectInstance__SubmitShadowModel(a, s, a->m_pmfShadow, a->m_ShadowAlpha);
        } else {
            Shadow_DrawBlob(a, s->blobType, 0);
        }
    }
end:
    return;
}
