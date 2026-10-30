#include "common.h"

/* Name entry: append the selected character to the current player's name. */

typedef struct {
    s32 cursor;   /* 0x00 */
    f32 timer;    /* 0x04 */
    s32 length;   /* 0x08 */
    char name[12]; /* 0x0C */
} NameEntry; /* 0x18 bytes */

typedef struct {
    u8 pad0[0x14];
    char *text; /* 0x14 */
} MenuItem;

typedef struct {
    u8 pad0[0x1C];
    void *player; /* 0x1C */
} PlayerEvent;

extern u8 D_80119870[];
extern NameEntry D_800C1FB0[];

s32 func_0025340C(void *state, void *player);

s32 func_00456364(MenuItem *item, PlayerEvent *event) {
    s32 i = func_0025340C(D_80119870, event->player);

    D_800C1FB0[i].timer = 0.0f;
    if (D_800C1FB0[i].length == 8) {
        return 0;
    }
    D_800C1FB0[i].name[D_800C1FB0[i].cursor] = item->text[0];
    D_800C1FB0[i].cursor++;
    D_800C1FB0[i].length++;
    D_800C1FB0[i].name[D_800C1FB0[i].length] = '\0';
    return 0;
}
