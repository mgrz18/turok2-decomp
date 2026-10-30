/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00444F0C.c, func_00444F0C, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ char *text;
} MenuItemTmpl;

extern u32 D_8011AAE4; /* D_8011AAD8 + 0xC */
extern char D_800B4158[];
extern char D_800B4170[];
extern char D_800C1318[];
extern s32 func_00206078(void);
extern void func_00206068(s32);
extern s32 func_004160F0(MenuItemTmpl *a);
extern void func_0029E3E0(char *dst, char *fmt, ...);

s32 func_0041C8CC(MenuItemTmpl *a) {
    u32 h = D_8011AAE4 >> 4;

    func_00206068(func_00206078() + 2);
    if (h == 0) {
        a->text = D_800B4158;
    } else {
        a->text = D_800B4170;
        func_0029E3E0(a->text + (func_004160F0(a) - 4), D_800C1318, h);
    }
    return 0;
}
