#include "common.h"

/* Name entry: draw the item, with a blinking cursor box under the character
 * being edited. */

typedef struct {
    s32 cursor;    /* 0x00 */
    f32 timer;     /* 0x04 */
    s32 length;    /* 0x08 */
    char name[12]; /* 0x0C */
} NameEntry; /* 0x18 bytes */

typedef struct {
    u8 pad0[0x14];
    s32 x; /* 0x14 */
    u8 pad18[8];
    s32 y; /* 0x20 */
} MenuItem;

typedef struct {
    u8 pad0[0x1C];
    void *player; /* 0x1C */
    u8 pad20[0x10];
    f32 fade;     /* 0x30 */
    f32 alpha;    /* 0x34 */
} MenuEvent;

extern u8 D_80119870[];
extern NameEntry D_800C1FB0[];
extern s32 D_800C2030; /* screen width */
extern s32 D_800C1BD0;

s32 func_0025340C(void *state, void *player);
void func_0027AF4C(void);
void func_002778A8(s32 x0, s32 y0, s32 x1, s32 y1, s32 border, s32 invert, s32 red, s32 green, s32 blue, s32 alpha);
void func_00416168(void *menu, MenuItem *item, void *arg2, MenuEvent *event);

void func_0045646C(void *menu, MenuItem *item, void *arg2, MenuEvent *event) {
    s32 i = func_0025340C(D_80119870, event->player);
    f32 charWidth;
    f32 alpha;
    s32 x;

    if (D_800C1FB0[i].timer <= 6.0f) {
        charWidth = (D_800C2030 * 9) / 284;
        x = item->x + D_800C1FB0[i].cursor * charWidth;
        alpha = event->fade * (event->alpha * 64.0f);
        D_800C1BD0 = -1;
        func_0027AF4C();
        func_002778A8(x, item->y - 2, x + charWidth, item->y, 0, 0, 255, 255, 255, (u32)alpha & 0xFF);
    }
    func_00416168(menu, item, arg2, event);
}
