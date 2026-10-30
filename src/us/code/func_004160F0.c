/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00442158.c, func_00442158, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s16 type;
    /* 0x06 */ u8 pad6[0xE];
    /* 0x14 */ u8 *text;
} MenuItemTmpl;

extern u8 *func_00413230(void);

/* Length of the item's text up to the end or the first newline. */
s32 func_004160F0(MenuItemTmpl *a) {
    s32 n = 0;
    u8 *s;

    if (a->type == 5) {
        s = func_00413230();
    } else {
        s = a->text;
    }
    for (; s && *s && *s != '\n'; s++) {
        n++;
    }
    return n;
}
