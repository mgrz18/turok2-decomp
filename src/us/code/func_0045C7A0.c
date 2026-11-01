/* Second-module copy of func_00423880 (first module; this one at link address 0x004237A0), calling this module's own functions at their link addresses. */
#include "common.h"

void func_00416644(char *dst, char *fmt, void *arg2, s32 arg3, s32 arg4);

extern char D_0043BDF0[];
extern char D_0043BE14[];
extern char D_80119E2C[];
extern s32 D_8011F148;
extern s32 D_800C2020;
extern s32 D_80130940;
extern s32 D_80130990;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ u8 *player;
    /* 0x20 */ s32 arg;
    /* 0x24 */ char *text;
} MenuEvent;

s32 func_0045C7A0(s32 unused, MenuEvent *event) {
    char *dst;

    D_800C2020 = 1;
    D_80130940 = 0;
    if (D_80130990 != 0) {
        D_8011F148 = 1;
    } else {
        if (event->player != NULL) {
            dst = *(char **)(event->player + 0x51C) + 0x53C;
        } else {
            dst = D_80119E2C;
        }
        if (event->text == D_0043BE14 || event->text == D_0043BDF0) {
            D_80130940 = 0;
        }
        func_00416644(dst, event->text, event->player, event->arg, 0);
    }
    return 1;
}
