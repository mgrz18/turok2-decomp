/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/CAMERA/80233920_29.CPP,
 * func_80239678_de, with Turok 2's CCameraPool and CCamera. */
#include "common.h"
#include "structs/camera.h"

extern f64 D_800A9C10;          /* 2^32, for an unsigned count */
void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);     /* out = a - b */
f32 func_00298470(f32 x);                                       /* square root */

/* How far a point is from the active cameras, on average. */
f32 func_00281518(CCameraPool *pool, CVector3 *point) {
    CVector3 d;
    f32 sum;
    CCamera *cam;
    f64 n;
    s32 count;

    if (pool->m_ActiveList.m_nCount == 0) {
        return 0.0f;
    }
    cam = pool->m_ActiveList.m_pHead;
    sum = 0.0f;
    if (cam != NULL) {
        do {
            func_0020EF2C(&d, &cam->m_vPos, point);
            sum += func_00298470(d.x * d.x + d.y * d.y + d.z * d.z);
            cam = cam->m_pNext;
        } while (cam != NULL);
    }
    count = pool->m_ActiveList.m_nCount;
    n = (f64) count;
    if (count < 0) {
        n += D_800A9C10;
    }
    return sum / (f32) n;
}
