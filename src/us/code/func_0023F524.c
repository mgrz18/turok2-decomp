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
    s16 amount;     /* 0x0C */
} PickupEntry;

typedef struct Player {
    char pad0[4];
    Vec3 position;  /* 0x004 */
    char pad10[0x51C - 0x10];
    void *ui;       /* 0x51C */
    char pad520[8];
    s16 lifeForce;  /* 0x528 */
    s16 lives;      /* 0x52A */
} Player;

extern char D_801198B0[];

void func_00275624(s32 sound);
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

/* Life force pickup: every 100 gives an extra life, up to 9. */
s32 func_0023F524(Player *player, PickupEntry *entry) {
    void *resource;
    s32 sound;
    s32 callback;

    player->lifeForce += entry->amount;
    if (player->lifeForce >= 100) {
        player->lifeForce -= 100;
        if (player->lives < 9) {
            player->lives++;
            func_00275624(0x3A9D);
        }
    }
    sound = entry->sound;
    callback = entry->callback;
    resource = entry->resource;
    pickup_feedback(player, resource, sound, callback);
    return 1;
}
