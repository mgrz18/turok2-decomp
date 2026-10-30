#include "common.h"

/* Pick the menu page to go back to, from the game mode flags. */

typedef struct {
    u8 pad0[4];
    s8 mode; /* 0x04 */
} MenuArgs;

typedef struct {
    u8 pad0[0x20];
    MenuArgs *args; /* 0x20 */
} MenuEvent;

extern s32 D_80130984;
extern s32 D_80130990;
extern s32 D_80130994;
extern s32 D_80130998;
extern s32 D_801309B4;
extern s32 D_800C2028;
extern u8 D_0043ABA0[];
extern u8 D_0043ABC4[];
extern u8 D_0043ABE8[];
extern u8 D_0043AC0C[];
extern u8 D_0043874C[];
extern u8 D_004390B0[];
extern u8 D_0043BE14[];
extern u8 D_0043B658[];

static __inline__ void *modePage(s32 *mode) {
    void *page;

    switch (*mode) {
    case 0:
    default:
        page = D_0043ABA0;
        break;
    case 1:
        page = D_0043ABC4;
        break;
    case 2:
        page = D_0043ABE8;
        break;
    case 3:
        page = D_0043AC0C;
        break;
    }
    return page;
}

void *func_00456B7C(MenuEvent *event) {
    void *page = NULL;
    s32 mode;

    if (D_80130984 != 0) {
        if (D_801309B4 != 0) {
            if (D_80130990 != 0) {
                mode = D_800C2028;
            } else {
                mode = event->args->mode;
            }
            page = modePage(&mode);
        } else {
            page = D_0043BE14;
        }
    } else if (D_80130994 != 0) {
        if (D_801309B4 != 0) {
            page = D_0043874C;
        } else if (D_80130998 != 0) {
            page = D_0043BE14;
        } else {
            page = D_004390B0;
        }
    } else if (D_80130990 != 0) {
        page = D_0043B658;
    }
    return page;
}
