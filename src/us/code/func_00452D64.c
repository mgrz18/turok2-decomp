#include "common.h"
#include "globals/menu_text.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/game.h"

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */


/* Shows setting unk14 (0-255, centred on 0x80) in steps of 8 from -16 to 15. */
s32 SoundMenu_MusicVolumeLabel_2(MenuItem *item) {
    s32 value = (gEngineApp_m_Settings.unk14 - 0x80) / 8;

    if (value == 0) {
        item->text = sSoundMenuText_MusicVolumeNorm;
    } else if (value == 15) {
        item->text = sSoundMenuText_MusicVolumeMax;
    } else if (value == -16) {
        item->text = sSoundMenuText_MusicVolumeOff;
    } else if (value > 0) {
        item->text = sSoundMenuText_MusicVolume;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "  %2d", value);
    } else {
        item->text = sSoundMenuText_MusicVolume;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], " %3d", value);
    }
    return 0;
}
