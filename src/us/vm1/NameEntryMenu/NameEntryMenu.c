#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/player.h"
/* Player-name menu: highlights the row of the player the menu belongs to
 * and copies that player's name (trailing spaces trimmed, nothing when it is
 * still the default "player" name) into the edit buffer. */



#define ROW_SELECTED 0x01800000

static __inline__ s32 nameCmp(u8 *a, u8 *b, s32 n) {
    u8 c1;
    u8 c2;

    do {
        c1 = *a++;
        c2 = *b++;
        n--;
        if (c1 != c2 || c1 == 0) {
            break;
        }
    } while (n == 0); /* sic: compares only the first character */
    return c1 - c2;
}

void NameEntryMenu_Init(Menu *menu) {
    sptr index;
    s32 i;
    s32 spaces;
    u8 *name;
    u8 *dst;

    index = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, menu->player);
    switch (index) {
    case 0:
    default:
        menu->items[1].flags |= ROW_SELECTED;
        menu->items[2].flags &= ~ROW_SELECTED;
        menu->items[3].flags &= ~ROW_SELECTED;
        menu->items[4].flags &= ~ROW_SELECTED;
        break;
    case 1:
        menu->items[2].flags |= ROW_SELECTED;
        menu->items[1].flags &= ~ROW_SELECTED;
        menu->items[3].flags &= ~ROW_SELECTED;
        menu->items[4].flags &= ~ROW_SELECTED;
        break;
    case 2:
        menu->items[3].flags |= ROW_SELECTED;
        menu->items[1].flags &= ~ROW_SELECTED;
        menu->items[2].flags &= ~ROW_SELECTED;
        menu->items[4].flags &= ~ROW_SELECTED;
        break;
    case 3:
        menu->items[4].flags |= ROW_SELECTED;
        menu->items[1].flags &= ~ROW_SELECTED;
        menu->items[2].flags &= ~ROW_SELECTED;
        menu->items[3].flags &= ~ROW_SELECTED;
        break;
    }
    gNameEntries[index].timer = 0;
    dst = gNameEntries[index].name;
    name = gEngineApp_m_Settings.slots[index].m_Name;
    i = 8;
    spaces = 0;
    while (i > 0) {
        i--;
        if (name[i] != ' ') {
            break;
        }
        spaces++;
    }
    if (nameCmp(name, "player", 6) == 0) {
        spaces = 8;
    }
    for (i = 0; i < 8 - spaces; i++) {
        *dst++ = *name++;
    }
    *dst = 0;
    gNameEntries[index].cursor = 8 - spaces;
    gNameEntries[index].length = 8 - spaces;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80445964_de_7.c, func_80445964_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/engine.h"

extern int Controller_MenuLeft(struct ControllerProfile *object);
extern int Controller_MenuRight(struct ControllerProfile *object);
/* the menu by its host layout (round 10 y4): the view's s32 input at N64 0x20 is
 * Menu.controller, a pointer at another host offset, and the call cast it to s32 (macOS, statics
 * above 4 GB: Controller_MenuLeft crashed when a soak run opened the cheat entry; Linux non-PIE
 * statics fit in 32 bits) */
#define T2_MENU_INPUT(m) (((Menu *) (m))->controller)
#define T2_MENU_LEFT(m) Controller_MenuLeft(T2_MENU_INPUT(m))
#define T2_MENU_RIGHT(m) Controller_MenuRight(T2_MENU_INPUT(m))

s32 NameEntryMenu_UpdateName(void *unused, MenuRecAt20V1 *menu) {
    sptr i;
    f32 t;
    i = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, (uptr) ((Menu *) menu)->player);
    t = gNameEntries[i].timer + gFrameIncrement;
    gNameEntries[i].timer = t;
    if (t >= (15.0f)) {
        gNameEntries[i].timer = t - (15.0f);
    }
    if (menu->state == 1) {
        if (T2_MENU_LEFT(menu) != 0 && gNameEntries[i].cursor > 0) {
            gNameEntries[i].cursor--;
            gNameEntries[i].timer = 0.0f;
        } else if (T2_MENU_RIGHT(menu) != 0) {
            if (gNameEntries[i].cursor < gNameEntries[i].length) {
                gNameEntries[i].cursor++;
                gNameEntries[i].timer = 0.0f;
            }
        }
    }
    return 0;
}

