#include "common.h"

s32 func_0026EFB8(s32 id);

extern u8 *D_800C201C;
extern char D_800B4A40[];
extern char D_800B4A50[];
extern char D_800B4A60[];
extern char D_800B4A70[];
extern char D_800B4A88[];
extern char D_800B4A9C[];
extern char D_800B4AB4[];
extern char D_800B4ABC[];

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ char *text;
} MenuItem;

s32 func_00421F88(MenuItem *item) {
    switch (func_0026EFB8(*(s32 *)(D_800C201C + 0x424))) {
        case -1:
        default:
            item->text = D_800B4ABC;
            break;
        case 0:
            item->text = D_800B4AB4;
            break;
        case 1:
            item->text = D_800B4A40;
            break;
        case 2:
            item->text = D_800B4A50;
            break;
        case 3:
            item->text = D_800B4A60;
            break;
        case 4:
            item->text = D_800B4A70;
            break;
        case 5:
            item->text = D_800B4A88;
            break;
        case 6:
            item->text = D_800B4A9C;
            break;
    }
    return 0;
}
