#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ char *text;
} MenuItemTmpl;

typedef struct {
    /* 0x00 */ u8 pad0[0x25];
    /* 0x25 */ s8 unk25;
    /* 0x26 */ s8 limit;
    /* 0x27 */ s8 level;
} GameSetup;

extern GameSetup D_8011AAD8;
extern char D_800C0E84[]; /* "  %02d" */
extern char D_800C0E8C[]; /* "none" */
extern s32 func_004160F0(MenuItemTmpl *a);
extern void func_0029E3E0(char *dst, char *fmt, ...);

/* Prints the second limit (D_8011AAD8+0x26) into the label, or "none". */
s32 func_00418584(MenuItemTmpl *a) {
    GameSetup *setup = &D_8011AAD8;

    if (setup->limit != 0) {
        func_0029E3E0(a->text + (func_004160F0(a) - 4), D_800C0E84, setup->limit);
    } else {
        func_0029E3E0(a->text + (func_004160F0(a) - 4), D_800C0E8C);
    }
    return 0;
}
