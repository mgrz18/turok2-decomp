#include "common.h"

/* Name entry item draw callback: while the cursor blink timer of the menu's
 * player is low, draws the text cursor (9 virtual pixels wide, scaled to the
 * screen) under the edited column, faded by the window alpha, then draws the
 * item as usual. As func_0041A904. */

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 x;
    /* 0x18 */ u8 pad18[0x20 - 0x18];
    /* 0x20 */ s32 y;
} MenuItem;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ void *player;
    /* 0x20 */ u8 pad20[0x30 - 0x20];
    /* 0x30 */ f32 alpha;
    /* 0x34 */ f32 fade;
} MenuWindow;

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
extern void func_00416168(void *menu, MenuItem *item, s32 arg2, MenuWindow *win);

void func_0041D51C(void *menu, MenuItem *item, s32 arg2, MenuWindow *win) {
    s32 index;
    f32 width;
    f32 alpha;
    s32 x;

    index = func_0025340C(D_80119870, win->player);
    if (D_800C1FB0[index].timer <= 6.0f) {
        width = D_800C2030 * 9 / 284;
        x = item->x + D_800C1FB0[index].column * width;
        alpha = win->alpha * (win->fade * 64.0f);
        D_800C1BD0 = -1;
        func_0027AF4C();
        func_002778A8(x, item->y - 2, x + width, item->y, 0, 0, 0xFF, 0xFF, 0xFF, (u8) alpha);
    }
    func_00416168(menu, item, arg2, win);
}
