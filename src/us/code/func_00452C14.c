#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */

extern char D_800B65D0[];
extern char D_800B65B8[];
extern char D_800B65A0[];
extern char D_800B65E8[];

/* Shows setting unk16 (0-255, centred on 0x80) in steps of 8 from -16 to 15. */
s32 SoundMenu_SpeechVolumeLabel_2(MenuItem *item) {
    s32 value = (D_8011AAD8.unk16 - 0x80) / 8;

    if (value == 0) {
        item->text = D_800B65D0;
    } else if (value == 15) {
        item->text = D_800B65B8;
    } else if (value == -16) {
        item->text = D_800B65A0;
    } else if (value > 0) {
        item->text = D_800B65E8;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "  %2d", value);
    } else {
        item->text = D_800B65E8;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], " %3d", value);
    }
    return 0;
}
