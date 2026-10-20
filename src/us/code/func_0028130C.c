#include "common.h"
#include "structs/camera.h"

void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);     /* out = a - b */
f32 func_00298470(f32 x);                                       /* square root */

/* How far a point is from the camera. */
f32 func_0028130C(CCamera *cam, CVector3 *point) {
    CVector3 d;

    func_0020EF2C(&d, &cam->m_vPos, point);
    return func_00298470(d.x * d.x + d.y * d.y + d.z * d.z);
}
