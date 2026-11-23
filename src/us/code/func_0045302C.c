#include "common.h"
#include "globals/menu_text.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/menu.h"

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */


/* Shows sSoundMenuTrack: a fixed line when 0, otherwise the number. */
s32 SoundMenu_MusicTrackLabel_2(MenuItem *item) {
    if (sSoundMenuTrack == 0) {
        item->text = sSoundMenuText_MusicNone;
    } else {
        item->text = sSoundMenuText_Music;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 2], "%2d", sSoundMenuTrack);
    }
    return 0;
}
