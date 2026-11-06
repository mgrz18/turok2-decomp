#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"
#include "structs/weapon.h"


typedef struct {
    /* 0x000 */ u8 pad0[0x110];
    /* 0x110 */ f32 unk110;
    /* 0x114 */ u8 pad114[4];
    /* 0x118 */ f32 unk118;
} State;

void CPlayer__StartWeaponBackgroundSound(CPlayer *player, s32 id);
void CPlayer__StopWeaponBackgroundSound(CPlayer *player);
void CAI__SetMode(CGameObjectInstance *actor, State *state, s32 mode);

void WeaponMode_AE794_Update(CGameObjectInstance *actor, State *state) {
    CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);

    if (player->m_pLoopingSound_WeaponBackground == 0) {
        CPlayer__StartWeaponBackgroundSound(player, 0x464);
    }
    if (!(player->m_ContState.m_dwButtons & 0x2000)) {
        CPlayer__StopWeaponBackgroundSound(player);
        if (state->unk110 < 0.27925270795822144f /* 16 deg */ || state->unk118 > 0.0f) {
            CAI__SetMode(actor, state, 2);
        } else {
            state->unk118 = D_800AF618[22]->fireDelay * 15.0f;
            CAI__SetMode(actor, state, 5);
        }
    }
}
