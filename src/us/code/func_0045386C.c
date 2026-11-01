#include "common.h"

/* Text entry item: draws the item, with a blinking 2-pixel cursor bar under the character being edited
 * (sibling of func_0045646C, for a single entry). */

typedef struct {
    s32 cursor; /* 0x00 */
    f32 timer;  /* 0x04 */
} TextEntry;

typedef struct {
    u8 pad0[0x14];
    s32 x; /* 0x14 */
    u8 pad18[8];
    s32 y; /* 0x20 */
} MenuItem;

typedef struct {
    u8 pad0[0x30];
    f32 fade;  /* 0x30 */
    f32 alpha; /* 0x34 */
} MenuEvent;

extern TextEntry D_800C1F80;
extern s32 D_800C2030; /* screen width */
extern s32 D_800C1BD0;

void func_0027AF4C(void);
void func_002778A8(s32 x0, s32 y0, s32 x1, s32 y1, s32 border, s32 invert, s32 red, s32 green, s32 blue, s32 alpha);
void func_00416168(void *menu, MenuItem *item, void *arg2, MenuEvent *event);

void func_0045386C(void *menu, MenuItem *item, void *arg2, MenuEvent *event) {
    f32 charWidth;
    f32 alpha;
    s32 x;

    if (D_800C1F80.timer <= 6.0f) {
        charWidth = (D_800C2030 * 11) / 284;
        x = item->x + D_800C1F80.cursor * charWidth;
        alpha = event->fade * (event->alpha * 128.0f);
        D_800C1BD0 = -1;
        func_0027AF4C();
        func_002778A8(x, item->y, x + charWidth, item->y + 2, 0, 0, 255, 255, 255, (u32)alpha & 0xFF);
    }
    func_00416168(menu, item, arg2, event);
}
