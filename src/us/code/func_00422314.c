#include "common.h"

extern char D_800B4B1C[];
extern char D_800B4B48[];
extern char D_800B5340[];
extern char D_800B534C[];
extern char D_800B5358[];
extern char D_800B5364[];
extern char D_800B5370[];
extern char D_800B537C[];
extern char D_800B5388[];
extern char D_800B5394[];
extern char D_800B53A0[];
extern char D_800B53AC[];
extern s8 D_80130950[];
extern s32 D_801309A8;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ char *text;
} MenuItem;

s32 func_00422314(MenuItem *item) {
    char *src;
    char *dst;
    s32 i;

    if (D_801309A8 != 0) {
        item->text = D_800B4B1C;
    } else {
        item->text = D_800B4B48;
        switch (D_80130950[0]) {
            case 0:
            default:
                src = D_800B5340;
                break;
            case 1:
                src = D_800B534C;
                break;
            case 2:
                src = D_800B5358;
                break;
            case 3:
                src = D_800B5364;
                break;
            case 4:
                src = D_800B5370;
                break;
            case 5:
                src = D_800B537C;
                break;
            case 6:
                src = D_800B5388;
                break;
            case 7:
                src = D_800B5394;
                break;
            case 8:
                src = D_800B53A0;
                break;
            case 9:
                src = D_800B53AC;
                break;
        }
        dst = item->text;
        dst += 8;
        for (i = 0; i < 11; i++) {
            *dst++ = *src++;
        }
    }
    return 0;
}
