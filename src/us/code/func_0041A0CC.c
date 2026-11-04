#include "common.h"
#include "structs/menu.h"

extern s32 D_800C1F78; /* music track */
extern char D_800B6600[];
extern char D_800B6618[];
extern char D_800C1040[]; /* number format */
extern s32 func_004160F0(MenuItem *a);
extern void func_0029E3E0(char *dst, char *fmt, ...);

s32 func_0041A0CC(MenuItem *a) {
    if (D_800C1F78 == 0) {
        a->text = D_800B6600;
    } else {
        a->text = D_800B6618;
        func_0029E3E0(a->text + (func_004160F0(a) - 2), D_800C1040, D_800C1F78);
    }
    return 0;
}
