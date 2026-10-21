/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/CAMERA/80233920_29.CPP,
 * func_802395A4_de, with Turok 2's CCameraPool and CCamera. */
#include "common.h"
#include "structs/camera.h"

extern f32 D_800A9C08;          /* farther than any camera, squared */
void func_0020F510(f32 *v, f32 limit);
void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);     /* out = a - b */

/* The active camera nearest a point, for its sound; the main camera when
 * the pool says only it is used for sound, or when none is active. */
CCamera *func_00281444(CCameraPool *pool, CVector3 *point) {
    CVector3 d;
    CCamera *cam;
    CCamera *best;
    f32 bestDist;
    f32 dist;

    func_0020F510(&point->x, 20000.0f);
    if (pool->m_bOnlyUseMainCameraForSound != 0) {
        return &pool->m_MainCamera;
    }
    cam = pool->m_ActiveList.m_pHead;
    bestDist = D_800A9C08;
    best = NULL;
    if (cam != NULL) {
        do {
            func_0020EF2C(&d, point, &cam->m_vPos);
            dist = d.x * d.x + d.y * d.y + d.z * d.z;
            if (dist < bestDist) {
                best = cam;
                bestDist = dist;
            }
            cam = cam->m_pNext;
        } while (cam != NULL);
    }
    if (best == NULL) {
        return &pool->m_MainCamera;
    }
    return best;
}
