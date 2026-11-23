#include "common.h"
#include "structs/menu.h"
#include "globals/game.h"

extern char sSoundMenuText_MusicVolumeNorm[];
extern char sSoundMenuText_MusicVolumeMax[];
extern char sSoundMenuText_MusicVolumeOff[];
extern char sSoundMenuText_MusicVolume[];
extern char sSoundMenuFmt_2d[]; /* "  %2d" */
extern char sSoundMenuFmt_3d[]; /* " %3d" */
extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

/* Label of a slider setting: its value from -16 to 15, in steps of 8 raw units. */
s32 SoundMenu_MusicVolumeLabel(MenuItem *a) {
    s32 v = (gEngineApp_m_Settings.unk14 - 0x80) / 8;

    if (v == 0) {
        a->text = sSoundMenuText_MusicVolumeNorm;
    } else if (v == 15) {
        a->text = sSoundMenuText_MusicVolumeMax;
    } else if (v == -16) {
        a->text = sSoundMenuText_MusicVolumeOff;
    } else if (v > 0) {
        a->text = sSoundMenuText_MusicVolume;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sSoundMenuFmt_2d, v);
    } else {
        a->text = sSoundMenuText_MusicVolume;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sSoundMenuFmt_3d, v);
    }
    return 0;
}
