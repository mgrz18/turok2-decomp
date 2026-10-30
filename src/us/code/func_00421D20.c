/* WRONG CUT: one function 0x00421D20-0x00421DFC (size 0xDC); splat cut its second half into
 * func_00421D90 (it starts in the middle of an if chain, reading $v0 set here). */
#include "common.h"

extern char D_800C140C[];
extern char D_800B4870[];
extern char D_800B487C[];
extern char D_800B4888[];
extern char D_800B4894[];
extern char D_800B48A8[];
extern char D_800B48E4[];
extern s32 D_80130984;
extern s32 D_80130990;
extern s32 D_80130994;
extern s32 D_801309B4;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ char *text;
} MenuItem;

s32 func_00421D20(MenuItem *item, s16 *id) {
    s32 n = *id;
    u32 k = n - 3;

    if (D_80130994 != 0) {
        if (D_801309B4 != 0) {
            item->text = D_800B4870;
        } else {
            item->text = D_800B4888;
        }
    } else if (D_80130984 != 0) {
        if (k < 16) {
            item->text = D_800B48A8;
            return 0;
        }
        if (n == 21) {
            item->text = D_800C140C;
        } else if (D_801309B4 != 0) {
            item->text = D_800B487C;
        } else {
            item->text = D_800B4894;
        }
    } else if (D_80130990 != 0) {
        item->text = D_800B48E4;
    }
    return 0;
}
