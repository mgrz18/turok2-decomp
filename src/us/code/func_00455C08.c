#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"

/* Menu text item: copy a player's 8-character name into the item's text and
 * print one of the current player's u16 counters (per opponent) after it. */

extern s32 D_8011ACE4;
extern char D_8011AB8B[];
extern char D_800C1338[];

s32 func_004160F0(MenuItem *item);
void func_0029E3E0(char *dst, char *fmt, s32 value);

s32 func_00455C08(MenuItem *item, Menu *event) {
    s32 other = item->index - 2;
    u8 *player = (u8 *) event->player;
    s32 value;
    char *dst;
    char *src;
    s32 i;

    if (D_8011ACE4 != 0) {
        value = ((CPlayerOptions *)*(void **)(player + 0x518))->m_Tagged[other];
    } else {
        value = ((CPlayerOptions *)*(void **)(player + 0x518))->m_Killed[other];
    }
    dst = item->text;
    src = &D_8011AB8B[other * 64];
    for (i = 0; i < 8; i++) {
        *dst++ = *src++;
    }
    func_0029E3E0(item->text + (func_004160F0(item) - 2), D_800C1338, value);
    return 0;
}
