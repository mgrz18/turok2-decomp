#include "common.h"
#include "structs/player.h"

#define P_F32(p, off) (*(f32 *)((u8 *)(p) + (off)))

extern f32 func_00268A5C(CGameObjectInstance *actor);
extern void func_002117DC(f32 *value, f32 target, f32 rate);

/* Same body as func_002559E0. */
void func_00257720(CPlayer *player, CGameObjectInstance *actor) {
    if (!(actor->m_AI.m_dwFlags & 0xC0000)) {
        actor->m_vVelocity.y = 0.0f;
        func_002117DC(&P_F32(player, 0xAA4), func_00268A5C(actor) - P_F32(player, 0xAE4) * 0.6f, 0.5f);
    }
}
