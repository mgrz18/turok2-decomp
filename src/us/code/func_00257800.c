#include "common.h"
#include "structs/player.h"
#include "structs/game_object.h"

void func_00257800(CPlayer *arg0) {
    arg0->m_CharacterActor.m_vVelocity.x = 0.0f;
    arg0->m_CharacterActor.m_vVelocity.y = 0.0f;
    arg0->m_CharacterActor.m_vVelocity.z = 0.0f;
    arg0->m_ShockedTimer = 0;
    arg0->m_CharAnim = 1;
    arg0->m_CharacterActor.m_dwFlags &= 0xFF7FFFFF;
}
