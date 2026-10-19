#include "common.h"
#include "structs/camera.h"

s32 func_0026F480(CPlane *planes, CVector3 *point);

/* Whether a point is inside the camera's view volume (its six planes). The
 * point comes by value. */
s32 func_0028124C(CCamera *cam, CVector3 point) {
    return func_0026F480(cam->m_ViewVolume, &point);
}
