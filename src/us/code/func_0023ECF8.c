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
    s16 index;      /* 0x0C */
    s16 argument;   /* 0x0E */
    s16 argument2;  /* 0x10 */
} PickupEntry;

typedef struct PlayerOptions {
    char pad0[0x3E];
    u8 unk3E;       /* 0x3E */
} PlayerOptions;

typedef struct Player {
    char pad0[4];
    Vec3 position;  /* 0x004 */
    char pad10[0x140 - 0x10];
    s32 flags;      /* 0x140 */
    char pad144[0x518 - 0x144];
    PlayerOptions *options; /* 0x518 */
    void *ui;       /* 0x51C */
    char pad520[0x972 - 0x520];
    s8 keys[0x24];  /* 0x972 */
    s16 level;      /* 0x996 */
    char pad998[0xB14 - 0x998];
    u16 lastKey;    /* 0xB14 */
} Player;

typedef struct KeyInfo {
    char pad0[0x14];
    s32 flags;      /* 0x14 */
} KeyInfo;

extern char D_801198B0[];
extern KeyInfo *D_800AF618[];
extern u8 D_8011AAF1;

s32 func_00253B54(Player *player, s32 index, s32 argument);
void func_00281060(void *ui, s32 red, s32 green, s32 blue, s32 alpha, s32 a5, s32 a6, s32 a7);
void func_0027FC2C(void *arg0, void *ui, void *resource);
s32 func_00275544(s32 sound, Vec3 position, s32 arg4, s32 arg5);
void func_0027580C(s32 callback);

/* Key pickup: mark the key as held and play the descriptor's feedback. */
s32 func_0023ECF8(Player *player, PickupEntry *entry) {
    KeyInfo *info;
    void *resource;
    s32 sound;
    s32 callback;
    s32 result;
    s32 had;
    Player *p;

    result = 0;
    if (entry->argument != -1 && player->keys[entry->index] != 0 &&
        (result = func_00253B54(player, entry->argument, entry->argument2)) == 0 &&
        (!D_8011AAF1 || !player->options->unk3E)) {
        return 0;
    }
    if (D_8011AAF1) {
        if (!(D_800AF618[entry->index]->flags & 8)) {
            return 0;
        }
    } else {
        if (D_800AF618[entry->index]->flags & 8) {
            return 0;
        }
    }
    had = player->keys[entry->index];
    p = player; /* a second name for the player: needed for register allocation */
    p->keys[entry->index] = 1;
    info = D_800AF618[entry->index];
    if (p->level < entry->index) {
        if ((p->flags & 0x80) ? (info->flags & 2) : (info->flags & 1)) {
            if (!D_8011AAF1 && had == 0) {
                p->lastKey = entry->index;
            }
        }
    }
    sound = entry->sound;
    callback = entry->callback;
    resource = entry->resource;
    func_00281060(p->ui, 0x80, 0x32, 0x32, 0x4B, 0, 0, 2);
    if (resource != NULL) {
        func_0027FC2C(D_801198B0, p->ui, resource);
    }
    if (sound != 0) {
        func_00275544(sound, p->position, 0, -1);
    }
    if (callback != 0) {
        func_0027580C(callback);
    }
    if (result == 0) {
        func_00253B54(p, entry->argument, entry->argument2);
    }
    return 1;
}
