#include "common.h"

extern char D_800C1768[];
extern char D_800B43BC[];
extern char D_800B4410[];
extern char D_800B4464[];
extern char D_800B44C4[];
extern char D_800B44FC[];
extern char D_800B4560[];
extern s32 D_80130A30;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ char *text;
} MenuItem;

s32 func_0042505C(MenuItem *item) {
    switch (D_80130A30) {
        case -1:
        case 0:
        default:
            item->text = D_800C1768;
            break;
        case 1:
            item->text = D_800B43BC;
            break;
        case 2:
            item->text = D_800B4410;
            break;
        case 3:
            item->text = D_800B4464;
            break;
        case 4:
            item->text = D_800B44C4;
            break;
        case 5:
            item->text = D_800B44FC;
            break;
        case 6:
            item->text = D_800B4560;
            break;
    }
    return 0;
}
