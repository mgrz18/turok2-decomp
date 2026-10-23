/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/802ABDBC_11.CPP, func_802ACB18_de, with Turok 2's layouts. */
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
    s16 index;      /* 0x0C */
    s16 argument;   /* 0x0E */
} PickupEntry;

typedef struct Player {
    char pad0[4];
    Vec3 position;  /* 0x004 */
    char pad10[0x51C - 0x10];
    void *ui;       /* 0x51C */
} Player;

extern char D_801198B0[];

s32 func_00253B54(Player *player, s32 index, s32 argument);
void func_00281060(void *ui, s32 red, s32 green, s32 blue, s32 alpha, s32 a5, s32 a6, s32 a7);
void func_0027FC2C(void *arg0, void *ui, void *resource);
s32 func_00275544(s16 sound, Vec3 position, s32 arg4, s32 arg5);
void func_0027580C(s32 callback);

/* Flashes the player's screen and plays a pickup's optional resource, sound and callback. */
void func_0023F7B8(Player *player, void *resource, s32 sound, s32 callback) {
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
