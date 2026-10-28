#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x16];
    /* 0x16 */ s16 item;    /* item given on pickup, 0: none */
    /* 0x18 */ s16 message; /* index in D_800B69D8 when it cannot be taken */
} PickupInfo;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ PickupInfo info;
} PickupDef;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ PickupDef *def;
} Pickup;

typedef struct {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ s8 taken;
} PickupState;

typedef struct {
    /* 0x000 */ u8 pad0[0x51C];
    /* 0x51C */ void *camera;
} Player;

typedef struct {
    /* 0x000 */ u8 kind; /* 1: player */
    /* 0x001 */ u8 pad1[0xD3];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ u8 padD8[0xD0];
    /* 0x1A8 */ Player *owner;
} Toucher;

extern u8 D_801198B0[]; /* game */
extern u8 D_801198F0[]; /* game + 0x40: fallback camera */
extern char *D_800B69D8[]; /* {text, sound} pairs */
extern s32 D_800B69DC[];  /* D_800B69D8 + 4 */

extern s32 func_00253720(Player *player, s32 item); /* can take it? */
extern void func_002536E0(Player *player, s32 item);
extern void func_002817A0(void *game, void *camera, char *text, Pickup *obj, f32 time);
extern void func_0027580C(s32 sound);

/* A touch on a pickup: give the item, or show why it cannot be taken. */
s32 func_00265F90(Pickup *obj, PickupState *state, Toucher *toucher) {
    PickupInfo *info = &obj->def->info;
    Player *player;
    void *camera;

    if (info->item == 0 || state->taken != 0 || toucher->kind != 1 || !(toucher->flags & 0x300000)) {
        return 1;
    }
    player = toucher->owner;
    if (func_00253720(player, info->item)) {
        func_002536E0(player, info->item);
        return 1;
    }
    if (info->message == 0) {
        return 0;
    }
    camera = D_801198F0;
    if (player->camera != NULL) {
        camera = player->camera;
    }
    func_002817A0(D_801198B0, camera,
                  D_800B69D8[info->message * 2], obj, 4.0f);
    if (D_800B69DC[info->message * 2] != 0) {
        func_0027580C(D_800B69DC[info->message * 2]);
    }
    return 0;
}