/* Name entry: append the item's character, up to 8. */
s32 NameEntryMenu_AddChar(MenuItem *a, Menu *c) {
    sptr i = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, c->player);

    gNameEntries[i].timer = 0;
    if (gNameEntries[i].length == 8) {
        return 0;
    }
    gNameEntries[i].name[gNameEntries[i].cursor] = a->text[0];
    gNameEntries[i].cursor++;
    gNameEntries[i].length++;
    gNameEntries[i].name[gNameEntries[i].length] = 0;
    return 0;
}
s32 NameEntryMenu_CompareName(u8 *a, u8 *b, s32 count);

s32 NameEntryMenu_CompareName(u8 *a, u8 *b, s32 count) {
    u8 c;
    u8 d;
    do {
        c = *a++;
        d = *b++;
        count--;
    } while (c == d && c != 0 && count == 0);
    return c == d;
}

#include "globals/render.h"
#include "globals/menu.h"

/* Name entry item draw callback: while the cursor blink timer of the menu's
 * player is low, draws the text cursor (9 virtual pixels wide, scaled to the
 * screen) under the edited column, faded by the window alpha, then draws the
 * item as usual. As EnterCheatMenu_DrawTextField. */


extern void Hud_SetupFillMode(void);
extern void Hud_DrawBevelBox(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j);
extern void MenuItem_DrawText(void *menu, MenuItemLayout *item, void *arg2, Menu *win);

/* The menu's items and its MenuDesc (MpMenu opens it). Its .data is the object's own
 * (versions/turok2.us.yaml: a `.data` subsegment cut out of virtual_rodata_0), and the key
 * strings are its .rodata: cc1 writes an initializer's strings where the variable is output,
 * the last element's first, which is why the ROM has " ", "9" .. "0", "z" .. "a" between
 * NameEntryMenu_UpdateName's 15.0f and NameEntryMenu_DrawName's literals. The title and the
 * last three keys' texts are the engine's (globals/menu_text.h); the four name fields are
 * the players' name buffers (gNameEntries[].name). The port keeps the generated copy (tools/port/gendata.py:
 * the s32 `data` slot holds an N64 address, a handle there). */
#include "globals/menu_text.h"
#include "globals/menu_desc.h"

#ifndef TARGET_PORT
void NameEntryMenu_DrawName(void *menu, MenuItemLayout *item, void *arg2, Menu *win);
s32 NameEntryMenu_DeleteChar(s32 menu, Menu *c);
s32 NameEntryMenu_Confirm(s32 menu, Menu *c, s32 pool);
s32 NameEntryMenu_IsMatchOver(void);
s32 Menu_Draw();

