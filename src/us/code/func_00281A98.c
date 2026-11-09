#include "common.h"
#ifdef TARGET_PORT
#include "structs/camera.h"
#endif

s32 CEmitterPool__Construct(s32);                         /* extern */

void CCamera__ConstructSkySystem(s32 arg0) {
#ifdef TARGET_PORT /* host layout of CCamera (structs/camera.h) */
    CEmitterPool__Construct(arg0 + T2_OFFSETOF(CCamera, m_SkySystem));
#else
    CEmitterPool__Construct(arg0 + 0x558);
#endif
}
