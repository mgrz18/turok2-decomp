#include "common.h"
#include "structs/menu.h"
#include "globals/game.h"

extern char D_800B65D0[];
extern char D_800B65B8[];
extern char D_800B65A0[];
extern char D_800B65E8[];
extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

/* Label of a slider setting: its value from -16 to 15, in steps of 8 raw units. */
s32 SoundMenu_SpeechVolumeLabel(MenuItem *a) {
    s32 v = (gEngineApp_m_Settings.unk16 - 0x80) / 8;

    if (v == 0) {
        a->text = D_800B65D0;
    } else if (v == 15) {
        a->text = D_800B65B8;
    } else if (v == -16) {
        a->text = D_800B65A0;
    } else if (v > 0) {
        a->text = D_800B65E8;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), "  %2d", v);
    } else {
        a->text = D_800B65E8;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), " %3d", v);
    }
    return 0;
}
