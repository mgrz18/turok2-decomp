#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/game.h"

s32 func_004160F0(MenuItem *item); /* length of the item's text line */
s32 func_0029E3E0(char *buf, const char *fmt, ...); /* sprintf */

extern char D_800B4158[];
extern char D_800B4170[];

s32 func_00206078(void);
void func_00206068(s32);

/* Shows setting 0x0C in sixteenths: a fixed line for 0, otherwise the number. */
s32 func_0045582C(MenuItem *item) {
    u32 value = D_8011AAD8.unkC >> 4;

    func_00206068(func_00206078() + 2);
    if (value == 0) {
        item->text = D_800B4158;
    } else {
        item->text = D_800B4170;
        func_0029E3E0(&item->text[func_004160F0(item) - 4], "  %02d", value);
    }
    return 0;
}
