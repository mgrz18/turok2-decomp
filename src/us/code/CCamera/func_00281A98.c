#include "common.h"

/* The port: a real CCamera pointer (64-bit hosts: no truncation) and the sky system at its host
 * offset (structs/camera.h). */
#include "structs/camera.h"

void CEmitterPool__Construct(struct CEmitterPool *);

void CCamera__ConstructSkySystem(CCamera *cam) {
    CEmitterPool__Construct((struct CEmitterPool *) cam->m_SkySystem);
}
