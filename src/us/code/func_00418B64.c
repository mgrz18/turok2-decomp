#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ char *text;
} MenuItemTmpl;

typedef struct {
    /* 0x00 */ u8 pad0[0x21];
    /* 0x21 */ s8 limit;
} GameSetup;

extern GameSetup D_8011AAD8;
extern char D_800C0E84[]; /* "  %02d" */
extern char D_800C0E8C[]; /* "none" */
extern s32 func_004160F0(MenuItemTmpl *a);
extern void func_0029E3E0(char *dst, char *fmt, ...);

/* Prints the setting at D_8011AAD8+0x21 into the label, or "none". */
s32 func_00418B64(MenuItemTmpl *a) {
    GameSetup *setup = &D_8011AAD8;

    if (setup->limit != 0) {
        func_0029E3E0(a->text + (func_004160F0(a) - 4), D_800C0E84, setup->limit);
    } else {
        func_0029E3E0(a->text + (func_004160F0(a) - 4), D_800C0E8C);
    }
    return 0;
}
