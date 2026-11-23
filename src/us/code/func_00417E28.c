#include "common.h"
#include "structs/menu.h"
#include "globals/game.h"

extern char sFinalScoreMenuText_RedBloodScore[];
extern char sFinalScoreMenuFmt_04d[]; /* "%d"-style format */
extern s32 MenuItem_TextLength(MenuItem *a);
#ifdef TARGET_PORT
/* variadic on the host: arm64 Apple passes variadic arguments on the stack (docs/port/MACOS.md) */
extern void sprintf(char *dst, const char *fmt, ...);
#else
extern void sprintf(char *dst, char *fmt, s32 value);
#endif

/* Prints gEngineApp_m_Settings.match.teamScore[1] over the last four characters of the label. */
s32 FinalScoreMenu_RedScoreLabel(MenuItem *a) {
    a->text = sFinalScoreMenuText_RedBloodScore;
    sprintf(a->text + (MenuItem_TextLength(a) - 4), sFinalScoreMenuFmt_04d, gEngineApp_m_Settings.match.teamScore[1]);
    return 0;
}
