#include "common.h"

#ifdef TARGET_PORT
/* The port: a real CCamera pointer (64-bit hosts: no truncation) and the sky system at its host
 * offset (structs/camera.h). */
#include "structs/camera.h"

void CEmitterPool__Construct(void *pool);

void CCamera__ConstructSkySystem(CCamera *cam) {
    CEmitterPool__Construct(cam->m_SkySystem);
}
#else
s32 CEmitterPool__Construct(s32);                         /* extern */

void CCamera__ConstructSkySystem(sptr arg0) {
    CEmitterPool__Construct(arg0 + 0x558);
}
#endif
