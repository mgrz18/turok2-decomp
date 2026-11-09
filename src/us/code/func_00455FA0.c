#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/player.h"

/* Name entry: highlight the current player's column and load the player's
 * name (trailing spaces trimmed; the default name "player" counts as empty)
 * into the editing buffer. */

#define ENTRY_SELECTED 0x01800000

extern u8 D_8011AB8B[];


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
    s32 i = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, event->player);
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
    src = &D_8011AB8B[i * 64];
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
