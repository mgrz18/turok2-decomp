#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x934];
    /* 0x934 */ s16 ammo[1];
} Player;

typedef struct {
    /* 0x000 */ u8 pad0[0x1A8];
    /* 0x1A8 */ Player *owner;
} Actor;

s16 *func_0025398C(Player *player, s32 weapon);
s32 func_0024E700(Player *player, s32 weapon);
void func_00243414(Actor *actor, void *state, s32 mode);

void func_00259D38(Actor *actor, void *state) {
    Player *player = actor->owner;
    s16 *ammo = &player->ammo[*func_0025398C(player, 0x10)];

    if (*ammo > 0) {
        (*ammo)--;
    }
    if (func_0024E700(player, 0x10) == 0) {
        func_00243414(actor, state, 7);
    } else {
        func_00243414(actor, state, 4);
    }
}
