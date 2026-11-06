#include "common.h"
#include "structs/menu.h"

/* Menu item draw callback: while the cursor blink timer is low, draws the
 * highlight box (11 virtual pixels wide, scaled to the screen) at the cursor
 * column, faded by the item's alpha, then draws the item as usual. */

extern s32 gScreenWidth;
extern s32 gMenuCurrentFont;

extern void Hud_SetupFillMode(void);
extern void Hud_DrawBevelBox(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j);
extern void MenuItem_DrawText(void *menu, MenuItemLayout *item, s32 arg2, Menu *win);

void EnterCheatMenu_DrawTextField(void *menu, MenuItemLayout *item, s32 arg2, Menu *win) {
    f32 width;
    f32 alpha;
    s32 x;

    if (gCheatEntry.timer <= 6.0f) {
        width = gScreenWidth * 11 / 284;
        x = item->left + gCheatEntry.cursor * width;
        alpha = win->fade * (win->alpha * 128.0f);
        gMenuCurrentFont = -1;
        Hud_SetupFillMode();
        Hud_DrawBevelBox(x, item->bottom, x + width, item->bottom + 2, 0, 0, 0xFF, 0xFF, 0xFF, (u8) alpha);
    }
    MenuItem_DrawText(menu, item, arg2, win);
}
