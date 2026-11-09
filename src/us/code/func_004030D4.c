#include "common.h"
#include "protos/ai.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x3C];
    /* 0x3C */ s32 flags;
    /* 0x40 */ u8 pad1[0xC4 - 0x40];
    /* 0xC4 */ s16 id;
    /* 0xC6 */ u8 pad2;
    /* 0xC7 */ s8 count;
} Item004030D4;

void MantidQueen_UpdateAttackAnim(s32 a0, Item004030D4 *item) {
    s32 needed;

    switch (item->id) {
        case 0xFA2:
        case 0x10CC:
            item->flags &= ~2;
            break;
    }
    if (item->id == 0x10CC || item->id == 0x1131) {
        needed = 2;
    } else {
        needed = 1;
    }
    if (item->count >= needed) {
        CAI__SetMode(a0, item, 8);
    }
}