MenuItemDesc gNameEntryMenuItems[] = {
    { 0, { 0 }, 0x00000041, 0, 0, { 0x00, 0x00, 0xFF, 0x01 }, (s32)sNameEntryText_Edit, (void *)MenuItem_DrawText, 0, 0, 0, 0 },
    { 0, { 0 }, 0x00210101, 0, 8, { 0x00, 0x00, 0xFF, 0x01 }, (s32)gNameEntries[0].name, (void *)NameEntryMenu_DrawName, (void *)NameEntryMenu_UpdateName, 0, 0, 0 },
    { 0, { 0 }, 0x00210101, 0, 8, { 0x00, 0x00, 0xFF, 0x01 }, (s32)gNameEntries[1].name, (void *)NameEntryMenu_DrawName, (void *)NameEntryMenu_UpdateName, 0, 0, 0 },
    { 0, { 0 }, 0x00210101, 0, 8, { 0x00, 0x00, 0xFF, 0x01 }, (s32)gNameEntries[2].name, (void *)NameEntryMenu_DrawName, (void *)NameEntryMenu_UpdateName, 0, 0, 0 },
    { 0, { 0 }, 0x00210101, 0, 8, { 0x00, 0x00, 0xFF, 0x01 }, (s32)gNameEntries[3].name, (void *)NameEntryMenu_DrawName, (void *)NameEntryMenu_UpdateName, 0, 0, 0 },
    { 0, { 0 }, 0x00010110, 0, 10, { 0x07, 0x01, 0x20, 0x08 }, (s32)"a", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0x20, 0x08 }, (s32)"b", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0x20, 0x08 }, (s32)"c", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0x20, 0x08 }, (s32)"d", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0x20, 0x08 }, (s32)"e", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0x20, 0x08 }, (s32)"f", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0x20, 0x08 }, (s32)"g", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0xF9, 0x20, 0x08 }, (s32)"h", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00010110, 0, 4, { 0x07, 0x01, 0xF8, 0x08 }, (s32)"i", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0xF8, 0x08 }, (s32)"j", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0xF8, 0x08 }, (s32)"k", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0xF8, 0x08 }, (s32)"l", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0xF8, 0x08 }, (s32)"m", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0xF8, 0x08 }, (s32)"n", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0xF8, 0x08 }, (s32)"o", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0xF9, 0xF8, 0x08 }, (s32)"p", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00010110, 0, 4, { 0x07, 0x01, 0xF8, 0x08 }, (s32)"q", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0xF8, 0x08 }, (s32)"r", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0xF8, 0x08 }, (s32)"s", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0xF8, 0x08 }, (s32)"t", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0xF8, 0x08 }, (s32)"u", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0xF8, 0x08 }, (s32)"v", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0xF8, 0x08 }, (s32)"w", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0xF9, 0xF8, 0x08 }, (s32)"x", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00010110, 0, 4, { 0x07, 0x01, 0xF8, 0x08 }, (s32)"y", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0xF8, 0x08 }, (s32)"z", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0xF8, 0x08 }, (s32)"0", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0xF8, 0x08 }, (s32)"1", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0xF8, 0x08 }, (s32)"2", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0xF8, 0x08 }, (s32)"3", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0xF8, 0x08 }, (s32)"4", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0xF9, 0xF8, 0x08 }, (s32)"5", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00010110, 0, 4, { 0x07, 0x01, 0xF8, 0xE0 }, (s32)"6", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0xF8, 0xE0 }, (s32)"7", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0xF8, 0xE0 }, (s32)"8", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0xF8, 0xE0 }, (s32)"9", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0xF8, 0xE0 }, (s32)" ", (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0xF8, 0xE0 }, (s32)sNameEntryText_Glyph2, (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_AddChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0x01, 0xF8, 0xE0 }, (s32)sNameEntryText_Glyph3, (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_DeleteChar, 0, 0 },
    { 0, { 0 }, 0x00024110, 6, 0, { 0xFF, 0xF9, 0xF8, 0xE0 }, (s32)sNameEntryText_Glyph4, (void *)MenuItem_DrawText, 0, (void *)NameEntryMenu_Confirm, 0, 0 },
};

MenuDesc gNameEntryMenuDesc = {
    gNameEntryMenuItems, 45, { 0 }, (void *)NameEntryMenu_Init, 0, (void *)Menu_Draw,
    NameEntryMenu_IsMatchOver, 5, 0, { 0 }, 0,
};
#endif


void NameEntryMenu_DrawName(void *menu, MenuItemLayout *item, void *arg2, Menu *win) {
    sptr index;
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

/* Name entry: delete the last character. */
s32 NameEntryMenu_DeleteChar(s32 menu, Menu *c) {
    sptr i = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, c->player);

    gNameEntries[i].timer = 0;
    if (gNameEntries[i].length == 0) {
        return 0;
    }
    gNameEntries[i].length--;
    gNameEntries[i].cursor--;
    gNameEntries[i].name[gNameEntries[i].cursor] = 0;
    return 0;
}

#include "structs/player.h"

extern void PlayerOptions_SetDefaultName(void *options);
extern void MenuItem_SelectSubmenu(sptr menu, Menu *c, sptr pool);
/* Name entry: confirm. Copies the name (blanks for unused characters) into the setup slot;

 * an empty name resets the player's options. */
s32 NameEntryMenu_Confirm(s32 menu, Menu *c, s32 pool) {
    CPlayer *player = c->player;
    sptr i = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, player);
    u8 *src = gNameEntries[i].name;
    u8 *dst = gEngineApp_m_Game.m_Settings.slots[0].m_Name + (i << 6);
    s32 n = 0;
    s32 j;
    u8 ch;

    for (j = 0; j < 8; j++) {
        ch = *src++;
        if (ch == 0) {
            ch = ' ';
        } else {
            n++;
        }
        *dst++ = ch;
    }
    *dst = 0;
    if (n == 0) {
        PlayerOptions_SetDefaultName(player->m_pOptions);
    }
    MenuItem_SelectSubmenu(menu, c, pool);
    return 1;
}

s32 NameEntryMenu_IsMatchOver(void)
{
  s32 var_a0;
  GameMatch *new_var;
  var_a0 = 0;
  new_var = &gEngineApp_m_Settings.match;
  if (((gEngineApp_m_Settings.match.teamOver != 0) || (new_var->over != 0)) || (new_var->done != 0))
  {
    var_a0 = 1;
  }
  return var_a0;
}
