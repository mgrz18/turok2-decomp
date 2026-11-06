#include "common.h"
#include "structs/player.h"

#define P_F32(p, off) (*(f32 *)((u8 *)(p) + (off)))

extern f32 GameObject_GetWaterHeight(CGameObjectInstance *actor);
extern void Math_EaseToward(f32 *value, f32 target, f32 rate);

void PlayerWade_Enter(CPlayer *player, CGameObjectInstance *actor) {
    if (!(actor->m_AI.m_dwFlags & 0xC0000)) {
        actor->m_vVelocity.y = 0.0f;
        Math_EaseToward(&P_F32(player, 0xAA4), GameObject_GetWaterHeight(actor) - P_F32(player, 0xAE4) * 0.6f, 0.5f);
    }
}
