/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/802ABDBC_11.CPP, func_802ACB18_de (the pickup feedback), with Turok 2's layouts. */
#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

/* Pickup effect descriptor. */
typedef struct PickupEntry {
    void *resource; /* 0x00 */
    char pad4[2];
    s16 sound;      /* 0x06 */
    s16 callback;   /* 0x08 */
    char padA[2];
    s32 amount;     /* 0x0C */
    s32 max;        /* 0x10 */
} PickupEntry;

typedef struct PlayerDef {
    char pad0[0x18];
    s32 maxHealth;  /* 0x18 */
} PlayerDef;

typedef struct Player {
    char pad0[4];
    Vec3 position;  /* 0x004 */
    char pad10[4];
    PlayerDef *def; /* 0x014 */
    char pad18[0x144 - 0x18];
    u32 health;     /* 0x144 */
    char pad148[0x51C - 0x148];
    void *ui;       /* 0x51C */
    char pad520[4];
    s32 health8;    /* 0x524, health in 24.8 fixed point */
} Player;

extern u8 D_8011AAF1;

extern char D_801198B0[];

void func_00281060(void *ui, s32 red, s32 green, s32 blue, s32 alpha, s32 a5, s32 a6, s32 a7);
void func_0027FC2C(void *arg0, void *ui, void *resource);
s32 func_00275544(s32 sound, Vec3 position, s32 arg4, s32 arg5);
void func_0027580C(s32 callback);

/* Flashes the screen and plays the descriptor's optional resource, sound and callback. */
static __inline__ void pickup_feedback(Player *player, void *resource, s32 sound, s32 callback) {
    func_00281060(player->ui, 0x80, 0x32, 0x32, 0x4B, 0, 0, 2);
    if (resource != NULL) {
        func_0027FC2C(D_801198B0, player->ui, resource);
    }
    if (sound != 0) {
        func_00275544(sound, player->position, 0, -1);
    }
    if (callback != 0) {
        func_0027580C(callback);
    }
}

/* Health pickup: add the entry's amount, up to its cap (in multiplayer, up
 * to the player's own maximum). */
s32 func_0023F3E4(Player *player, PickupEntry *entry) {
    void *resource;
    s32 sound;
    s32 callback;
    s32 result;
    s32 max;
    u32 limit;

    max = entry->max << 8;
    result = 0;
    if (player->health8 < max) {
        player->health8 += entry->amount << 8;
        do { /* a block boundary keeps the flag load after the store */
            if (D_8011AAF1) {
                limit = player->def->maxHealth << 8;
                player->health = player->health8 = (u32)player->health8 < limit ? (u32)player->health8 : limit;
            } else if (player->health8 > max) {
                player->health8 = max;
            }
        } while (0);
        result = 1;
    }
    if (result != 0) {
        sound = entry->sound;
        callback = entry->callback;
        resource = entry->resource;
        pickup_feedback(player, resource, sound, callback);
    }
    return result;
}
