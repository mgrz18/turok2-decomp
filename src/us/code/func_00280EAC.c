#include "common.h"
#include "structs/camera.h"

/* Resets the under-water wobble of the view: still, at its rest scale and
 * speeds (the speeds are angles a frame: pi/2 and 75 degrees). */
void func_00280EAC(CCamera *cam) {
    cam->m_WaterWobbleXAngle = 0.0f;
    cam->m_WaterWobbleYAngle = 0.0f;
    cam->m_WaterWobbleXScale = 0.2f;
    cam->m_WaterWobbleYScale = 0.1f;
    cam->m_WaterWobbleXSpeed = 1.5707965f;     /* 0x3FC90FDC: a hair over the float nearest pi/2 */
    cam->m_WaterWobbleYSpeed = 1.3089970f;
}
