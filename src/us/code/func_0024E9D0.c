#include "common.h"

typedef struct Region {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ u32 flags;
    /* 0x48 */ u8 pad48[0x52 - 0x48];
    /* 0x52 */ u16 flags52;
    /* 0x54 */ u16 flags54;
} Region;

typedef struct Player {
    /* 0x000 */ u8 pad0[0x6AA];
    /* 0x6AA */ u8 keys[2];
    /* 0x6AC */ u8 pad6AC[0xA08 - 0x6AC];
    /* 0xA08 */ s16 state;
    /* 0xA0A */ u8 padA0A[0xA9C - 0xA0A];
    /* 0xA9C */ f32 unkA9C;
} Player;

extern u32 D_8011AAD8;              /* cheat flags */

Region *func_00268A2C(void *actor);
s32 func_0024E5F0(Player *player, void *actor, s32 state);

static __inline__ s32 func_0024E9D0_isSpecial(Player *player) {
    if (player->state == 18 || player->state == 16 || player->state == 17) {
        return 1;
    }
    return 0;
}

static __inline__ s32 func_0024E9D0_hasKey(Player *player, s32 i) {
    if (D_8011AAD8 & 0x10) {
        return 1;
    }
    return player->keys[i] & 0xF;
}

/* Enters state 32 when the actor stands in a region flagged 0x1000 whose
 * key requirements (0x800 on the flags, 0x10 on flags54) the player meets.
 * Returns whether it did. */
s32 func_0024E9D0(Player *player, void *actor) {
    Region *region;

    if (player->unkA9C > 0.0f) {
        return 0;
    }
    if (func_0024E9D0_isSpecial(player)) {
        return 0;
    }
    region = func_00268A2C(actor);
    if (region != NULL && (region->flags52 & 0x1000)) {
        if (!(region->flags & 0x800)) {
            if (region->flags54 & 0x10) {
                goto second;
            }
            goto enter;
        } else {
            if (func_0024E9D0_hasKey(player, 0)) {
                goto enter;
            }
            if (!(region->flags54 & 0x10)) {
                return 0;
            }
        }
    second:
        if (func_0024E9D0_hasKey(player, 1)) {
        enter:
            func_0024E5F0(player, actor, 32);
            return 1;
        }
    }
    return 0;
}
