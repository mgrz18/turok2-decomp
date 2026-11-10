#include "common.h"
#include "structs/player.h"

s32 CPlayer__StopLoopingSounds();                            /* extern */

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__StopLoopingSounds receives PlayerDeathWait_Enter's own argument
 * registers (pass-through); the host passes them. */
void PlayerDeathWait_Enter(CPlayer *arg0) {
    arg0->m_WaitDeathmatchTimer = 0;
    CPlayer__StopLoopingSounds(arg0);
    arg0->m_CharacterActor.m_vVelocity.x = 0;
    arg0->m_CharacterActor.m_vVelocity.y = 0;
    arg0->m_CharacterActor.m_vVelocity.z = 0;
    arg0->m_ShockedTimer = 0;
    arg0->m_CharAnim = 1;
    arg0->m_CharacterActor.m_dwFlags = arg0->m_CharacterActor.m_dwFlags & 0xFF7FFFFF;
}
#else
void PlayerDeathWait_Enter(CPlayer *arg0) {
    arg0->m_WaitDeathmatchTimer = 0;
    CPlayer__StopLoopingSounds();
    arg0->m_CharacterActor.m_vVelocity.x = 0;
    arg0->m_CharacterActor.m_vVelocity.y = 0;
    arg0->m_CharacterActor.m_vVelocity.z = 0;
    arg0->m_ShockedTimer = 0;
    arg0->m_CharAnim = 1;
    arg0->m_CharacterActor.m_dwFlags = arg0->m_CharacterActor.m_dwFlags & 0xFF7FFFFF;
}
#endif
