#include "common.h"
#include "structs/player.h"

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
    /* 0x000 */ u8 kind; /* 1: player */
    /* 0x001 */ u8 pad1[0xD3];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ u8 padD8[0xD0];
    /* 0x1A8 */ CPlayer *owner;
} Toucher;

extern u8 D_801198B0[]; /* game */
extern u8 D_801198F0[]; /* game + 0x40: fallback camera */
extern char *D_800B69D8[]; /* {text, sound} pairs */
extern s32 D_800B69DC[];  /* D_800B69D8 + 4 */

extern s32 CPlayer__GetPickupCountLow(CPlayer *player, s32 item); /* can take it? */
extern void CPlayer__UseSpecialPickup(CPlayer *player, s32 item);
extern void CCameraPool__AddMultiLineText(void *game, void *camera, char *text, Pickup *obj, f32 time);
extern void Sound__PlaySpeech(s32 sound);

/* A touch on a pickup: give the item, or show why it cannot be taken. */
s32 Action_CheckRequiredItem(Pickup *obj, PickupState *state, Toucher *toucher) {
    PickupInfo *info = &obj->def->info;
    CPlayer *player;
    void *camera;

    if (info->item == 0 || state->taken != 0 || toucher->kind != 1 || !(toucher->flags & 0x300000)) {
        return 1;
    }
    player = toucher->owner;
    if (CPlayer__GetPickupCountLow(player, info->item)) {
        CPlayer__UseSpecialPickup(player, info->item);
        return 1;
    }
    if (info->message == 0) {
        return 0;
    }
    camera = D_801198F0;
    if (player->m_pCamera != NULL) {
        camera = player->m_pCamera;
    }
    CCameraPool__AddMultiLineText(D_801198B0, camera,
                  D_800B69D8[info->message * 2], obj, 4.0f);
    if (D_800B69DC[info->message * 2] != 0) {
        Sound__PlaySpeech(D_800B69DC[info->message * 2]);
    }
    return 0;
}
