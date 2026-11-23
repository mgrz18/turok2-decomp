#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/game.h"

extern s32 MenuItem_TextLength(MenuItem *a);
/* variadic on the host: arm64 Apple passes variadic arguments on the stack (docs/port/MACOS.md) */
extern void sprintf(char *dst, const char *fmt, ...);

/* Prints gEngineApp_m_Settings.match.teamScore[1] over the last four characters of the label. */
s32 FinalScoreMenu_RedScoreLabel(MenuItem *a) {
    a->text = sFinalScoreMenuText_RedBloodScore;
    sprintf(a->text + (MenuItem_TextLength(a) - 4), sFinalScoreMenuFmt_04d, gEngineApp_m_Settings.match.teamScore[1]);
    return 0;
}
