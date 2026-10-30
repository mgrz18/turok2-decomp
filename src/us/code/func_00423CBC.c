/* WRONG CUT: one function 0x00423CBC-0x00423D38 (size 0x7C): a switch whose cases splat cut
 * into func_00423CF4, func_00423D04, func_00423D14, func_00423D24 (jump table jtbl_800C16D0),
 * and whose shared `return 0` tail is the already-accepted func_00423D30 (8 bytes). */
#include "common.h"

extern u8 D_800B5180[];
extern u8 D_800B51CC[];
extern u8 D_800B5218[];
extern u8 D_800B5264[];
extern u8 D_800B52C4[];
extern s32 D_801309B0;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ void *text;
} MenuItem;

s32 func_00423CBC(MenuItem *item) {
    switch (D_801309B0) {
        case 0:
        default:
            item->text = D_800B5180;
            break;
        case 1:
            item->text = D_800B51CC;
            break;
        case 2:
            item->text = D_800B5218;
            break;
        case 3:
            item->text = D_800B5264;
            break;
        case 4:
            item->text = D_800B52C4;
            break;
    }
    return 0;
}
