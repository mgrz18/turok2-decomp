#include "common.h"

/* Weapon state for weapon 0x14: drains ammo while firing, every third tick. */

typedef struct {
    /* 0x000 */ u8 pad0[0x934];
    /* 0x934 */ s16 ammo[1];
    /* 0x936 */ u8 pad936[0xA64 - 0x936];
    /* 0xA64 */ s32 flags;
    /* 0xA68 */ u8 padA68[0xAAC - 0xA68];
    /* 0xAAC */ f32 unk_AAC;
} WeaponPlayer;

typedef struct {
    /* 0x000 */ u8 pad0[0x40];
    /* 0x040 */ f32 charge;
    /* 0x044 */ u8 pad44[0x64 - 0x44];
    /* 0x064 */ f32 timer;
} WeaponState;

extern s32 D_800B6D1C;
extern u32 D_800B6D20;

extern s32 func_0024E700(WeaponPlayer *player, s32 weapon);
extern s32 func_00243414(void *obj, WeaponState *state, s32 mode);
extern s16 *func_0025398C(WeaponPlayer *player, s32 weapon);
extern void func_00253F40(WeaponPlayer *player);
extern s32 func_00248720(void *player, s32 arg1);

#define USE_AMMO()                                              {                                                               ammo = &player->ammo[*func_0025398C(player, 0x14)];         n = *ammo - 1;                                              if (n < 0) {                                                    n = 0;                                                  }                                                           *ammo = n;                                              }

void func_002584DC(void *obj, WeaponState *state) {
    WeaponPlayer *player = *(WeaponPlayer **)((u8 *)obj + 0x1A8);
    s16 *ammo;
    s32 n;

    if (!(player->flags & 0x2000) || func_0024E700(player, 0x14) == 0) {
        if (state->charge < 3.0f) {
            USE_AMMO();
        }
        func_00253F40(player);
        func_00243414(obj, state, 2);
        return;
    }
    if (state->charge > 30.0f) {
        player->unk_AAC = 45.0f;
    }
    if (D_800B6D1C != 0) {
        state->timer += 1.5f;
        func_00248720(*(void **)((u8 *)obj + 0x1A8), 0x14);
    }
    if (D_800B6D20 % 3 == 0) {
        USE_AMMO();
    }
}
