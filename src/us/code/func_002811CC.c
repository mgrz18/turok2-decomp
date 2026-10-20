#include "common.h"
#include "structs/camera.h"

/* Sets the camera's own shake source, as func_0028135C sets one of the
 * pool's: position, radius, wave type and a tenth of each amplitude. */
void func_002811CC(CCamera *cam, f32 ampX, f32 ampY, f32 ampZ, f32 radius, s32 type, CVector3 pos) {
    u8 pad[0x18];       /* unused stack, as in func_0028135C */
    CVector3 at;

    at = pos;
    cam->m_Shake.m_vPos = at;
    cam->m_Shake.m_Radius = radius;
    cam->m_Shake.m_XAxis.m_Type = type;
    cam->m_Shake.m_YAxis.m_Type = type;
    cam->m_Shake.m_ZAxis.m_Type = type;
    cam->m_Shake.m_XAxis.m_Shake = ampX * 0.1f;
    cam->m_Shake.m_YAxis.m_Shake = ampY * 0.1f;
    cam->m_Shake.m_ZAxis.m_Shake = ampZ * 0.1f;
}
