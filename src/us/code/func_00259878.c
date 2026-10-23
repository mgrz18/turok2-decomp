#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ f32 unk18;
} WeaponInfo;

typedef struct {
    /* 0x0000 */ u8 pad0[0xA64];
    /* 0x0A64 */ s32 flags;
    /* 0x0A68 */ u8 padA68[0x1288 - 0xA68];
    /* 0x1288 */ s32 unk1288;
} Player;

typedef struct {
    /* 0x000 */ u8 pad0[0x1A8];
    /* 0x1A8 */ Player *owner;
} Actor;

typedef struct {
    /* 0x000 */ u8 pad0[0x110];
    /* 0x110 */ f32 unk110;
    /* 0x114 */ u8 pad114[4];
    /* 0x118 */ f32 unk118;
} State;

extern WeaponInfo *D_800AF618[];
void func_00253DE0(Player *player, s32 id);
void func_00253E68(Player *player);
void func_00243414(Actor *actor, State *state, s32 mode);

void func_00259878(Actor *actor, State *state) {
    Player *player = actor->owner;

    if (player->unk1288 == 0) {
        func_00253DE0(player, 0x464);
    }
    if (!(player->flags & 0x2000)) {
        func_00253E68(player);
        if (state->unk110 < 0.27925270795822144f /* 16 deg */ || state->unk118 > 0.0f) {
            func_00243414(actor, state, 2);
        } else {
            state->unk118 = D_800AF618[22]->unk18 * 15.0f;
            func_00243414(actor, state, 5);
        }
    }
}
