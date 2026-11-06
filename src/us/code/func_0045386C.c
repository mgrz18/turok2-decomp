#include "common.h"
#include "structs/menu.h"

/* Text entry item: draws the item, with a blinking 2-pixel cursor bar under the character being edited
 * (sibling of NameEntryMenu_DrawName_2, for a single entry). */

extern s32 D_800C2030; /* screen width */
extern s32 D_800C1BD0;

void Hud_SetupFillMode(void);
void Hud_DrawBevelBox(s32 x0, s32 y0, s32 x1, s32 y1, s32 border, s32 invert, s32 red, s32 green, s32 blue, s32 alpha);
void MenuItem_DrawText(void *menu, MenuItemLayout *item, void *arg2, Menu *event);

void EnterCheatMenu_DrawTextField_2(void *menu, MenuItemLayout *item, void *arg2, Menu *event) {
    f32 charWidth;
    f32 alpha;
    s32 x;

    if (D_800C1F80.timer <= 6.0f) {
        charWidth = (D_800C2030 * 11) / 284;
        x = item->left + D_800C1F80.cursor * charWidth;
        alpha = event->fade * (event->alpha * 128.0f);
        D_800C1BD0 = -1;
        Hud_SetupFillMode();
        Hud_DrawBevelBox(x, item->bottom, x + charWidth, item->bottom + 2, 0, 0, 255, 255, 255, (u32)alpha & 0xFF);
    }
    MenuItem_DrawText(menu, item, arg2, event);
}
