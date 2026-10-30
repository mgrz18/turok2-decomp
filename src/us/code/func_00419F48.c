#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ char *text;
} MenuItemTmpl;

extern u8 D_8011AAED; /* D_8011AAD8 + 0x15: 0x80-centred setting */
extern char D_800B6570[];
extern char D_800B6558[];
extern char D_800B6540[];
extern char D_800B6588[];
extern char D_800C1030[]; /* "  %2d" */
extern char D_800C1038[]; /* " %3d" */
extern s32 func_004160F0(MenuItemTmpl *a);
extern void func_0029E3E0(char *dst, char *fmt, ...);

/* Label of a slider setting: its value from -16 to 15, in steps of 8 raw units. */
s32 func_00419F48(MenuItemTmpl *a) {
    s32 v = (D_8011AAED - 0x80) / 8;

    if (v == 0) {
        a->text = D_800B6570;
    } else if (v == 15) {
        a->text = D_800B6558;
    } else if (v == -16) {
        a->text = D_800B6540;
    } else if (v > 0) {
        a->text = D_800B6588;
        func_0029E3E0(a->text + (func_004160F0(a) - 4), D_800C1030, v);
    } else {
        a->text = D_800B6588;
        func_0029E3E0(a->text + (func_004160F0(a) - 4), D_800C1038, v);
    }
    return 0;
}
