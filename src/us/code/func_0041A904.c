#include "common.h"
#include "structs/menu.h"

/* Menu item draw callback: while the cursor blink timer is low, draws the
 * highlight box (11 virtual pixels wide, scaled to the screen) at the cursor
 * column, faded by the item's alpha, then draws the item as usual. */

typedef struct {
    /* 0x0 */ s32 column;
    /* 0x4 */ f32 timer;
} Cursor;

extern Cursor D_800C1F80;
extern s32 D_800C2030;
extern s32 D_800C1BD0;

extern void func_0027AF4C(void);
extern void func_002778A8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j);
extern void func_00416168(void *menu, MenuItemLayout *item, s32 arg2, Menu *win);

void func_0041A904(void *menu, MenuItemLayout *item, s32 arg2, Menu *win) {
    f32 width;
    f32 alpha;
    s32 x;

    if (D_800C1F80.timer <= 6.0f) {
        width = D_800C2030 * 11 / 284;
        x = item->left + D_800C1F80.column * width;
        alpha = win->fade * (win->alpha * 128.0f);
        D_800C1BD0 = -1;
        func_0027AF4C();
        func_002778A8(x, item->bottom, x + width, item->bottom + 2, 0, 0, 0xFF, 0xFF, 0xFF, (u8) alpha);
    }
    func_00416168(menu, item, arg2, win);
}
