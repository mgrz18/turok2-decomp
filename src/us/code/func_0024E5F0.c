#include "common.h"

typedef struct Player Player;

typedef struct PlayerStateInfo {
    /* 0x00 */ void (*enter)(Player *player, void *arg);
    /* 0x04 */ void *unk4;
    /* 0x08 */ void *unk8;
    /* 0x0C */ u32 flags;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s16 unk14;
} PlayerStateInfo;

struct Player {
    /* 0x000 */ u8 pad0[0xD4];
    /* 0x0D4 */ u32 actorFlags;
    /* 0x0D8 */ u8 padD8[0xA08 - 0xD8];
    /* 0xA08 */ s16 state;
    /* 0xA0A */ s16 lastState;
    /* 0xA0C */ u8 padA0C[4];
    /* 0xA10 */ s32 stateTime;
    /* 0xA14 */ u8 padA14[4];
    /* 0xA18 */ u32 lastStateFlags;
    /* 0xA1C */ u32 stateFlags;
    /* 0xA20 */ u8 padA20[0xC04 - 0xA20];
    /* 0xC04 */ s32 unkC04;
};

extern PlayerStateInfo D_800ADF54[];
extern u8 D_8011AAF1;

s32 func_002548FC(Player *player);

/* Switches the player to `state` (only to states 16-18 while
 * func_002548FC holds in D_8011AAF1's mode): runs its enter hook and takes
 * its flags. Returns whether the player ends up in that state. */
s32 func_0024E5F0(Player *player, void *arg, s32 state) {
    s16 last;
    u32 lastFlags;

    if (D_8011AAF1 != 0 && func_002548FC(player) != 0 && state != 18 && state != 16 && state != 17) {
        return 0;
    }
    last = player->state;
    lastFlags = player->stateFlags;
    player->state = state;
    player->lastState = last;
    player->lastStateFlags = lastFlags;
    player->actorFlags &= ~0x800000;
    if (D_800ADF54[state].enter != NULL) {
        D_800ADF54[state].enter(player, arg);
    }
    player->stateFlags = D_800ADF54[player->state].flags;
    if (D_800ADF54[player->state].unk10 != 0) {
        player->unkC04 = D_800ADF54[player->state].unk10;
    }
    if (player->state == state) {
        player->stateTime = 0;
        return 1;
    }
    return 0;
}
