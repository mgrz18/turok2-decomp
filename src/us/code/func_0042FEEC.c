/* WRONG CUT: the real function is func_0042FEEC, size 0x6C (splat split it at func_0042FF10). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_19.CPP, func_8044A6CC_de, with Turok 2's layouts. */
#include "common.h"

typedef struct Cameras {
    /* 0x0 */ s32 handle;
    /* 0x4 */ s32 cameras; /* address of count 0xEC0-byte records */
    /* 0x8 */ s32 count;
} Cameras;

void func_0042FFF0(char *);

/* Updates each camera's message block (at 0x558), then the owner's own at 0x598. */
void func_0042FEEC(Cameras *owner) {
    s32 i;

    for (i = 0; i < owner->count; i++) {
        func_0042FFF0((char *)(i * 0xEC0 + owner->cameras + 0x558));
    }
    func_0042FFF0((char *)owner + 0x598);
}
