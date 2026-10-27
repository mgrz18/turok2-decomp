#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x3E];
    /* 0x3E */ u8 tagMode;
} Options;

typedef struct {
    /* 0x0000 */ u8 pad0[0x518];
    /* 0x0518 */ Options *options;
    /* 0x051C */ u8 pad51C[0x934 - 0x51C];
    /* 0x0934 */ s16 ammo[0x31];
    /* 0x0996 */ s16 weapon;
    /* 0x0998 */ u8 pad998[0xA64 - 0x998];
    /* 0x0A64 */ s32 heldButtons;
    /* 0x0A68 */ s32 pressedButtons;
    /* 0x0A6C */ u8 padA6C[0xB2C - 0xA6C];
    /* 0x0B2C */ s32 unkB2C;
    /* 0x0B30 */ u8 padB30[0x1284 - 0xB30];
    /* 0x1284 */ s32 unk1284;
} Player;

typedef struct {
    /* 0x000 */ u8 pad0[0x1A8];
    /* 0x1A8 */ Player *owner;
} Object;

typedef struct {
    /* 0x000 */ u8 pad0[0x35];
    /* 0x035 */ s8 unk35;
    /* 0x036 */ u8 pad36[0x118 - 0x36];
    /* 0x118 */ f32 timer118;
    /* 0x11C */ u8 pad11C[0x130 - 0x11C];
    /* 0x130 */ f32 timer130;
} AI;

extern s32 func_0024E700(Player *player, s32 weapon);
extern s16 *func_0025398C(Player *player, s32 weapon);
extern void func_00243414(Object *obj, AI *ai, s32 mode);

/* Fire check for the first-person weapon: once its cooldown is over and the
 * weapon is usable, fire button 0x2000 starts mode 4 and a tap of 0x10 with
 * weapon 19 starts mode 5. Returns 1 when it fired. */
s32 func_00257EA0(Object *obj, AI *ai) {
    Player *player;
    s32 tapped;
    s32 mode;

    player = obj->owner;
    if (ai->timer118 > 0.0f) {
        return 0;
    }
    if (!func_0024E700(player, player->weapon)) {
        return 0;
    }
    if (player->options->tagMode == 1) {
        return 0;
    }
    if ((player->weapon == 13 || player->weapon == 29) && player->unkB2C != 2) {
        return 0;
    }
    if (player->weapon == 23 && ai->timer130 > 0.0f) {
        return 0;
    }
    if (player->unk1284 != 0) {
        return 0;
    }
    if (player->weapon == 21) {
        Player *owner = obj->owner;
        if (*(owner->ammo + *func_0025398C(owner, 21)) == 0) {
            return 0;
        }
    }
    tapped = 0;
    if (player->pressedButtons & 0x10) {
        tapped = player->weapon == 19;
    }
    if (player->heldButtons & 0x2000) {
        mode = 4;
    } else if (tapped) {
        mode = 5;
    } else {
        return 0;
    }
    func_00243414(obj, ai, mode);
    ai->unk35 = -1;
    return 1;
}
