#include "common.h"
#include "structs/menu.h"

extern u8 gEngineApp_m_Settings_unk15; /* gEngineApp_m_Settings + 0x15: 0x80-centred setting */
extern char D_800B6570[];
extern char D_800B6558[];
extern char D_800B6540[];
extern char D_800B6588[];
extern char D_800C1030[]; /* "  %2d" */
extern char D_800C1038[]; /* " %3d" */
extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

/* Label of a slider setting: its value from -16 to 15, in steps of 8 raw units. */
s32 SoundMenu_SfxVolumeLabel(MenuItem *a) {
    s32 v = (gEngineApp_m_Settings_unk15 - 0x80) / 8;

    if (v == 0) {
        a->text = D_800B6570;
    } else if (v == 15) {
        a->text = D_800B6558;
    } else if (v == -16) {
        a->text = D_800B6540;
    } else if (v > 0) {
        a->text = D_800B6588;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), D_800C1030, v);
    } else {
        a->text = D_800B6588;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), D_800C1038, v);
    }
    return 0;
}
