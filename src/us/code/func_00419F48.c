#include "common.h"
#include "structs/menu.h"
#include "globals/game.h"

extern char sSoundMenuText_SfxVolumeNorm[];
extern char sSoundMenuText_SfxVolumeMax[];
extern char sSoundMenuText_SfxVolumeOff[];
extern char sSoundMenuText_SfxVolume[];
extern char sSoundMenuFmt_2d[]; /* "  %2d" */
extern char sSoundMenuFmt_3d[]; /* " %3d" */
extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

/* Label of a slider setting: its value from -16 to 15, in steps of 8 raw units. */
s32 SoundMenu_SfxVolumeLabel(MenuItem *a) {
    s32 v = (gEngineApp_m_Settings.unk15 - 0x80) / 8;

    if (v == 0) {
        a->text = sSoundMenuText_SfxVolumeNorm;
    } else if (v == 15) {
        a->text = sSoundMenuText_SfxVolumeMax;
    } else if (v == -16) {
        a->text = sSoundMenuText_SfxVolumeOff;
    } else if (v > 0) {
        a->text = sSoundMenuText_SfxVolume;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sSoundMenuFmt_2d, v);
    } else {
        a->text = sSoundMenuText_SfxVolume;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sSoundMenuFmt_3d, v);
    }
    return 0;
}
