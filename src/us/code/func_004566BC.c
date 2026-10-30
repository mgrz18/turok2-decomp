#include "common.h"

/* Name entry accepted: copy the edited name (blank-padded to 8 characters)
 * into the current player's record; an empty name restores the default one. */

typedef struct {
    s32 cursor;    /* 0x00 */
    f32 timer;     /* 0x04 */
    s32 length;    /* 0x08 */
    char name[12]; /* 0x0C */
} NameEntry; /* 0x18 bytes */

typedef struct {
    u8 pad0[0x1C];
    u8 *player; /* 0x1C */
} MenuEvent;

extern u8 D_80119870[];
extern NameEntry D_800C1FB0[];

s32 func_0025340C(void *state, void *player);
void func_0041BBB0(void *options);
void func_00416454(void *menu, MenuEvent *event, void *arg2);

s32 func_004566BC(void *menu, MenuEvent *event, void *arg2) {
    u8 *player = event->player;
    s32 i = func_0025340C(D_80119870, player);
    u8 *src = (u8 *)D_800C1FB0[i].name;
    u8 *dst = D_80119870 + 0x131B + i * 64;
    s32 count = 0;
    s32 k;
    u8 c;

    for (k = 0; k < 8; k++) {
        c = *src++;
        if (c == 0) {
            c = ' ';
        } else {
            count++;
        }
        *dst++ = c;
    }
    *dst = '\0';
    if (count == 0) {
        func_0041BBB0(*(void **)(player + 0x518));
    }
    func_00416454(menu, event, arg2);
    return 1;
}
