#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/menu.h"

extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

s32 SoundMenu_MusicTrackLabel(MenuItem *a) {
    if (sSoundMenuTrack == 0) {
        a->text = sSoundMenuText_MusicNone;
    } else {
        a->text = sSoundMenuText_Music;
        sprintf(a->text + (MenuItem_TextLength(a) - 2), "%2d", sSoundMenuTrack);
    }
    return 0;
}
