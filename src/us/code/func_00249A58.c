#include "common.h"

typedef struct ActorDef {
    /* 0x00 */ s32 kind;
    /* 0x04 */ u8 pad4[0x10];
    /* 0x14 */ u32 flags;
} ActorDef;

typedef struct Target Target;
struct Target {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ ActorDef *def;
    /* 0x018 */ u8 pad18[0xD4 - 0x18];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ u8 padD8[0x144 - 0xD8];
    /* 0x144 */ s32 health;
    /* 0x148 */ u8 pad148[0x174 - 0x148];
    /* 0x174 */ s8 type;
    /* 0x175 */ u8 pad175[0x51C - 0x175];
    /* 0x51C */ Target *rider;
    /* 0x520 */ u8 pad520[0x54C - 0x520];
    /* 0x54C */ s32 unk54C;
};

typedef struct Player {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[0x996 - 0x140];
    /* 0x996 */ s16 weapon;
} Player;

s32 func_00245FC8(Player *, void *, Target *);

/* Whether the player's weapon may lock on to a target. */
s32 func_00249A58(Player *player, Target *t) {
    s32 weapon = player->weapon;
    s32 busy;
    s32 ok;

    if (weapon == 13 || weapon == 29) {
        if (t->flags & 0x300000) {
            if (!(t->def->flags & 0x20)) {
                return 0;
            }
            if (t->type == 0x23) {
                return 0;
            }
            if (t->type == 0xF) {
                return 0;
            }
            if (t->health == 0) {
                return 0;
            }
            busy = 0;
            if (t->rider != NULL) {
                busy = t->rider->unk54C != 0;
            }
            if (!busy) {
                goto visible;
            }
            return 0;
        }
        if (t->def->kind == 1) {
            if (!(t->def->flags & 0x2400)) {
                return 0;
            }
            if (t->type == 0x21 || t->type == 0x34 || t->type == 0x3C || t->type == 0x3D) {
                return 0;
            }
            if (t->health != 0) {
                return func_00245FC8(player, player->ai, t) == 0;
            }
            return 0;
        }
    }
    switch (t->def->kind) {
    case 1:
        if (t->type == 0x3C) {
            return 0;
        }
        if (weapon == 0x10) {
            goto visible;
        }
        return 1;
    case 4:
        if (t->health != 0) {
        visible:
            return func_00245FC8(player, player->ai, t) == 0;
        }
        return 0;
    case 7:
        return 0;
    case 11:
        ok = 0;
        if (t->type != 0x23 && t->type != 0xF && t->health != 0) {
            busy = 0;
            if (t->rider != NULL) {
                busy = t->rider->unk54C != 0;
            }
            if (!busy) {
                ok = 1;
            }
        }
        return ok;
    }
    return 0;
}
