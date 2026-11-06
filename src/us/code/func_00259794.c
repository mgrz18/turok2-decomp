#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"
#include "structs/weapon.h"


typedef struct {
    /* 0x000 */ u8 pad0[0x64];
    /* 0x064 */ s32 unk64;
    /* 0x068 */ u8 pad68[0x118 - 0x68];
    /* 0x118 */ f32 unk118;
} State;

void CPlayer__StartWeaponFiringSound(CPlayer *player, s32 id);

void WeaponMode_AE714_Enter(CGameObjectInstance *actor, State *state) {
    CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);

    state->unk64 = 0;
    state->unk118 = D_800AF618[((CPlayer *)actor->m_AI.m_pParent)->m_Data.m_CurrentWeapon]->fireDelay * 15.0f;
    CPlayer__StartWeaponFiringSound(player, 0x1A9);
}
