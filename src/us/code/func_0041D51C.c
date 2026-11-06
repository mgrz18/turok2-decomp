#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"

/* Name entry item draw callback: while the cursor blink timer of the menu's
 * player is low, draws the text cursor (9 virtual pixels wide, scaled to the
 * screen) under the edited column, faded by the window alpha, then draws the
 * item as usual. As EnterCheatMenu_DrawTextField. */

extern s32 gScreenWidth;
extern s32 gMenuCurrentFont;

extern s32 CPlayerPool__GetIndex(void *players, void *player);
extern void Hud_SetupFillMode(void);
extern void Hud_DrawBevelBox(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j);
extern void MenuItem_DrawText(void *menu, MenuItemLayout *item, s32 arg2, Menu *win);

void NameEntryMenu_DrawName(void *menu, MenuItemLayout *item, s32 arg2, Menu *win) {
    s32 index;
    f32 width;
    f32 alpha;
    s32 x;

    index = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, win->player);
    if (gNameEntries[index].timer <= 6.0f) {
        width = gScreenWidth * 9 / 284;
        x = item->left + gNameEntries[index].cursor * width;
        alpha = win->fade * (win->alpha * 64.0f);
        gMenuCurrentFont = -1;
        Hud_SetupFillMode();
        Hud_DrawBevelBox(x, item->bottom - 2, x + width, item->bottom, 0, 0, 0xFF, 0xFF, 0xFF, (u8) alpha);
    }
    MenuItem_DrawText(menu, item, arg2, win);
}
