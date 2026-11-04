#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x1A8];
    /* 0x1A8 */ CPlayer *owner;
} Actor;

s32 func_0024E700(CPlayer *player, s32 weapon);
void func_00243414(Actor *actor, void *state, s32 mode);

void func_00259630(Actor *actor, void *state) {
    CPlayer *player = actor->owner;

    if ((player->m_ContState.m_dwButtons & 0x2000) && func_0024E700(player, player->m_Data.m_CurrentWeapon)) {
        func_00243414(actor, state, 4);
    } else if (!(player->m_ContState.m_dwButtons & 0x10)) {
        func_00243414(actor, state, 2);
    }
}
