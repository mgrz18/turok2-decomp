#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "globals/game.h"

/* Player-name menu: highlights the row of the player the menu belongs to
 * and copies that player's name (trailing spaces trimmed, nothing when it is
 * still the default "player" name) into the edit buffer. */

extern u8 D_8011AB8B[][0x40];   /* player options +0x33: names */

extern s32 CPlayerPool__GetIndex(void *players, void *player);

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
    s32 index;
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
    name = D_8011AB8B[index];
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
