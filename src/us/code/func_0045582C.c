#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/game.h"

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */

extern char D_800B4158[];
extern char D_800B4170[];

s32 CCache__GetFrameDelay(void);
void CCache__SetFrameDelay(s32);

/* Shows setting 0x0C in sixteenths: a fixed line for 0, otherwise the number. */
s32 OptionsMenu_BrightnessText_2(MenuItem *item) {
    u32 value = D_8011AAD8.unkC >> 4;

    CCache__SetFrameDelay(CCache__GetFrameDelay() + 2);
    if (value == 0) {
        item->text = D_800B4158;
    } else {
        item->text = D_800B4170;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "  %02d", value);
    }
    return 0;
}
