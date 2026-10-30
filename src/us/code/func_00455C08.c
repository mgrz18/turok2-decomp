#include "common.h"

/* Menu text item: copy a player's 8-character name into the item's text and
 * print one of the current player's u16 counters (per opponent) after it. */

typedef struct {
    s32 index;  /* 0x00 */
    u8 pad4[0x10];
    char *text; /* 0x14 */
} MenuItem;

typedef struct {
    u8 pad0[0x1C];
    u8 *player; /* 0x1C */
} PlayerEvent;

typedef struct {
    u8 pad0[0x10];
    u16 teamCounts[8]; /* 0x10 */
    u16 counts[8];     /* 0x20 */
} PlayerOptions;

extern s32 D_8011ACE4;
extern char D_8011AB8B[];
extern char D_800C1338[];

s32 func_004160F0(MenuItem *item);
void func_0029E3E0(char *dst, char *fmt, s32 value);

s32 func_00455C08(MenuItem *item, PlayerEvent *event) {
    s32 other = item->index - 2;
    u8 *player = event->player;
    s32 value;
    char *dst;
    char *src;
    s32 i;

    if (D_8011ACE4 != 0) {
        value = ((PlayerOptions *)*(void **)(player + 0x518))->teamCounts[other];
    } else {
        value = ((PlayerOptions *)*(void **)(player + 0x518))->counts[other];
    }
    dst = item->text;
    src = &D_8011AB8B[other * 64];
    for (i = 0; i < 8; i++) {
        *dst++ = *src++;
    }
    func_0029E3E0(item->text + (func_004160F0(item) - 2), D_800C1338, value);
    return 0;
}
