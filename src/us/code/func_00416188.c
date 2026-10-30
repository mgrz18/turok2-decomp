/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00442224.c, func_00442224, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x17];
    /* 0x17 */ u8 ch;
} MenuItemTmpl;

extern void func_00413F74(MenuItemTmpl *a, s32 b, s32 c, s32 d, u8 *text);

/* Draws the item's single character. */
void func_00416188(MenuItemTmpl *a, s32 b, s32 c, s32 d) {
    u8 buf[2];

    buf[0] = a->ch;
    buf[1] = 0;
    func_00413F74(a, b, c, d, buf);
}
