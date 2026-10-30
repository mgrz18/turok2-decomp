/* WRONG CUT: func_004131B0 really runs 0x4131B0-0x413228 (0x78 bytes): the first three words of splat's func_0041321C
 * (`addu $2,$5,$0; jr $31; nop`) are its shared return. */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU_2/80400000_18.CPP, the item test inside func_8040332C_de. */
#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x11];
    /* 0x11 */ u8 kind;
    /* 0x12 */ u8 subkind;
} Item;

/* Whether the item is one of the kinds a cinema refuses: 1/5, 9/4 or 10/3. */
s32 func_004131B0(Item *item) {
    s32 hit;

    hit = 0;
    switch (item->kind) {
        case 1:
            if (item->subkind == 5) {
                hit = 1;
            }
            break;
        case 9:
            if (item->subkind == 4) {
                hit = 1;
            }
            break;
        case 10:
            if (item->subkind == 3) {
                hit = 1;
            }
            break;
        default:
            hit = 0;
            break;
    }
    return hit;
}
