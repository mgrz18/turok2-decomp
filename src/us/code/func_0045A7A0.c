#include "common.h"
#include "globals/pak.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xB0];
    /* 0xB0 */ s32 flags;
} Item;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ Item *item;
} Arg0;

extern s32 sPakMenuStartLatched;

s32 PakMenu_PollStartPressed_2(Arg0 *arg0) {
    if ((arg0->item->flags & 0x1000) && (sPakMenuStartLatched == 0)) {
        gPakMenuStartPressed = 1;
        sPakMenuStartLatched = 1;
    }
    return gPakMenuStartPressed;
}
