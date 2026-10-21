/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/CAMERA/80233920_18.CPP,
 * func_80239244_de, with Turok 2's CCamera (its fields sit elsewhere). */
#include "common.h"
#include "structs/camera.h"

extern s32 D_800C2030;          /* screen width */
extern s32 D_800C2034;          /* screen height */
void func_0020F9D4(CMtxF *m, CVector3 *in, CVector3 *out);   /* transform a point */

/* Where a point of the world falls on the screen, through the camera's
 * projection. */
void func_002810EC(CCamera *cam, CVector3 *point, f32 *x, f32 *y) {
    CVector3 value;
    f32 half;

    func_0020F9D4(&cam->m_mfProjection, point, &value);
    half = (f32) (D_800C2030 / 2);
    *x = value.x * half + half;
    *y = value.y * (f32) (-D_800C2034 / 2) + (f32) (D_800C2034 / 2);
}
