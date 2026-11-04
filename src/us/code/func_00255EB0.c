#include "common.h"
#include "structs/player.h"

s32 func_0042EB04();                            /* extern */

void func_00255EB0(CPlayer *arg0) {
    arg0->m_WaitDeathmatchTimer = 0;
    func_0042EB04();
    arg0->m_CharacterActor.m_vVelocity.x = 0;
    arg0->m_CharacterActor.m_vVelocity.y = 0;
    arg0->m_CharacterActor.m_vVelocity.z = 0;
    arg0->m_ShockedTimer = 0;
    arg0->m_CharAnim = 1;
    arg0->m_CharacterActor.m_dwFlags = arg0->m_CharacterActor.m_dwFlags & 0xFF7FFFFF;
}
