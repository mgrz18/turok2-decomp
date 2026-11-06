#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/render.h"
#include "globals/menu.h"

/* Name entry: draw the item, with a blinking cursor box under the character
 * being edited. */


s32 CPlayerPool__GetIndex(void *state, void *player);
void Hud_SetupFillMode(void);
void Hud_DrawBevelBox(s32 x0, s32 y0, s32 x1, s32 y1, s32 border, s32 invert, s32 red, s32 green, s32 blue, s32 alpha);
void MenuItem_DrawText_2(void *menu, MenuItemLayout *item, void *arg2, Menu *event);

void NameEntryMenu_DrawName_2(void *menu, MenuItemLayout *item, void *arg2, Menu *event) {
    s32 i = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, event->player);
    f32 charWidth;
    f32 alpha;
    s32 x;

    if (gNameEntries[i].timer <= 6.0f) {
        charWidth = (gScreenWidth * 9) / 284;
        x = item->left + gNameEntries[i].cursor * charWidth;
        alpha = event->fade * (event->alpha * 64.0f);
        gMenuCurrentFont = -1;
        Hud_SetupFillMode();
        Hud_DrawBevelBox(x, item->bottom - 2, x + charWidth, item->bottom, 0, 0, 255, 255, 255, (u32)alpha & 0xFF);
    }
    MenuItem_DrawText_2(menu, item, arg2, event);
}
