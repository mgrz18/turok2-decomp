#include "common.h"
#include "structs/menu.h"

/* Name entry item draw callback: while the cursor blink timer of the menu's
 * player is low, draws the text cursor (9 virtual pixels wide, scaled to the
 * screen) under the edited column, faded by the window alpha, then draws the
 * item as usual. As func_0041A904. */

typedef struct {
    /* 0x00 */ s32 column;
    /* 0x04 */ f32 timer;
    /* 0x08 */ s32 end;
    /* 0x0C */ char text[0x18 - 0xC];
} NameEdit;

extern u8 D_80119870[];
extern NameEdit D_800C1FB0[];
extern s32 D_800C2030;
extern s32 D_800C1BD0;

extern s32 func_0025340C(void *players, void *player);
extern void func_0027AF4C(void);
extern void func_002778A8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j);
extern void func_00416168(void *menu, MenuItemLayout *item, s32 arg2, Menu *win);

void func_0041D51C(void *menu, MenuItemLayout *item, s32 arg2, Menu *win) {
    s32 index;
    f32 width;
    f32 alpha;
    s32 x;

    index = func_0025340C(D_80119870, win->player);
    if (D_800C1FB0[index].timer <= 6.0f) {
        width = D_800C2030 * 9 / 284;
        x = item->left + D_800C1FB0[index].column * width;
        alpha = win->fade * (win->alpha * 64.0f);
        D_800C1BD0 = -1;
        func_0027AF4C();
        func_002778A8(x, item->bottom - 2, x + width, item->bottom, 0, 0, 0xFF, 0xFF, 0xFF, (u8) alpha);
    }
    func_00416168(menu, item, arg2, win);
}
