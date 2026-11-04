#include "common.h"
#include "structs/player.h"

extern void func_002117DC(f32 *value, f32 target, f32 rate);
extern void func_0042E29C(CPlayer *player);

void func_00255FF4(CPlayer *player) {
    if (player->m_Mode == 29) {
        player->m_CharacterActor.m_vVelocity.y = -307.19998f;
    }
    if (player->m_DoDeathCin == 0) {
        if (!(player->m_ModeFlags & 0x8000)) {
            func_002117DC(&player->m_vHeadRotOffset.z, 1.308997f, 0.25f);
        }
        if (player->m_ModeTime > 37.5f) {
            func_0042E29C(player);
        }
    }
}
