/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/802ABDBC_11.CPP, func_802ACB18_de (the pickup feedback), with Turok 2's layouts. */
#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

/* Pickup effect descriptor. */
typedef struct PickupEntry {
    void *resource; /* 0x00 */
    s16 id;         /* 0x04 */
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
    char pad520[0xA28 - 0x520];
    f32 timerA28;   /* 0xA28 */
    f32 timerA2C;   /* 0xA2C */
    f32 amountA30;  /* 0xA30 */
} Player;

typedef struct PickupParams {
    char pad0[0xC];
    f32 amount;     /* 0x0C */
} PickupParams;

typedef struct PickupDef {
    char pad0[0x14];
    PickupParams params; /* 0x14 */
} PickupDef;

typedef struct PickupObject {
    char pad0[0x14];
    PickupDef *def; /* 0x14 */
} PickupObject;

extern char D_801198B0[];

void func_00248644(Player *player, s32 id);
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

/* Item pickups selected by the entry's id. */
s32 func_0023F634(Player *player, PickupEntry *entry, PickupObject *obj) {
    void *resource;
    s32 sound;
    s32 callback;
    PickupParams *params;

    params = &obj->def->params;
    switch (entry->id) {
        case 0x708:
            player->timerA28 = 120.0f;
            break;
        case 0x709:
            player->timerA2C = 120.0f;
            break;
        case 0x70A:
        case 0x70B:
            player->amountA30 += params->amount;
            break;
        case 0x111C:
        case 0x111D:
        case 0x111E:
        case 0x111F:
        case 0x1120:
        case 0x1130:
        case 0x1131:
        case 0x1132:
        case 0x1133:
        case 0x1134:
        case 0x1194:
            func_00248644(player, entry->id);
            break;
    }
    sound = entry->sound;
    callback = entry->callback;
    resource = entry->resource;
    pickup_feedback(player, resource, sound, callback);
    return 1;
}
