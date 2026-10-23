#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ f32 unk18;
} WeaponInfo;

typedef struct {
    /* 0x000 */ u8 pad0[0x996];
    /* 0x996 */ s16 weapon;
} Player;

typedef struct {
    /* 0x000 */ u8 pad0[0x1A8];
    /* 0x1A8 */ Player *owner;
} Actor;

typedef struct {
    /* 0x000 */ u8 pad0[0x64];
    /* 0x064 */ s32 unk64;
    /* 0x068 */ u8 pad68[0x118 - 0x68];
    /* 0x118 */ f32 unk118;
} State;

extern WeaponInfo *D_800AF618[];
void func_00253EB8(Player *player, s32 id);

void func_00259794(Actor *actor, State *state) {
    Player *player = actor->owner;

    state->unk64 = 0;
    state->unk118 = D_800AF618[actor->owner->weapon]->unk18 * 15.0f;
    func_00253EB8(player, 0x1A9);
}
