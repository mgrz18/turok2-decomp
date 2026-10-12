#include "common.h"
#include "m2c_macros.h"
#include "structs/camera.h"

void func_002810C4(CCamera *arg0, s8 *arg1, s8 *arg2, s8 *arg3) {
    *arg1 = arg0->m_FogColor[0] & 0xF8;
    *arg2 = arg0->m_FogColor[1] & 0xF8;
    *arg3 = arg0->m_FogColor[2] & 0xF8;
}
