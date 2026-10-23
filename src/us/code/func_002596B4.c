#include "common.h"

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 pad4[0x48 - 0x4];
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 unk4C;
} WeaponInfo;

typedef struct {
    /* 0x000 */ u8 pad0[0x51C];
    /* 0x51C */ s32 unk51C;
    /* 0x520 */ u8 pad520[0x996 - 0x520];
    /* 0x996 */ s16 weapon;
    /* 0x998 */ u8 pad998[0xB14 - 0x998];
    /* 0xB14 */ s16 nextWeapon;
} Player;

typedef struct {
    /* 0x000 */ u8 pad0[0x1A8];
    /* 0x1A8 */ Player *owner;
} Actor;

typedef struct {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ s32 unk2C;
    /* 0x030 */ u8 pad30[0x108 - 0x30];
    /* 0x108 */ s32 unk108;
    /* 0x10C */ s32 unk10C;
    /* 0x110 */ s32 unk110;
    /* 0x114 */ u8 pad114[4];
    /* 0x118 */ s32 unk118;
    /* 0x11C */ u8 pad11C[0x128 - 0x11C];
    /* 0x128 */ s32 unk128;
} State;

extern WeaponInfo *D_800AF618[];
extern u32 D_800B6D20;
extern s32 D_800F7078;
extern u8 D_801198B0[];
void func_0027FC2C(void *buf, s32 a, s32 b);
void func_00253DE0(Player *player, s32 id);

void func_002596B4(Actor *actor, State *state) {
    Player *player = actor->owner;

    if (player->weapon != player->nextWeapon || D_800F7078 != 4) {
        player->weapon = player->nextWeapon;
        state->unk2C = D_800AF618[player->weapon]->unk48;
        state->unk108 = D_800AF618[player->weapon]->unk4C;
        if (D_800B6D20 >= 5) {
            func_0027FC2C(D_801198B0, player->unk51C, D_800AF618[player->weapon]->unk0);
        }
        state->unk10C = 0;
        state->unk110 = 0;
        state->unk128 = 0;
        state->unk118 = 0;
    }
    func_00253DE0(player, 0x1A8);
}
