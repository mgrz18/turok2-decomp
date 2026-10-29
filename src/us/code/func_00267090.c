/* Boundary fixed in versions/cuts.us.txt: the function starts at 0x00267090. splat cut its first word,
 * `lw $2, 0x14($4)`, into a one-word func_00267090 (the name its callers
 * jal), and the rest into func_00267094. Written whole under its real name.
 * Its switch reads jtbl_800A7D30, which still lives in the asm .rodata. */
#include "common.h"

typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ u8 pad4[0x10];
    /* 0x14 */ u32 flags;
} ActorDef;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ u32 flags;
} Attachment;

typedef struct {
    /* 0x000 */ u8 pad0[0xBF8];
    /* 0xBF8 */ s32 unkBF8;
} Player;

typedef struct {
    /* 0x000 */ u8 type; /* 1: player */
    /* 0x001 */ u8 pad1[0x13];
    /* 0x014 */ ActorDef *def;
    /* 0x018 */ u8 pad18[0xBC];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ u8 padD8[0x98];
    /* 0x170 */ Attachment *attachment;
    /* 0x174 */ u8 pad174[0x34];
    /* 0x1A8 */ Player *owner;
} Actor;

extern u8 D_8011AAF1;

s32 func_00267090(Actor *actor) {
    Player *owner;
    Attachment *attachment;
    u32 flags;
    u32 mask;

    switch ((u32) actor->def->kind) {
        case 11:
            if (actor->type != 1) {
                return 1;
            }
            if (actor->flags & 0x300000) {
                owner = actor->owner;
                if (D_8011AAF1 != 0 && owner->unkBF8 != 0) {
                    return 0;
                }
            }
            /* fallthrough */
        case 8:
            return 1;
        case 0:
            return actor->def->flags & 2;
        case 2:
            if (actor->type == 1) {
                return actor->flags & 0x10000;
            }
            return 0;
        case 1:
            if (actor->type == 1 && (actor->flags & 0x2000) && (attachment = actor->attachment) != NULL) {
                mask = 0x10000;
                return (attachment->flags & mask) == 0;
            }
            return 0;
        case 4:
        case 5:
        case 7:
        case 10:
            if (actor->type == 1) {
                flags = actor->flags;
                if (!(flags & 0x2000)) {
                    return 0;
                }
                return flags & 0x10000;
            }
            return 0;
        case 12:
            return actor->def->flags & 1;
        case 3:
        case 6:
        case 9:
        default:
            return 0;
    }
}
