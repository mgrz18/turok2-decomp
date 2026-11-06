#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xB0];
    /* 0xB0 */ s32 flags;
} Item;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ Item *item;
} Arg0;

extern s32 D_800C202C;
extern s32 D_8011B108;

s32 PakMenu_PollStartPressed_2(Arg0 *arg0) {
    if ((arg0->item->flags & 0x1000) && (D_800C202C == 0)) {
        D_8011B108 = 1;
        D_800C202C = 1;
    }
    return D_8011B108;
}
