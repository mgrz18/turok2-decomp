#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ u32 flags;
    /* 0x0C */ u8 padC[8];
    /* 0x14 */ char *text;
} MenuItemTmpl;

typedef struct {
    /* 0x000 */ u8 pad0[0x19];
    /* 0x019 */ u8 unk19;
    /* 0x01A */ u8 pad1A[0x1B0 - 0x1A];
    /* 0x1B0 */ s32 mode;
} GameSetup;

extern GameSetup D_8011AAD8;
extern u8 D_8011AAF3; /* D_8011AAD8 + 0x1B */
extern char D_800B3FD8[];
extern char D_800B3FF0[];

s32 func_0041C338(MenuItemTmpl *a) {
    if (D_8011AAD8.mode == 13 && D_8011AAD8.unk19 == 0) {
        a->flags &= ~0x1000000;
    } else {
        a->flags |= 0x1000000;
    }
    switch (D_8011AAF3) {
    case 0:
        a->text = D_800B3FD8;
        break;
    case 1:
        a->text = D_800B3FF0;
        break;
    }
    return 0;
}
