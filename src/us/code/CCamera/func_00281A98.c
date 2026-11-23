#include "common.h"

/* The port: a real CCamera pointer (64-bit hosts: no truncation) and the sky system at its host
 * offset (structs/camera.h). */
#include "structs/camera.h"

void CEmitterPool__Construct(void *pool);

void CCamera__ConstructSkySystem(CCamera *cam) {
    CEmitterPool__Construct(cam->m_SkySystem);
}
