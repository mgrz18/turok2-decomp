/* Local types that vm1/TitleMenu and its vm2 copy both define (one layout): one definition
 * for both copies (tools/struct_census.py). */
#ifndef STRUCTS_TWIN_TITLEMENU_H
#define STRUCTS_TWIN_TITLEMENU_H

#include "common.h"
#include "structs/menu.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
} Unk00419618;
typedef struct {
    /* 0x00 */ u8 pad0[0x80];
    /* 0x80 */ u32 flags80;
    /* 0x84 */ u8 pad84[0x24];
    /* 0xA8 */ u32 flagsA8;
} A254Screen;
typedef struct {
    /* 0x00 */ s16 state;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ A254Screen *screen;
} A254Menu;

#endif
