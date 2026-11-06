#include "common.h"
#include "structs/player.h"

extern void Math_EaseToward(f32 *value, f32 target, f32 rate);
extern void CPlayer__LoseLife(CPlayer *player);

void CPlayer__UpdateDeath(CPlayer *player) {
    if (player->m_Mode == 29) {
        player->m_CharacterActor.m_vVelocity.y = -307.19998f;
    }
    if (player->m_DoDeathCin == 0) {
        if (!(player->m_ModeFlags & 0x8000)) {
            Math_EaseToward(&player->m_vHeadRotOffset.z, 1.308997f, 0.25f);
        }
        if (player->m_ModeTime > 37.5f) {
            CPlayer__LoseLife(player);
        }
    }
}
