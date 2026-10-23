#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x996];
    /* 0x996 */ s16 weapon;
    /* 0x998 */ u8 pad998[0xA64 - 0x998];
    /* 0xA64 */ s32 flags;
} Player;

typedef struct {
    /* 0x000 */ u8 pad0[0x1A8];
    /* 0x1A8 */ Player *owner;
} Actor;

s32 func_0024E700(Player *player, s32 weapon);
void func_00243414(Actor *actor, void *state, s32 mode);

void func_00259630(Actor *actor, void *state) {
    Player *player = actor->owner;

    if ((player->flags & 0x2000) && func_0024E700(player, player->weapon)) {
        func_00243414(actor, state, 4);
    } else if (!(player->flags & 0x10)) {
        func_00243414(actor, state, 2);
    }
}
