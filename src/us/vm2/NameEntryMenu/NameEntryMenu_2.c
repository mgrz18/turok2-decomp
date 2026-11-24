#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/player.h"
/* Name entry: highlight the current player's column and load the player's
 * name (trailing spaces trimmed; the default name "player" counts as empty)
 * into the editing buffer. */

#define ENTRY_SELECTED 0x01800000

static __inline__ s32 nameEquals(u8 *a, u8 *b, s32 count) {
    u8 c;
    u8 d;
    do {
        c = *a++;
        d = *b++;
        count--;
    } while (c == d && c != 0 && count == 0);
    return c == d;
}

void NameEntryMenu_Init_2(Menu *event) {
    sptr i = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, event->player);
    s32 trailing;
    char *dst;
    u8 *src;
    s32 n;
    s32 k;

    switch (i) {
    case 0:
    default:
        event->items[1].flags |= ENTRY_SELECTED;
        event->items[2].flags &= ~ENTRY_SELECTED;
        event->items[3].flags &= ~ENTRY_SELECTED;
        event->items[4].flags &= ~ENTRY_SELECTED;
        break;
    case 1:
        event->items[2].flags |= ENTRY_SELECTED;
        event->items[1].flags &= ~ENTRY_SELECTED;
        event->items[3].flags &= ~ENTRY_SELECTED;
        event->items[4].flags &= ~ENTRY_SELECTED;
        break;
    case 2:
        event->items[3].flags |= ENTRY_SELECTED;
        event->items[1].flags &= ~ENTRY_SELECTED;
        event->items[2].flags &= ~ENTRY_SELECTED;
        event->items[4].flags &= ~ENTRY_SELECTED;
        break;
    case 3:
        event->items[4].flags |= ENTRY_SELECTED;
        event->items[1].flags &= ~ENTRY_SELECTED;
        event->items[2].flags &= ~ENTRY_SELECTED;
        event->items[3].flags &= ~ENTRY_SELECTED;
        break;
    }

    gNameEntries[i].timer = 0.0f;
    dst = gNameEntries[i].name;
    src = gEngineApp_m_Settings.slots[i].m_Name;
    for (n = 8, trailing = 0; n > 0; trailing++) {
        if (src[--n] != ' ') {
            break;
        }
    }
    if (nameEquals(src, "player", 6)) {
        trailing = 8;
    }
    for (k = 0; k < 8 - trailing; k++) {
        *dst++ = *src++;
    }
    *dst = '\0';
    gNameEntries[i].cursor = 8 - trailing;
    gNameEntries[i].length = 8 - trailing;
}

#include "globals/engine.h"

typedef struct { s16 unk0; u8 pad[0x1E]; void *unk20; } O06A2;
extern s32 Controller_MenuLeft(void *a);
extern s32 Controller_MenuRight(void *a);
extern f32 D_800C1348;

s32 NameEntryMenu_UpdateName_2(s32 a, O06A2 *b) {
    sptr n = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, *(s32 *)((u8 *)b + 0x1C));
    f32 v = gNameEntries[n].timer + gFrameIncrement;
    gNameEntries[n].timer = v;
    if (D_800C1348 <= v) gNameEntries[n].timer = v - D_800C1348;
    if (b->unk0 == 1) {
        if (Controller_MenuLeft(b->unk20) && gNameEntries[n].cursor > 0) {
            gNameEntries[n].cursor--;
            gNameEntries[n].timer = 0;
        } else if (Controller_MenuRight(b->unk20) && gNameEntries[n].cursor < gNameEntries[n].length) {
            gNameEntries[n].cursor++;
            gNameEntries[n].timer = 0;
        }
    }
    return 0;
}

/* Name entry: append the selected character to the current player's name. */



s32 NameEntryMenu_AddChar_2(MenuItem *item, Menu *event) {
    sptr i = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, event->player);

    gNameEntries[i].timer = 0.0f;
    if (gNameEntries[i].length == 8) {
        return 0;
    }
    gNameEntries[i].name[gNameEntries[i].cursor] = item->text[0];
    gNameEntries[i].cursor++;
    gNameEntries[i].length++;
    gNameEntries[i].name[gNameEntries[i].length] = '\0';
    return 0;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80445964_de_8.c, func_80445B88_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

s32 NameEntryMenu_CompareName_2(u8 *a, u8 *b, s32 count);

s32 NameEntryMenu_CompareName_2(u8 *a, u8 *b, s32 count) {
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

/* Name entry: draw the item, with a blinking cursor box under the character
 * being edited. */


void Hud_SetupFillMode(void);
void Hud_DrawBevelBox(s32 x0, s32 y0, s32 x1, s32 y1, s32 border, s32 invert, s32 red, s32 green, s32 blue, s32 alpha);
void MenuItem_DrawText_2(void *menu, MenuItemLayout *item, void *arg2, Menu *event);

void NameEntryMenu_DrawName_2(void *menu, MenuItemLayout *item, void *arg2, Menu *event) {
    sptr i = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, event->player);
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

struct ObjRecAt5D8;
typedef struct ObjRecAt5D8 ObjRecAt5D8;
struct ObjRecAt5D8 {
    char pad0[0x5D8];
    void * unk5D8;
};
struct EntryRecAt1C;
struct EntryRecAt1C {
    char pad[0x1C];
    ObjRecAt5D8 *unk1C;
};
typedef struct EntryRecAt1C EntryRecAt1C;

s32 NameEntryMenu_DeleteChar_2(s32 unused, EntryRecAt1C *arg1) {
    sptr i;
    s32 count;
    i = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, arg1->unk1C);
    count = gNameEntries[i].length;
    gNameEntries[i].timer = 0;
    if (count == 0) {
        return 0;
    }
    gNameEntries[i].length = count - 1;
    gNameEntries[i].cursor--;
    gNameEntries[i].name[gNameEntries[i].cursor] = 0;
    return 0;
}

/* Name entry accepted: copy the edited name (blank-padded to 8 characters)
 * into the current player's record; an empty name restores the default one. */


void PlayerOptions_SetDefaultName_2(void *options);
void MenuItem_SelectSubmenu_2(void *menu, Menu *event, void *arg2);

s32 NameEntryMenu_Confirm_2(void *menu, Menu *event, void *arg2) {
    u8 *player = event->player;
    sptr i = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, player);
    u8 *src = (u8 *)gNameEntries[i].name;
    u8 *dst = gEngineApp_m_Game.m_Settings.slots[0].m_Name + i * 64;
    s32 count = 0;
    s32 k;
    u8 c;

    for (k = 0; k < 8; k++) {
        c = *src++;
        if (c == 0) {
            c = ' ';
        } else {
            count++;
        }
        *dst++ = c;
    }
    *dst = '\0';
    if (count == 0) {
        PlayerOptions_SetDefaultName_2(*(void **)(player + 0x518));
    }
    MenuItem_SelectSubmenu_2(menu, event, arg2);
    return 1;
}

#include "m2c_macros.h"

s32 NameEntryMenu_IsMatchOver_2(void) {
    return gEngineApp_m_Settings.match.over;
}
