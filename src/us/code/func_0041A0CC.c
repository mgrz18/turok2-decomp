#include "common.h"
#include "structs/menu.h"

extern s32 D_800C1F78; /* music track */
extern char D_800B6600[];
extern char D_800B6618[];
extern char D_800C1040[]; /* number format */
extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

s32 SoundMenu_MusicTrackLabel(MenuItem *a) {
    if (D_800C1F78 == 0) {
        a->text = D_800B6600;
    } else {
        a->text = D_800B6618;
        sprintf(a->text + (MenuItem_TextLength(a) - 2), D_800C1040, D_800C1F78);
    }
    return 0;
}
