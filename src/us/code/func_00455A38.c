#include "common.h"

/* Menu text item: print one u16 stat of the current player's record (64-byte
 * records, field at 0x8011AB5E) at the end of the item's text. */

typedef struct {
    u8 pad0[0x14];
    char *text; /* 0x14 */
} MenuItem;

typedef struct {
    u8 pad0[0x1C];
    void *player; /* 0x1C */
} PlayerEvent;

extern u8 D_80119870[];
extern u16 D_8011AB5E[];
extern char D_800C1330[];

s32 func_0025340C(void *state, void *player);
s32 func_004160F0(MenuItem *item);
void func_0029E3E0(char *dst, char *fmt, s32 value);

s32 func_00455A38(MenuItem *item, PlayerEvent *event) {
    s32 value = D_8011AB5E[func_0025340C(D_80119870, event->player) * 32];

    func_0029E3E0(item->text + (func_004160F0(item) - 4), D_800C1330, value);
    return 0;
}
