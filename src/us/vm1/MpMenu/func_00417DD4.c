#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"

#include "globals/game.h"
/* the blue team's score read as a scalar: globals/game.h's array (or the member path) changes the ROM's code here */
extern s32 gMatchBlueTeamScore __asm__("gEngineApp_m_Settings_match_teamScore");
extern s32 MenuItem_TextLength(MenuItem *a);
/* variadic on the host: arm64 Apple passes variadic arguments on the stack (docs/port/MACOS.md) */
extern void sprintf(char *dst, const char *fmt, ...);

/* Prints gEngineApp_m_Settings_match_teamScore over the last four characters of the label. */
s32 FinalScoreMenu_BlueScoreLabel(MenuItem *a) {
    a->text = sFinalScoreMenuText_BlueBloodScore;
    sprintf(a->text + (MenuItem_TextLength(a) - 4), "%04d", gMatchBlueTeamScore);
    return 0;
}
