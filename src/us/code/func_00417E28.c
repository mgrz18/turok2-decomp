#include "common.h"
#include "structs/menu.h"

extern char D_800B42F0[];
extern char D_800C0E70[]; /* "%d"-style format */
extern s32 D_8011ACE0;
extern s32 MenuItem_TextLength(MenuItem *a);
#ifdef TARGET_PORT
/* variadic on the host: arm64 Apple passes variadic arguments on the stack (docs/port/MACOS.md) */
extern void sprintf(char *dst, const char *fmt, ...);
#else
extern void sprintf(char *dst, char *fmt, s32 value);
#endif

/* Prints D_8011ACE0 over the last four characters of the label. */
s32 FinalScoreMenu_RedScoreLabel(MenuItem *a) {
    a->text = D_800B42F0;
    sprintf(a->text + (MenuItem_TextLength(a) - 4), D_800C0E70, D_8011ACE0);
    return 0;
}
