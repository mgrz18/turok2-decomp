#include "common.h"

s32 func_00253640(void *player, s32 arg1, s32 arg2);

typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ s32 flags;
} MenuItem;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ MenuItem *item;
    /* 0x10 */ u8 pad10[0xC];
    /* 0x1C */ u8 *player;
} MenuEvent;

s32 func_00423210(s32 unused, MenuEvent *event) {
    u8 *player = event->player;

    if (func_00253640(player, 1, *(s32 *)(player + 0x92C)) != 0) {
        event->item->flags |= 0x01000000;
    } else {
        event->item->flags &= ~0x01000000;
    }
    return 0;
}
