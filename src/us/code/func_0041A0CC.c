#include "common.h"
#include "structs/menu.h"
#include "globals/menu.h"

extern char sSoundMenuText_MusicNone[];
extern char sSoundMenuText_Music[];
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
