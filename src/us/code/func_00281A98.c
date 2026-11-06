#include "common.h"

s32 CEmitterPool__Construct(s32);                         /* extern */

void CCamera__ConstructSkySystem(s32 arg0) {
    CEmitterPool__Construct(arg0 + 0x558);
}
