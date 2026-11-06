#include "common.h"
#include "structs/camera.h"

s32 Frustum_ContainsPoint(CPlane *planes, CVector3 *point);

/* Whether a point is inside the camera's view volume (its six planes). The
 * point comes by value. */
s32 CCamera__IsPointVisible(CCamera *cam, CVector3 point) {
    return Frustum_ContainsPoint(cam->m_ViewVolume, &point);
}
