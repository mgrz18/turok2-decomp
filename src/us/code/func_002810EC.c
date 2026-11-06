/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/CAMERA/80233920_18.CPP,
 * func_80239244_de, with Turok 2's CCamera (its fields sit elsewhere). */
#include "common.h"
#include "structs/camera.h"

extern s32 gScreenWidth;          /* screen width */
extern s32 gScreenHeight;          /* screen height */
void CMtxF__TransformPointProjective(CMtxF *m, CVector3 *in, CVector3 *out);   /* transform a point */

/* Where a point of the world falls on the screen, through the camera's
 * projection. */
void CCamera__WorldToScreen(CCamera *cam, CVector3 *point, f32 *x, f32 *y) {
    CVector3 value;
    f32 half;

    CMtxF__TransformPointProjective(&cam->m_mfProjection, point, &value);
    half = (f32) (gScreenWidth / 2);
    *x = value.x * half + half;
    *y = value.y * (f32) (-gScreenHeight / 2) + (f32) (gScreenHeight / 2);
}
