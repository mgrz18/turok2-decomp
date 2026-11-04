#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"


s16 *func_0025398C(CPlayer *player, s32 weapon);
s32 func_0024E700(CPlayer *player, s32 weapon);
void func_00243414(CGameObjectInstance *actor, void *state, s32 mode);

void func_00259D38(CGameObjectInstance *actor, void *state) {
    CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);
    s16 *ammo = &player->m_Data.m_AmmoList[*func_0025398C(player, 0x10)];

    if (*ammo > 0) {
        (*ammo)--;
    }
    if (func_0024E700(player, 0x10) == 0) {
        func_00243414(actor, state, 7);
    } else {
        func_00243414(actor, state, 4);
    }
}
