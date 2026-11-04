#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"


s32 func_0024E700(CPlayer *player, s32 weapon);
void func_00243414(CGameObjectInstance *actor, void *state, s32 mode);

void func_00259630(CGameObjectInstance *actor, void *state) {
    CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);

    if ((player->m_ContState.m_dwButtons & 0x2000) && func_0024E700(player, player->m_Data.m_CurrentWeapon)) {
        func_00243414(actor, state, 4);
    } else if (!(player->m_ContState.m_dwButtons & 0x10)) {
        func_00243414(actor, state, 2);
    }
}
