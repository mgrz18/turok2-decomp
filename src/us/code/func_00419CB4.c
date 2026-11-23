#include "common.h"
#include "structs/menu.h"
#include "globals/game.h"

extern char sSoundMenuText_SpeechVolumeNorm[];
extern char sSoundMenuText_SpeechVolumeMax[];
extern char sSoundMenuText_SpeechVolumeOff[];
extern char sSoundMenuText_SpeechVolume[];
extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

/* Label of a slider setting: its value from -16 to 15, in steps of 8 raw units. */
s32 SoundMenu_SpeechVolumeLabel(MenuItem *a) {
    s32 v = (gEngineApp_m_Settings.unk16 - 0x80) / 8;

    if (v == 0) {
        a->text = sSoundMenuText_SpeechVolumeNorm;
    } else if (v == 15) {
        a->text = sSoundMenuText_SpeechVolumeMax;
    } else if (v == -16) {
        a->text = sSoundMenuText_SpeechVolumeOff;
    } else if (v > 0) {
        a->text = sSoundMenuText_SpeechVolume;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), "  %2d", v);
    } else {
        a->text = sSoundMenuText_SpeechVolume;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), " %3d", v);
    }
    return 0;
}
