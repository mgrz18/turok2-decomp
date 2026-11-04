#include "common.h"

/* The multiplayer settings, D_8011AAD8 (MenuSettings in func_00450F4C; 0x184 bytes, see func_0042F514). */
typedef struct MenuSettings {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x10];
    /* 0x20 */ s8 unk20;
    /* 0x21 */ s8 unk21;
    /* 0x22 */ s8 unk22;
    /* 0x23 */ s8 unk23;
    /* 0x24 */ u8 unk24;
    /* 0x25 */ s8 unk25;
    /* 0x26 */ s8 unk26;
    /* 0x27 */ s8 unk27;
    /* 0x28 */ s8 unk28;
    /* 0x29 */ u8 unk29;
    /* 0x2A */ s8 unk2A;
    /* 0x2B */ s8 unk2B;
    /* 0x2C */ s8 unk2C;
    /* 0x2D */ s8 unk2D;
    /* 0x2E */ u8 unk2E;
} MenuSettings;

s32 func_0041648C(s32, s32, s32, s32, s32, s32);

extern MenuSettings D_8011AAD8;

s32 func_00451E54(s32 arg0, s32 arg1) {
    MenuSettings *settings = &D_8011AAD8;

    settings->unk2A = func_0041648C(arg1, settings->unk2A, 5, 0, 0x1E, 1);
    return 0;
}
