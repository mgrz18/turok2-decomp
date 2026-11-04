#include "common.h"

extern s32 D_800C202C;
extern s32 D_8011B108;

/* arg0->0x20 points at the current selection record; byte 4 is its index. */
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s8 index;
    /* 0x05 */ u8 pad5[0xB0 - 0x5];
    /* 0xB0 */ s32 flags;
} Selection;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ Selection *sel;
} SelectionOwner;

s32 func_00421880(SelectionOwner *arg0) {
    if ((arg0->sel->flags & 0x1000) && (D_800C202C == 0)) {
        D_8011B108 = 1;
        D_800C202C = 1;
    }
    return D_8011B108;
}
