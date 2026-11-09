#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/game.h"

/* Menu text item: copy a player's 8-character name into the item's text and
 * print one of the current player's u16 counters (per opponent) after it. */

extern char D_8011AB8B[];
extern char D_800C1338[];

s32 MenuItem_TextLength_2(MenuItem *item);
#ifdef TARGET_PORT
/* variadic on the host: arm64 Apple passes variadic arguments on the stack (docs/port/MACOS.md) */
void sprintf(char *dst, const char *fmt, ...);
#else
void sprintf(char *dst, char *fmt, s32 value);
#endif

s32 StatsMenu_PlayerRowText_2(MenuItem *item, Menu *event) {
    s32 other = item->index - 2;
    u8 *player = (u8 *) event->player;
    s32 value;
    char *dst;
    char *src;
    s32 i;

    if (gEngineApp_m_Settings.match.tag != 0) {
        value = ((CPlayerOptions *)*(void **)(player + 0x518))->m_Tagged[other];
    } else {
        value = ((CPlayerOptions *)*(void **)(player + 0x518))->m_Killed[other];
    }
    dst = item->text;
    src = &D_8011AB8B[other * 64];
    for (i = 0; i < 8; i++) {
        *dst++ = *src++;
    }
    sprintf(item->text + (MenuItem_TextLength_2(item) - 2), D_800C1338, value);
    return 0;
}
