#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"

/* Name entry: draw the item, with a blinking cursor box under the character
 * being edited. */

extern s32 D_800C2030; /* screen width */
extern s32 D_800C1BD0;

s32 CPlayerPool__GetIndex(void *state, void *player);
void Hud_SetupFillMode(void);
void Hud_DrawBevelBox(s32 x0, s32 y0, s32 x1, s32 y1, s32 border, s32 invert, s32 red, s32 green, s32 blue, s32 alpha);
void MenuItem_DrawText(void *menu, MenuItemLayout *item, void *arg2, Menu *event);

void NameEntryMenu_DrawName_2(void *menu, MenuItemLayout *item, void *arg2, Menu *event) {
    s32 i = CPlayerPool__GetIndex(&D_80119870.m_PlayerList, event->player);
    f32 charWidth;
    f32 alpha;
    s32 x;

    if (D_800C1FB0[i].timer <= 6.0f) {
        charWidth = (D_800C2030 * 9) / 284;
        x = item->left + D_800C1FB0[i].cursor * charWidth;
        alpha = event->fade * (event->alpha * 64.0f);
        D_800C1BD0 = -1;
        Hud_SetupFillMode();
        Hud_DrawBevelBox(x, item->bottom - 2, x + charWidth, item->bottom, 0, 0, 255, 255, 255, (u32)alpha & 0xFF);
    }
    MenuItem_DrawText(menu, item, arg2, event);
}
