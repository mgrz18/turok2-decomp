#include "common.h"
#include "structs/player.h"

void func_002565C0(CPlayer *arg0) {
    arg0->m_DesiredJumpAngle = 0;
    arg0->m_JumpU = 0;
    arg0->m_StartJumpAngle = (f32) arg0->m_JumpAngle;
}
