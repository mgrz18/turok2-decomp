#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/game.h"

extern s32 MenuItem_TextLength_2(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

/* Label of a slider setting: its value from -16 to 15, in steps of 8 raw units. */
s32 SoundMenu_MusicVolumeLabel_2(MenuItem *a) {
    s32 v = (gEngineApp_m_Settings.unk14 - 0x80) / 8;

    if (v == 0) {
        a->text = sSoundMenuText_MusicVolumeNorm;
    } else if (v == 15) {
        a->text = sSoundMenuText_MusicVolumeMax;
    } else if (v == -16) {
        a->text = sSoundMenuText_MusicVolumeOff;
    } else if (v > 0) {
        a->text = sSoundMenuText_MusicVolume;
        sprintf(a->text + (MenuItem_TextLength_2(a) - 4), sSoundMenuFmt_2d, v);
    } else {
        a->text = sSoundMenuText_MusicVolume;
        sprintf(a->text + (MenuItem_TextLength_2(a) - 4), sSoundMenuFmt_3d, v);
    }
    return 0;
}
