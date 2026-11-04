#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x996];
    /* 0x996 */ s16 weapon;
    /* 0x998 */ u8 pad998[0xA64 - 0x998];
    /* 0xA64 */ s32 flags;
} Player;


s32 func_0024E700(Player *player, s32 weapon);
void func_00243414(CGameObjectInstance *actor, void *state, s32 mode);

void func_00259630(CGameObjectInstance *actor, void *state) {
    Player *player = ((Player *)actor->m_AI.m_pParent);

    if ((player->flags & 0x2000) && func_0024E700(player, player->weapon)) {
        func_00243414(actor, state, 4);
    } else if (!(player->flags & 0x10)) {
        func_00243414(actor, state, 2);
    }
}
