/* WRONG CUT: the real function is func_00452EA8, size 0xE0 (splat starts it 8 bytes late, at func_00452EA8,
 * past the two loads GCC scheduled ahead of the prologue). */
#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/game.h"

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */

extern char sSoundMenuText_SfxVolumeNorm[];
extern char sSoundMenuText_SfxVolumeMax[];
extern char sSoundMenuText_SfxVolumeOff[];
extern char sSoundMenuText_SfxVolume[];

/* Shows setting unk15 (0-255, centred on 0x80) in steps of 8 from -16 to 15. */
s32 func_00452EA8(MenuItem *item) {
    s32 value = (gEngineApp_m_Settings.unk15 - 0x80) / 8;

    if (value == 0) {
        item->text = sSoundMenuText_SfxVolumeNorm;
    } else if (value == 15) {
        item->text = sSoundMenuText_SfxVolumeMax;
    } else if (value == -16) {
        item->text = sSoundMenuText_SfxVolumeOff;
    } else if (value > 0) {
        item->text = sSoundMenuText_SfxVolume;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "  %2d", value);
    } else {
        item->text = sSoundMenuText_SfxVolume;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], " %3d", value);
    }
    return 0;
}
